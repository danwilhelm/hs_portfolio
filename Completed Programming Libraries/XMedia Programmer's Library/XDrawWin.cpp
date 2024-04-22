/* XDraw Windows 95/98 Wrapper Functions		*
 * by Daniel Wilhelm							*/

// WindowProc: Handles our Windows messages
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;	
	HDC				hdc;	

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
			return(0);
		} break;
   
		case WM_PAINT:
		{
   			hdc = BeginPaint(hwnd,&ps);		// validate the window 
        
			EndPaint(hwnd,&ps);				// end painting

			return(0);
   		} break;

		case WM_DESTROY:
		{
			//Shutdown();
			PostQuitMessage(0);				// quit the app

			return(0);
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

	while(TRUE)
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

	Shutdown();							// shutdown the app

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
							WS_POPUP | WS_VISIBLE,	// visible w/parameters
					 		0,0,			// starting x, y
							win_xres,		// window size (x, y)
							win_yres,  
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

	winclass.lpszMenuName	= menu;								// no menu
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
