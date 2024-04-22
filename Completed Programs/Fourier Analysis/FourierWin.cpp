//--------------------------------------------------------------------
//Name:		Fourier.cpp
//Author:	Daniel Wilhelm (12/05/01)
//
//Desc:		Windows app to perform a Discrete Fourier Transform on 
//          any waveform. Supports .WAV loading.
//
//Known Bugs:
//    (1) If zoomed in, position is > 0, and user zooms out, crashes.
//    (2) Sometimes wave graph is not drawn after extended 
//        zooming/positioning.
//--------------------------------------------------------------------

#include "Fourier.h"


/******** THE DFT (Discrete Fourier Transform) ********
/  Transforms real vs. amplitude data to frequency vs. amplitude data
/  NOTE: Imaginary values not calculated!!!
/  
/  Algorithm:
/    DOFOR k = 0 to N - 1
/       DOFOR n = 0 to N - 1
/          angle = k * w0 * n
/          real[k] = real[k] + f(n) * cos(angle) / N
/          imaginary[k] = imaginary[l] - f(n) * sin(angle) / N
/       ENDDO
/    ENDDO
/  
/  O(n) = n^2 (inefficient)
*******************************************************/
float *DFT(float *source, HWND hwndDlg)
{
	char temp[25];
	float angle, w0 = (float)((3.1415926*2)/datasize);
	float *dest;
	unsigned int k, n;

	// Create an array to place the DFT results
	dest = (float *)new float[datasize];
	memset(dest, 0, sizeof(float) * datasize);

	// The DFT
	for(k=0;k<datasize/2;k++)  {
		for(n=0;n<datasize;n++)  {
			angle = k * n * w0;
			dest[k] += (float)((source[n] * cos(angle))/datasize);
		}

		// update the % complete
		itoa((int)((float)k/(float)(datasize/2-1) * 100), temp, 10);
		strcat(temp, "% complete");
		SendDlgItemMessage(hwndDlg, IDC_DFTCOMPLETE, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)temp);
	}
	
	return dest;
}


// Takes wave data and initializes scroll bars, graphics, etc.
void SetupWave(HWND hwndDlg)
{
	unsigned int i;
	SCROLLINFO scrollinfo;
	char temp[50];
	
	// A waveform has been loaded, but its DFT hasn't been calculated
	waveloaded = true;
	dftdone	   = false;

	//SetWindowText(hwndDlg, wavfilename);

	// Display load success, samples/sec, and datasize messages
	strcpy(temp, "Wave loaded successfully.");
	SendDlgItemMessage(hwndDlg, IDC_WAVEPROP, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)temp);
	
	itoa(samplespersec, temp, 10);
	SendDlgItemMessage(hwndDlg, IDC_SAMPLESPERSEC, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)temp);

	itoa(datasize, temp, 10);
	SendDlgItemMessage(hwndDlg, IDC_DATASIZE, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)temp);

	// Setup the initial scroll bar positions (max zoomed out, beginning of wave)
	scrollpos = 0;
	scrollzoom = datasize-graphx;
	
	// Set up the position bar
	scrollinfo.cbSize	 = sizeof(SCROLLINFO);
	scrollinfo.fMask	 = SIF_RANGE | SIF_PAGE | SIF_POS;
	scrollinfo.nMin		 = 0;
	scrollinfo.nMax		 = datasize-scrollzoom-graphx;
	scrollinfo.nPage	 = (datasize-scrollzoom-graphx)/10;
	scrollinfo.nPos		 = scrollpos;
	SendDlgItemMessage(hwndDlg, IDC_POSWAVE, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollinfo);

	// Set up the zoom bar
	scrollinfo.cbSize	 = sizeof(SCROLLINFO);
	scrollinfo.fMask	 = SIF_RANGE | SIF_PAGE | SIF_POS;
	scrollinfo.nMin		 = graphx;
	scrollinfo.nMax		 = datasize;
	scrollinfo.nPage	 = datasize/10;
	scrollinfo.nPos		 = datasize;
	SendDlgItemMessage(hwndDlg, IDC_ZOOMWAVE, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollinfo);
			
	// Enable the scroll bars
	SendDlgItemMessage(hwndDlg, IDC_POSWAVE, WM_ENABLE, TRUE, 0);
	SendDlgItemMessage(hwndDlg, IDC_ZOOMWAVE, WM_ENABLE, TRUE, 0);
	
	// Draw the WAV to the screen
	DrawWAV(hwndDlg, 0, datasize);

	
	// copy the data into a second array scaled from -1 to 1
	if(real) delete real;
	real = (float *)new float[datasize];

	for(i=0;i<datasize;i++)
		real[i] = ((float)(sndbuffer[i])-128)/128;
}


