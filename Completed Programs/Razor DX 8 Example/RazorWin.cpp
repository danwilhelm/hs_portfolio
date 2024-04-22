//--------------------------------------------------------------------
//Name:		RazorWin.cpp
//Desc:		Wrappers for common Windows tasks
//--------------------------------------------------------------------

#include "Main.h"

//------------------------------------------------------------------------------------------\\
// This is your main windows function which is run first.
// It should not be declared and it should keep the current arguement list it has
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	//Windows message structure, used to get and handle messages
	MSG msg;

	//Setup the Instance to be used by anything later
	App.hInst = hInst;
	//Set it to false to beign with
	CheckSurfaces = false;
	//The app starts active
	Active = true;
	
	// Start D3D
	if(InitDirect3D())
	{
		MessageBox(NULL, "Could Not Initialize DirectX 8.0", "Razor 3D Engine", MB_OK);
		return false;
	}

	GetVideoModes();
	
	// Start the engine setup dialog box
	if(!DialogBox(hInst, "IDD_RAZORINIT", NULL, InitRazorProc))
		return false;  

	//Get rid of the cursor, if we need it, we'll draw it ourselves
	ShowCursor(false);

	//Creates the window and stores the HWND into App.hWnd
	if(!MakeWindow(ScreenWidth, ScreenHeight))
	{
		MessageBox(NULL, "Could Not Create Window", "Razor 3D Engine", MB_OK);
		return false;
	}

	// Creates the device w/ the proper video mode, etc.
	if(InitD3DDevice(App.hWnd))
	{
		MessageBox(NULL, "Could Not Initialize DirectX 8.0 Device", "Razor 3D Engine", MB_OK);
		return false;
	}
    
	// Clear the back buffer
	D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1, 0 );

	RazorKeys.Init();

//-----------------Message Loop-------------------------------------------------\\
	//
	while(runnin)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			if(!Active)
				CheckSurfaces = true;
			else{
				if(CheckSurfaces)
				{
					CheckSurfaces = false;
				}

				MainLoop();
			}
		}
	}
//----------------End Message Loop----------------------------------------------//
	return true;
}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------\\
// This is the WindowProclamation function
// This handles all the messages that windows sends to your program
// The main function receives the messages and tells WindProc to handle it
LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Switch the message because its the easiest way to handle it
	switch(msg)
	{
		//Tells us whether our app is active or not
		case WM_ACTIVATEAPP:
			Active = (BOOL)wParam;
			break;
		
		// Someone is trying to exit	
		case WM_KEYDOWN:
			runnin = false;
			break;
			
		//Someone is trying to kill us
		case WM_DESTROY:
			runnin = false;
			break;

		//Let Windows handle it, b/c I dont know what to do with it
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//





//------------------------------------------------------------------------------------------\\
// MakeWindow(640, 480, hInst);
// This creates the window with a certain height and width using CreateWindowEx
// This should be run from the main function before the message loop
BOOL MakeWindow(int WinWidth, int WinHeight)
{
	
	//Define the window class
	WNDCLASS WndCls;
	
	//Set the App's HWND to null so we can make sure it worked later
	App.hWnd = NULL;
	
	//Tell it what the name of the WindowProclamation function is
	WndCls.lpfnWndProc = WindProc;
	
	//Default style for DirectDraw apps
	WndCls.style = CS_HREDRAW | CS_VREDRAW;
	
	//I dunno what this is, just set it to 0
	WndCls.cbClsExtra = 0;
	
	//I dunno what this is, just set it to 0
	WndCls.cbWndExtra = 0;
	
	//Tell it what the name of the HINSTANCE is
	WndCls.hInstance = App.hInst;
	
	//No need for an icon for now
	WndCls.hIcon = NULL;
	
	//Default Cursor
	WndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	//The menu names, even though we dont use it, set it
	WndCls.lpszMenuName = "Razor 3D Engine";
	
	//Sets the app's background to black
	WndCls.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	
	//The app's class name
	WndCls.lpszClassName = "Razor 3D Engine";
	
	//We must register the class with windows
	if(!RegisterClass(&WndCls))
		return false;
	
	//And then we create the window
	App.hWnd = CreateWindowEx(WS_EX_APPWINDOW, "Razor 3D Engine", "Razor 3D Engine",
		WS_VISIBLE | WS_POPUP, 0, 0, WinWidth, WinHeight, NULL, NULL, App.hInst, NULL);
	
	//If we dont have a window, end the program
	if(App.hWnd == NULL)
		return false;
	
	//Succes, Update the window
	UpdateWindow(App.hWnd);
	return true;
}
//------------------------------------------------------------------------------------------//

