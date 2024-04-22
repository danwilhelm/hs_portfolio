/************************************************************************
	XMediaWin.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

int gamerunning = TRUE;

// WindowProc: Handles our Windows messages
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;	
	HDC				hdc;
    POINT           p = {0, 0}; // Translation point for the window's client region
	

	switch(msg)
	{	
		case WM_COMMAND:					
        {
			switch(LOWORD(wparam))			// put MENU resources in here
			{
				case 0: break;

				default: break;
			}
		}


		case WM_CREATE:
		{
			return 0;
		} break;
   
		case WM_PAINT:
		{
   			hdc = BeginPaint(hwnd,&ps);		// validate the window 
        
			EndPaint(hwnd,&ps);				// end painting

			return 0;
   		} break;

		case WM_MOVE:
		{
/*            if (!IsIconic(hwnd))
            {

                g_rcSrc.left = 0;
                g_rcSrc.right = BMAPX;
                g_rcSrc.top = 0;
                g_rcSrc.bottom = BMAPY;
                GetClientRect(hwnd, &g_rcDst);
                g_dwXRatio = (g_rcDst.right - g_rcDst.left) * 1000 /
                             (g_rcSrc.right - g_rcSrc.left);
                g_dwYRatio = (g_rcDst.bottom - g_rcDst.top) * 1000 /
                             (g_rcSrc.bottom - g_rcSrc.top);
                ClientToScreen(hwnd, &p);
                g_rcDst.left = p.x;
                g_rcDst.top = p.y;
                g_rcDst.bottom += p.y;
                g_rcDst.right += p.x;
                CheckBoundries();
            }*/

/*            g_iWinPosX = (int) (short) LOWORD(lParam);
            g_iWinPosY = (int) (short) HIWORD(lParam);
            return DefWindowProc(hWnd, uMsg, wParam, lParam);*/

		} break;


		case WM_SIZE:
		{
			/*if (wParam != SIZE_MINIMIZED)
			{
				GetClientRect(hwnd, &g_rcDst);
				ClientToScreen(hwnd, &p);
				g_rcDst.left = p.x;
				g_rcDst.top = p.y;
				g_rcDst.bottom += p.y;
				g_rcDst.right += p.x;
				g_rcSrc.left = 0;
				g_rcSrc.right = BMAPX;
				g_rcSrc.top = 0;
				g_rcSrc.bottom = BMAPY;
				// Here we multiply by 1000 to preserve 3 decimal places in the
				// division opperation (we picked 1000 to be on the same order
				// of magnitude as the stretch factor for easier comparisons)
				g_dwXRatio = (g_rcDst.right - g_rcDst.left) * 1000 /
							 (g_rcSrc.right - g_rcSrc.left);
				g_dwYRatio = (g_rcDst.bottom - g_rcDst.top) * 1000 /
							 (g_rcSrc.bottom - g_rcSrc.top);
				CheckBoundries();
        
				return 0;
			}*/

            /*g_iWinWidth = (int) (short) LOWORD(lParam);
            g_iWinHeight = (int) (short) HIWORD(lParam);
            return DefWindowProc(hWnd, uMsg, wParam, lParam);*/

		} break;


		case WM_DESTROY:
		{
			Shutdown();
			PostQuitMessage(0);				// quit the app

			return 0;
		} break;

		default:break;
	}

	return (DefWindowProc(hwnd, msg, wparam, lparam));	// default actions
}


// WINMAIN: the real main function
int WINAPI WinMain(	HINSTANCE hinstance,		// app instance
					HINSTANCE hprevinstance,	// instance of calling app
					LPSTR lpcmdline,			// command line params
					int ncmdshow)				// show state of window
{

	MSG		   msg;

	win_hinstance = hinstance;

	Init();					// SETUP WINDOW PARAMS

	if(win_windowmade == FALSE)		// if a window isn't made, make one!
		CreateFullScreen();

	while(gamerunning)
	{
		DWORD start_time = GetTickCount();
		
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// checks for a msg
		{ 
		   if(msg.message == WM_QUIT)	// if a quit msg, exit loop
		   	   break;

		   TranslateMessage(&msg);		// translate acc keys

		   DispatchMessage(&msg);		// send msg to our WindowProc
		}

		Main();							// MAIN FUNCTION

		while((GetTickCount() - start_time) < 33);
	}

	return(msg.wParam);					// return to Windows
}


int CreateWindowed(LPCTSTR title, int x, int y, int width, int height, DWORD style)
{	
	if(win_classreg == FALSE)
		MyResources();
	
	if (!(win_hwnd = CreateWindowEx(NULL,   // extended styles
							WIN_CLASSNAME,	// class name
							title,			// window title bar
							style,			// visible w/parameters
							x,				// window start (x,y)
							y,
							width,			// window size (x,y)
							height,
							NULL,			// handle to parent 
							NULL,			// handle to menu
							win_hinstance,	// app instance
							NULL)))
		return(FALSE);

	win_windowed   = TRUE;
	win_windowmade = TRUE;	
	return(TRUE);
}


int CreateFullScreen(LPCTSTR title)
{
	if(win_classreg == FALSE)
		MyResources();
	
	if (!(win_hwnd = CreateWindowEx(NULL,   // extended styles
							WIN_CLASSNAME,  // class name
							title,			// window title bar
							WS_POPUP | WS_VISIBLE | WS_CAPTION,	// visible w/parameters
					 		0,0,			// starting x, y
							640,			// window size (x, y)
							480,			// (win_xres, win_yres)
							NULL,			// handle to parent 
							NULL,			// handle to menu
							win_hinstance,	// app instance
							NULL)))
		return(FALSE);

	win_windowed   = FALSE;
	win_windowmade = TRUE;
	return(TRUE);
}



int MyResources(LPCTSTR icon, LPCTSTR smicon, LPCTSTR cursor, LPCTSTR menu)
{
	WNDCLASSEX winclass;

	if(icon == NULL) 
		winclass.hIcon		= LoadIcon(NULL, IDI_APPLICATION);		// load icon
	else		
		winclass.hIcon		= LoadIcon(win_hinstance, icon);
	
	if(smicon == NULL) 
		winclass.hIconSm	= LoadIcon(NULL, IDI_APPLICATION);
	else			   
		winclass.hIconSm	= LoadIcon(win_hinstance, smicon);

	if(cursor == NULL) 
		winclass.hCursor	= LoadCursor(NULL, IDC_ARROW);
	else
		winclass.hCursor	= LoadCursor(win_hinstance, cursor);

	winclass.lpszMenuName	= menu;									// no menu
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	// load background (black)

	// fill in the rest of the window class stucture
	winclass.cbSize         = sizeof(WNDCLASSEX);		// struct size
	winclass.style			= CS_DBLCLKS | CS_OWNDC |	// check for dbl-clicks, window has own DC,
							  CS_HREDRAW | CS_VREDRAW;	// does horiz. and vert. redraws
	winclass.lpfnWndProc	= WindowProc;				// main message handler
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= win_hinstance;			// app instance
	winclass.lpszClassName	= WIN_CLASSNAME;			// class name

	if (!RegisterClassEx(&winclass))					// now register the class + make the window
		return(FALSE);
	
	win_classreg = TRUE;
	return(TRUE);
}
