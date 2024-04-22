//--------------------------------------------------------------------
//Name:		InitRazor.cpp
//Desc:		Functions for initializing Razor and D3D	
//--------------------------------------------------------------------

#ifndef INITRAZOR_H_
#define INITRAZOR_H_

BOOL APIENTRY InitRazorProc(HWND hwndDlg, UINT uMsg,
							WPARAM wParam, LPARAM lParam);
//int SortModesCallback( const VOID* arg1, const VOID* arg2 );

HRESULT InitDirect3D(void);
HRESULT InitD3DDevice(HWND hwnd);
void	ShutdownDirect3D();
void	GetVideoModes();
void UpdateDlgModeText(int Mode, HWND hwndDlg);

#endif