void DrawWAV(HWND hwnd, int wavbegin, int wavend)
{
	HDC hdc;
	HPEN hpen;
	HBRUSH hbrush;

	UCHAR maxamp = 0, minamp = 0xFF;

	int i, j, view = 0;
	float xinc = (float)(wavend - wavbegin)/graphx;
	float yinc = (float)128/graphy;
	float avg;

	// Obtain a handle to the graphics window
	hdc = GetWindowDC(hwnd);
	
	// Draw the waveform outline
	SetMapMode(hdc, MM_ANISOTROPIC);			
	SetViewportOrgEx(hdc, viewport[view*2], viewport[view*2+1], NULL);

	// Create a black pen + black brush
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectObject(hdc, hpen);
	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hbrush);

	// Black out the wave graph
	Rectangle(hdc, 0, -graphy, graphx, graphy);

	// Black out the bar graph
	SetViewportOrgEx(hdc, viewport[2], viewport[3], NULL);
	Rectangle(hdc, 0, 0, graphx, -graphy*2);

	// Draw the axes for the bar graph
	MoveToEx(hdc, 0,-2*graphy, NULL);
	LineTo(hdc, 0,0);
	LineTo(hdc, graphx,0);

	// Create a gray pen
	DeleteObject(hpen);
	hpen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	SelectObject(hdc, hpen);

	// Draw the gray x-axis for the wave graph
	SetViewportOrgEx(hdc, viewport[0], viewport[1], NULL);
	MoveToEx(hdc, 0,0, NULL);
	LineTo(hdc, graphx,0);

	if(waveloaded == false)
		return;

	// Create a bright green pen
	DeleteObject(hpen);
	hpen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	SelectObject(hdc, hpen);

	// For every pixel, find the wave value and display it as a graph
	// NOTE: ALIASING A PROBLEM!
	for(i=0;i<graphx;i++)  {
		j = (int)((i+1)*xinc + wavbegin);
		
		MoveToEx(hdc, i,(int)((sndbuffer[(int)(j-xinc)]-128)/yinc), NULL);
		LineTo(hdc, i+1,(int)((sndbuffer[j]-128)/yinc)+1);
	}

	if(dftdone == false)
		return;
	
	// Create a red pen
	DeleteObject(hpen);
	hpen = CreatePen(PS_SOLID, 1, RGB(225, 0, 0));
	SelectObject(hdc, hpen);
	
	// For every pixel, display the dft as a bar graph
	SetViewportOrgEx(hdc, viewport[2], viewport[3], NULL);
	for(i=0;i<graphx;i++)  {
		
		// Find the largest dft value in the range/pixel
		avg = 0;
		for(j=(int)((i)*xinc+wavbegin);j<(int)((i+1)*xinc+wavbegin);j++)
			avg = __max(avg, dft[j]);

		MoveToEx(hdc, i+1,0, NULL);
		LineTo(hdc, i+1,(int)(-(avg*(highdft))));
	}


	ReleaseDC(hwnd, hdc);
	DeleteObject(hpen);
	DeleteObject(hbrush);
}


// WinMain - entry point of the app
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	hInst = hInstance;

	// Register the Windows class (primarily for icons)
	RegisterFourierClass(hInst);
	
	// Display the dialog box
	DialogBox(hInst, "IDD_FOURIERMAIN", NULL, (DLGPROC)FourierProc);

	return 0;
}


// Registers the class (mainly for an icon)
BOOL RegisterFourierClass(HINSTANCE hInstance)
{
    WNDCLASSEX WndClass;

    WndClass.style          = 0;
    WndClass.lpfnWndProc    = FourierProc;
    WndClass.cbClsExtra     = 0;
    WndClass.cbWndExtra     = DLGWINDOWEXTRA;
    WndClass.hInstance      = hInstance;
    WndClass.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOURIER));
	WndClass.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOURIER));
    WndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );
    WndClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndClass.lpszMenuName   = NULL;
    WndClass.lpszClassName  = "Fourier";

    return(RegisterClassEx(&WndClass));
}


