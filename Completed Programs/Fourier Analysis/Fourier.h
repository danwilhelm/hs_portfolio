//--------------------------------------------------------------------
//Name:		Fourier.h
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

#ifndef FOURIER_H
#define FOURIER_H

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <math.h>

#include "resource.h"
#include "dlgopen.c"
#include "mmio.c"

MCIDEVICEID sounddevice;	// To playback sounds

UCHAR *sndbuffer = NULL;	// holds wave data (UCHAR)
float *dft = NULL;			// holds DFT'd data
float *real = NULL;			// holds wave data (-1 < data < 1)

float highdft = 0;			// highest DFT value (for scaling)

DWORD datasize = 0;				// size of wave file (in bytes)
DWORD samplespersec = 22025;	// samples per second

char wavfilename[128];		// filename of the file to load

int graphx = 585;			// width of graph windows
int graphy = 85;			// height of graph windows

int scrollpos = 0;			// position in wave (sample number)
int scrollzoom = 0;			// position of zoom (in pixels shown)

int viewport[] = {15, 385, 15, 295};	// where graphs should start

bool waveloaded = false;	// is a wave loaded?
bool dftdone	= false;	// has a dft been performed?

HWND hWnd;					// handle to the window
HINSTANCE hInst;			// instance of the app

// DFT-performer function
float *DFT(float *source, HWND hwndDlg);

// Windows functions
LRESULT APIENTRY FourierProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterFourierClass (HINSTANCE hInstance);

// Draw and setup waves
void DrawWAV(HWND hwnd, int wavbegin, int wavend);
void SetupWave(HWND hwndDlg);

// MCI Functions
void MCI_StopPlay(HWND hwnd, MCIDEVICEID device);
bool MCI_Play(HWND hwnd, MCIDEVICEID device, DWORD from);
bool MCI_SetTime(HWND hwnd, MCIDEVICEID device);
void MCI_Close(HWND hwnd, MCIDEVICEID device);
bool MCI_Open(HWND hwnd, LPSTR filename, MCIDEVICEID *device);

#include "mci.cpp"

#endif