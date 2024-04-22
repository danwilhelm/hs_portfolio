//#pragma comment (lib, "d3d8.lib")

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D8             XgcD3D    = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE8       XgcDevice = NULL; // Our rendering device

HWND win_hwnd = NULL;
HINSTANCE win_hinstance = NULL;
DWORD win_msperframe = 33;

int win_xres = 0, win_yres = 0;
char win_title[100] = "XGC Graphics Wrappers App";
BOOL win_fullscreen = FALSE;

D3DDISPLAYMODE XgcDisplayMode = {640, 480, NULL};

HRESULT hr;

#define XgcError(str,hr)	DXTRACE_ERR(str,hr)
#define XgcMsg(str)			DXTRACE_MSG(str)

#define HLTERR(f)  if(hr = f) return XgcError("", hr)
#define RETERR(f)  if(hr = f) return hr
#define RETFAIL(f) if(FAILED(hr = f)) return hr
#define RETSUCC(f) if(SUCCEEDED(hr = f)) return hr


#define DX_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; }}
#define SetFPS(fps)	  win_msperframe = 1000 / fps;

void SetWindow(BOOL fullscreen = FALSE, char *title = win_title, UINT x = 640, UINT y = 480, UINT bpp = NULL);

extern HRESULT Init(void);
extern HRESULT PreRender(void);
extern HRESULT Render(void);
extern HRESULT Shutdown(void);
