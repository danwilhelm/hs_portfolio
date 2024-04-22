#include "dx8wrappers.h"

/*
void GetVideoModes(void)
{
	LPDIRECT3D8 p_d3dt = Direct3DCreate8 (D3D_SDK_VERSION);
	D3DDISPLAYMODE d3ddm2;
	int allVideoModes = p_d3dt->GetAdapterModeCount (D3DADAPTER_DEFAULT);
	for (int i=0; i<allVideoModes; i++)
	{
		p_d3dt->EnumAdapterModes (D3DADAPTER_DEFAULT, i, &d3ddm2);
		char str[30];
		int bpp=0;
		if(d3ddm2.Format == D3DFMT_X1R5G5B5) bpp=16;
		if(d3ddm2.Format == D3DFMT_R5G6B5) bpp=16;
		if(d3ddm2.Format == D3DFMT_A1R5G5B5) bpp=16;
		if(d3ddm2.Format == D3DFMT_A4R4G4B4) bpp=16;
		if(d3ddm2.Format == D3DFMT_X4R4G4B4) bpp=16;
		if(d3ddm2.Format == D3DFMT_R8G8B8) bpp=24;
		if(d3ddm2.Format == D3DFMT_X8R8G8B8) bpp=32;
		if(d3ddm2.Format == D3DFMT_A8R8G8B8) bpp=32;
		sprintf(str, "%dx%dx%d _ %dhz", d3ddm2.Width, d3ddm2.Height, bpp, d3ddm2.RefreshRate);
		ComboBox_AddString (hFSMlist, str);
	};
	_RELEASE_ (p_d3dt);
	ComboBox_SetCurSel (hFSMlist, 0);
}*/


void SetWindow(BOOL fullscreen, char *title, UINT x, UINT y, UINT bpp)
{
	CopyMemory(win_title, title, strlen(title)+1);
	win_fullscreen = fullscreen;
	
	XgcDisplayMode.Width = x;
	XgcDisplayMode.Height = y;
	
	switch(bpp)
	{
		case 8:
			XgcDisplayMode.Format = D3DFMT_R3G3B2;
		break;

		case 16:
			XgcDisplayMode.Format = D3DFMT_R5G6B5;         
		break;

		case 24:
			XgcDisplayMode.Format = D3DFMT_R8G8B8;
		break;

		case 32:
			XgcDisplayMode.Format = D3DFMT_A4R4G4B4;
		break;

		default:
			XgcDisplayMode.Format = D3DFMT_UNKNOWN;
	};

	// Create the application's window  
	if(win_fullscreen)
	{		
		win_hwnd = CreateWindow( "XgcApp", win_title, WS_POPUP, 0,0, x, y,
								  GetDesktopWindow(), NULL, win_hinstance, NULL );
	}
	else
	{
		win_hwnd = CreateWindow( "XgcApp", win_title,
							 WS_OVERLAPPEDWINDOW, 
							 win_xres/2 - x/2, win_yres/2 - y/2, 
							 x, y,  
							 GetDesktopWindow(), NULL, win_hinstance, NULL );
	}	
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	Shutdown();

    DX_RELEASE(XgcDevice);
    DX_RELEASE(XgcD3D);
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
    // For our world matrix, we will just leave it as the identity
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    //D3DXMatrixRotationX( &matWorld, timeGetTime()/1000.0f );
    XgcDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 3.0f,-5.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    XgcDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    XgcDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT SetupRender()
{
    RETERR(PreRender());
	
    // Begin the scene
    XgcDevice->BeginScene();

    // Setup the world, view, and projection matrices
    SetupMatrices();

	RETERR(Render());

    // End the scene
    XgcDevice->EndScene();

    // Present the backbuffer contents to the display
    XgcDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{  
	win_xres	  = GetSystemMetrics (SM_CXSCREEN);
	win_yres	  = GetSystemMetrics (SM_CYSCREEN);
	win_hinstance = hInst;

	
	// Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "XgcApp", NULL };
    RegisterClassEx( &wc );

    // Create the scene geometry
    if(SUCCEEDED(Init()))
    {
        // Show the window
        ShowWindow(win_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(win_hwnd);

        // Enter the message loop
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while( msg.message!=WM_QUIT )
        {
			DWORD msframe = GetTickCount();

            if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
                if(SetupRender())
					msg.message = WM_QUIT;

			while((GetTickCount() - msframe) < 0);//win_msperframe);
        }
    }

    // Clean up everything and exit the app
    Cleanup();
    UnregisterClass( "Valkyrie Dawn", wc.hInstance );
    return 0;
}