// Message handling function
LRESULT APIENTRY FourierProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HFILE fh;
	SCROLLINFO scrollinfo;
	char temp[25], temp2[25];
	unsigned char sign = 240;
	FILE *outfile;
	char freqin[10];
	int frequency;
	float ratio, avg;
	int mousex, mousey;
	float xinc;
	int i;


	switch(uMsg)
	{
		// When a sound is finished playing
		case MM_MCINOTIFY:

			// Close the sound and enable the play button
			MCI_Close(hwndDlg, sounddevice);
			SendDlgItemMessage(hwndDlg, IDC_PLAYWAVE, WM_ENABLE, TRUE, 0);
			break;
	
		
		// When the window has been drawn over (ie to redraw it...)
		case WM_PAINT:
			
			DrawWAV(hwndDlg, 0, datasize);
			break;
	
		
		// When the dialog first starts, ...
		case WM_INITDIALOG:
						
			// Enable the scroll bars
			SendDlgItemMessage(hwndDlg, IDC_POSWAVE, WM_ENABLE, FALSE, 0);
			SendDlgItemMessage(hwndDlg, IDC_ZOOMWAVE, WM_ENABLE, FALSE, 0);
			break;
	
		
		// When the Horizontal scroll bar is altered...
		case WM_HSCROLL:
			
			// Get the new position of the scroll box
			scrollpos = SendDlgItemMessage(hwndDlg, IDC_POSWAVE, SBM_GETPOS, 0, 0);
			
			switch(LOWORD(wParam)) 
			{ 
				// Right arrow button
				case SB_LINEUP: 

					scrollpos -= datasize/100;
					break;

				// Left arrow button
				case SB_LINEDOWN:

					scrollpos += datasize/100;
					break;
				
				// Pages to the left
				case SB_PAGEUP:
					
					scrollpos -= scrollzoom;					
					break;

				// Pages to the right
				case SB_PAGEDOWN:
					
					scrollpos += scrollzoom;				
					break;

				// Slides the scroll box
				case SB_THUMBTRACK:

					scrollpos = HIWORD(wParam);
					break;
			};

			// Ensure that scroll box position is bounded
			if(scrollpos < 0) scrollpos = 0;
			if(scrollpos > datasize-scrollzoom-graphx) scrollpos = (signed)(datasize-scrollzoom-graphx);

			// Set the new position of the scroll box
			scrollinfo.cbSize	 = sizeof(SCROLLINFO);
			scrollinfo.fMask	 = SIF_POS;
			scrollinfo.nPos		 = scrollpos;

			SendDlgItemMessage(hwndDlg, IDC_POSWAVE, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollinfo);

			// Redraw the wave to reflect changes
			DrawWAV(hwndDlg, scrollpos, scrollpos+scrollzoom);
			break;

		
		// When the Vertical scroll bar is altered...
		case WM_VSCROLL:
						
			// Get the initial position of the scroll box
			scrollzoom = SendDlgItemMessage(hwndDlg, IDC_ZOOMWAVE, SBM_GETPOS, 0, 0);
			
			switch(LOWORD(wParam)) 
			{ 
				// Up arrow button
				case SB_LINEUP: 

					scrollzoom -= datasize/50;
					break;

				// Down arrow button
				case SB_LINEDOWN:

					scrollzoom += datasize/50;
					break;
				
				// Pages up
				case SB_PAGEUP:
					
					scrollzoom -= datasize/10;					
					break;

				// Pages down
				case SB_PAGEDOWN:
					
					scrollzoom += datasize/10;				
					break;

				// Slides the scroll box
				case SB_THUMBTRACK:

					scrollzoom = HIWORD(wParam);
					break;
			};

			// Ensure that scroll box position is bounded
			if(scrollzoom < graphx) scrollzoom = graphx;
			if(scrollpos+scrollzoom > datasize) scrollzoom = (signed)datasize-scrollzoom;

			// Set the new position of the scroll box
			scrollinfo.cbSize	 = sizeof(SCROLLINFO);
			scrollinfo.fMask	 = SIF_POS;
			scrollinfo.nPos		 = scrollzoom;

			SendDlgItemMessage(hwndDlg, IDC_ZOOMWAVE, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollinfo);

			// In case the scroll box is out of bounds (in an impossible position)
			if(datasize-scrollzoom-graphx-scrollpos <= 0)
				scrollpos = datasize-scrollzoom-graphx;
			
			// Set up the position bar
			scrollinfo.cbSize	 = sizeof(SCROLLINFO);
			scrollinfo.fMask	 = SIF_RANGE | SIF_PAGE | SIF_POS;
			scrollinfo.nMin		 = 0;
			scrollinfo.nMax		 = datasize-scrollzoom-graphx;
			scrollinfo.nPage	 = (datasize-scrollzoom-graphx)/10;
			scrollinfo.nPos		 = scrollpos;
			SendDlgItemMessage(hwndDlg, IDC_POSWAVE, SBM_SETSCROLLINFO, TRUE, (LPARAM)&scrollinfo);

			// Draw changes
			DrawWAV(hwndDlg, scrollpos, scrollpos+scrollzoom);

			break;


		case WM_MOUSEMOVE:
			
			mousex = LOWORD(lParam);
			mousey = HIWORD(lParam);
			
			// If the mouse is within the DFT bar graph area ...
			if(mousex > viewport[2] && mousex < viewport[2] + graphx && mousey > viewport[3] - graphy*2 - 30 && mousey < viewport[3] - 30)  {
				if(dftdone)  {
			
					mousex -= viewport[2];
					mousey -= viewport[3];
					
					// xinc = samples/pixel
					xinc = (float)((scrollzoom)/graphx);
					
					// Find the maximum amplitude per pixel
					avg = 0;
					for(i=(int)((mousex)*xinc+scrollpos);i<(int)((mousex+1)*xinc+scrollpos);i++)
						avg = __max(avg, dft[i]);
					
					// Display the Hz
					itoa((int)((mousex)*xinc)*((float)samplespersec/(float)datasize), temp, 10);
					strcat(temp, " Hz");
					SendDlgItemMessage(hwndDlg, IDC_GRAPHFREQ, WM_SETTEXT, 0, (long)temp);

					// Display the Amplitude
					sprintf(temp, "%f", avg);
					strcat(temp, " Amp");
					SendDlgItemMessage(hwndDlg, IDC_GRAPHAMP, WM_SETTEXT, 0, (long)temp);
				}
			}					
			else  {
				
				// Display the mouse's X-coodinate
				strcpy(temp, "MouseX: ");
				strcat(temp, itoa(mousex, temp2, 10));
				SendDlgItemMessage(hwndDlg, IDC_GRAPHFREQ, WM_SETTEXT, 0, (long)temp);

				// Display the mouse's Y-coordinate
				strcpy(temp, "MouseY: ");
				strcat(temp, itoa(mousey, temp2, 10));
				SendDlgItemMessage(hwndDlg, IDC_GRAPHAMP, WM_SETTEXT, 0, (long)temp);
			}
		
			break;
		
		
		// If a button is pressed...
		case WM_COMMAND:
			
			switch(LOWORD(wParam))
			{
			
			// The load button...	
			case IDC_LOADWAVE:
				
				// Display the Open File dialog box
				fh = DlgOpenFile (hwndDlg,
                              "Select a .WAV file",
                              (LONG)OF_EXIST,
                              "*.WAV",
                              wavfilename,
                              NULL
                              );

				// Read in the wave data from the selected file
				if(ReadWaveData(hwndDlg, wavfilename, (char **)&sndbuffer, &datasize, &samplespersec))  {
					SetupWave(hwndDlg);
				}
				else  {
					if(fh)
						MessageBox(hwndDlg, "Cannot open selected wave file.", "Error", MB_OK | MB_ICONEXCLAMATION);
				}

				break;

			
			// The play wave button...
			case IDC_PLAYWAVE:
				
				// Store the memory wave to disk first
				if(!WriteWaveData(hwndDlg, "temp.wav", (char *)sndbuffer, datasize, samplespersec))  {
					MessageBox(hwndDlg, "Cannot save wave file.", "Error", MB_OK | MB_ICONEXCLAMATION);
					break;
				}
				
				// Then play it from disk
				if(MCI_Open(hwndDlg, "temp.wav", &sounddevice))  {
					if(!MCI_Play(hwndDlg, sounddevice, 0))
						MCI_Close(hwndDlg, sounddevice);
				}

				// Enable the play wave button
				SendDlgItemMessage(hwndDlg, IDC_PLAYWAVE, WM_ENABLE, FALSE, 0);

				break;

			
			// The stop playback button...
			case IDC_STOPWAVE:

				// Stop all playback!
				MCI_StopPlay(hwndDlg, sounddevice);
				break;

			
			// The sine, square, or triangle wave button is pressed
			case IDC_SINEWAVE:
			case IDC_SQUAREWAVE:
			case IDC_TRIWAVE:

				// Get the frequency value
				SendDlgItemMessage(hwndDlg, IDC_FREQ, WM_GETTEXT, 10, (long)freqin);
				frequency = atoi(freqin);

				if(!frequency)  {
					MessageBox(hwndDlg, "Specify a frequency.", "Error", MB_OK | MB_ICONEXCLAMATION);
					break;	
				}

				// Set the data/sample size to constants
				datasize = 5000;
				samplespersec = 8000;

				frequency = samplespersec/frequency;

				// Allocate memory for the sound buffer
				if(sndbuffer)
					GlobalReAllocPtr(sndbuffer, datasize, GMEM_MOVEABLE);
				else
					sndbuffer = (UCHAR *)GlobalAllocPtr(GMEM_MOVEABLE, datasize);

				memset(sndbuffer, 0, datasize * sizeof(UCHAR));
				

				// If Sine Wave requested...
				if(LOWORD(wParam) == IDC_SINEWAVE)  {
					
					// SINE WAVE
					for(i=0;i<(int)datasize;i++)  {				
						sndbuffer[i] = (UCHAR)(128 * sin(((i % frequency)/(frequency/(3.1415926*2)))));
						sndbuffer[i] = 256 - (sndbuffer[i] - 128);
					}
				}

				// If Square Wave requested...
				if(LOWORD(wParam) == IDC_SQUAREWAVE)  {
					
					// SQUARE WAVE
					sign = 240;
					for(i=0;i<(int)datasize;i++)  {
						if(i % (frequency))
							sign = 256-sign;
						
						sndbuffer[i] = sign;
					}
				}

				// If Triangle Wave requested...
				if(LOWORD(wParam) == IDC_TRIWAVE)  {
					// RIGHT TRIANGLE WAVE
					for(i=0;i<(int)datasize;i++)  {
						sndbuffer[i] = ((i % frequency - (frequency>>1)) * (128/frequency)) + 128;
					}
				}

				// Initialize sliders, graphics, etc.
				SetupWave(hwndDlg);
				
				break;

			
			// The DFT button is pressed...
			case IDC_DFT:
										
				// Make room for the dft
				if(dft) delete dft;
				
				// Perform it!
				dft = DFT(real, hwndDlg);

				// Make all values positive and find the highest for scaling
				highdft = 0;
				for(i=0;i<(int)(datasize/2);i++)  {
					if(dft[i] < 0)
						dft[i] = -dft[i];
				
					highdft = __max(highdft, dft[i]);
				}

				// (For scaling)
				highdft = graphy*2/highdft;

				dftdone = true;
				DrawWAV(hwndDlg, scrollpos, scrollpos+scrollzoom);
				break;

			
			// The Make Report button is pressed...
			case IDC_MAKEREPORT:		
				
				// If the DFT has been performed
				if(dftdone)  {
					
					// Open the file and write header
					outfile = fopen("C:/temp.txt", "w");
					fprintf(outfile, "Report Generated by Fourier Analysis\n");
					fprintf(outfile, "by Daniel Wilhelm\n\n");
					fprintf(outfile, "FREQUENCY\tAMPLITUDE\tf(t)\n");
					
					// Copy DFT data to it
					for(i=0;i<(int)(datasize/2);i++)  {
						ratio = (float)(i * ((float)samplespersec / (float)datasize));
						fprintf(outfile, "%+03.1f\t\t%05.6f\t\t%+05.3f\n", ratio, dft[i], real[i]);
					}

					fclose(outfile);
					
					// Execute (usually) WordPad with the data
					ShellExecute(hwndDlg, "open", "C:/temp.txt", NULL, NULL, SW_SHOWNORMAL);
				}
				
				break;


			// The Close button is pressed (CleanUp)
			case IDCLOSE:
				if(real) delete real;
				if(dft)  delete dft;
				
				if(sounddevice)
					MCI_Close(hwndDlg, sounddevice);

				if(sndbuffer) 
					GlobalFreePtr(sndbuffer);

				// Close the dialog
				EndDialog(hwndDlg, TRUE);
				break;

			};
			break;
	};

	return 0;

}//------------------------------------------------------------------------------------------//
