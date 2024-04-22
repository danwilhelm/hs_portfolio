//--------------------------------------------------------------------
//Name:		InitRazor.cpp
//Desc:		Functions for initializing Razor and D3D
//--------------------------------------------------------------------

#include "Main.h"

//------------------------------------------------------------------------------------------\\
// Proclamation for the InitRazor Window that shows up first
// It should keep the current arguement list it has
BOOL APIENTRY InitRazorProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO	scrollInfo;
	int			scrollPos;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			
			// Set up the scroll bar
			scrollInfo.cbSize	 = sizeof(SCROLLINFO);
			scrollInfo.fMask	 = SIF_RANGE | SIF_PAGE | SIF_POS;
			scrollInfo.nMin		 = 0;
			scrollInfo.nMax		 = nModes-1;
			scrollInfo.nPage	 = 1;
			scrollInfo.nPos		 = 0;

			CheckRadioButton(hwndDlg, IDC_HAL, IDC_REF, IDC_HAL);
			CheckRadioButton(hwndDlg, IDC_WINDOWED, IDC_FULLSCREEN, IDC_FULLSCREEN);
			SendDlgItemMessage(hwndDlg, IDC_VIDEORES, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollInfo);

			UpdateDlgModeText(0, hwndDlg);

			break;
	
		case WM_HSCROLL:
			
			// Get the initial position of the scroll box
			scrollPos = SendDlgItemMessage(hwndDlg, IDC_VIDEORES, SBM_GETPOS, 0, 0);
			
			switch(LOWORD (wParam)) 
			{ 
				// Right arrow button
				case SB_LINEUP: 

					scrollPos--;
					break;

				// Left arrow button
				case SB_LINEDOWN:

					scrollPos++;
					break;
				
				// Pages to the left
				case SB_PAGEUP:
					
					scrollPos -= 2;
					break;

				// Pages to the right
				case SB_PAGEDOWN:
								
					scrollPos += 2;
					break;

				// Slides the scroll box
				case SB_THUMBTRACK:

					scrollPos = HIWORD(wParam);
					break;
			};

			// Ensure that scroll box position is bounded
			if(scrollPos < 0) scrollPos = 0;
			if(scrollPos >= nModes) scrollPos = nModes - 1;

			// Set the new position of the scroll box
			scrollInfo.cbSize	 = sizeof(SCROLLINFO);
			scrollInfo.fMask	 = SIF_POS;
			scrollInfo.nPos		 = scrollPos;

			SendDlgItemMessage(hwndDlg, IDC_VIDEORES, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollInfo);

			UpdateDlgModeText(scrollPos, hwndDlg);

			currentMode = scrollPos;

			break;   

	
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_HAL:
					deviceType = D3DDEVTYPE_HAL;       

					return TRUE;
					break;			

				case IDC_REF:
					deviceType = D3DDEVTYPE_REF;
					
					return TRUE;
					break;
				
				case IDC_WINDOWED:
					SendDlgItemMessage(hwndDlg, IDC_VIDEORES, WM_ENABLE, FALSE, 0);
					isWindowed = true;

					return TRUE;
					break;
				

				case IDC_FULLSCREEN:
					SendDlgItemMessage(hwndDlg, IDC_VIDEORES, WM_ENABLE, TRUE, 0);
					isWindowed = false;

					return TRUE;
					break;

				case IDOK:
					EndDialog( hwndDlg, TRUE );

					return TRUE;
					break;

				case IDCANCEL:
					EndDialog( hwndDlg, FALSE );

					return TRUE;
					break;
			};
			break;
	};

	return FALSE;
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// UpdateDlgModeText()
// Updates the video mode display in the InitRazor dialog box
void UpdateDlgModeText(int Mode, HWND hwndDlg)
{
	int		nBits;
	char	modeDisplay[15];
	
	switch(videoModes[Mode].Format)
	{
		// 16-bit modes
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
			nBits = 16; break;

		// 24-bit modes
		case D3DFMT_R8G8B8:		
			nBits = 24;	break;

		// 32-bit modes
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:	
			nBits = 32; break;
	};

	// Update the display above the scroll bar
	sprintf(modeDisplay, "%ix%ix%i", videoModes[Mode].Width, videoModes[Mode].Height, nBits);
	SendDlgItemMessage(hwndDlg, IDC_RESTEXT, WM_SETTEXT, 0, (LPARAM)modeDisplay);
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// InitDirect3D();
// Initializes Direct3D
HRESULT InitDirect3D()
{
	Direct3D = Direct3DCreate8(D3D_SDK_VERSION);
	

	// Obtain the current display mode
	D3DDISPLAYMODE d3ddm;
	Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	return S_OK;
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// InitD3DDevice();
// Initializes the D3D Device
HRESULT InitD3DDevice(HWND hwnd)
{
    // Set up the D3D parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.BackBufferWidth = videoModes[currentMode].Width;
	d3dpp.BackBufferHeight = videoModes[currentMode].Height;
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = isWindowed;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = videoModes[currentMode].Format;
	d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	
	// Set the refresh rate only if it is not an adapter default
	if(videoModes[currentMode].RefreshRate)
		d3dpp.FullScreen_RefreshRateInHz = videoModes[currentMode].RefreshRate;
	else
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    
	// Create the D3DDevice
    if( FAILED( Direct3D->CreateDevice( D3DADAPTER_DEFAULT, deviceType, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &D3DDevice ) ) )
    {
        return E_FAIL;
    }

	ScreenWidth	 = videoModes[currentMode].Width;
	ScreenHeight = videoModes[currentMode].Height;

    // Turn off culling
    D3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting
    D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn off the zbuffer
    D3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

	return S_OK;
}
// End InitD3DDevice
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// GetVideoModes()
// Retrieves a list of all available video modes
// Call just before exiting program anywhere
void GetVideoModes()
{
	// Get the number of available video modes
	nModes = Direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT);
	
	// Allocate enough memory for them all
	videoModes = new D3DDISPLAYMODE[nModes];

	// Enumerate the video modes
	int i;
	for(i=0;i<nModes;i++)
		Direct3D->EnumAdapterModes(D3DADAPTER_DEFAULT, i, &videoModes[i]);

}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------\\
// ShutdownDirect3D();
// Properly shuts down Direct3D
// Call just before exiting program anywhere
void ShutdownDirect3D()
{
  HELPER_RELEASE(D3DTexture);
  HELPER_RELEASE(D3DIndexBuffer);
  HELPER_RELEASE(D3DVertexBuffer);
  HELPER_RELEASE(D3DDevice);
  HELPER_RELEASE(Direct3D);
}
//------------------------------------------------------------------------------------------//
