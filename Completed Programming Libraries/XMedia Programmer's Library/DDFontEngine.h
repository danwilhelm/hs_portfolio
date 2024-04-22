//---------------------------------------------------------------------------
//
//  DDFontEngine - Copyright © 2000 Michael Fötsch.
//
//      The DirectDraw Bitmap Font Engine from "Fast Bitmap Fonts
//      for DirectDraw", which can be found at www.mr-gamemaker.com.
//      The tutorial and a TrueType-to-DDF converter are also available at
//      http://www.crosswinds.net/~foetsch
//
//---------------------------------------------------------------------------



#ifndef DDFontEngineH
#define DDFontEngineH


//---------------------------------------------------------------------------

// TEXTCOLOR macro
//      Used to pass textcolor to TDDFont constructor and TDDFont::SetTextColor

#define TEXTCOLOR(r, g, b) ((DWORD) (((BYTE) (b) | \
    ((WORD) (g) << 8)) | \
    (((DWORD) (BYTE) (r)) << 16)))


class TDDFontEngine;


class TDDFont
{

public:
    TDDFont(TDDFontEngine *ddfe, LPCSTR fontfile, DWORD textcolor=TEXTCOLOR(255, 255, 255));
    ~TDDFont();



    TDDFontEngine *Ddfe;
    //LPDIRECTDRAWSURFACE7 lpFontSurf;
	CSurface fontsurface;

    char *FontFile;
    LPBITMAPINFO lpbi;
    LPVOID lpBits;
    TEXTMETRIC TextMetrics;
    ABC ABCWidths[256];
    LOGFONT LogFont;
    int SurfWidth, SurfHeight;
    int CellWidth, CellHeight;
    RECT SrcRects[256];     // Pre-calculated SrcRects for Blt
    int BPlusC[256];
    DWORD TextColor;

    bool LoadFont();
    void SetTextColor(DWORD textcolor);

};

//---------------------------------------------------------------------------



class TDDFontEngine
{

public:
    LPDIRECTDRAW7 lpDD;
    TDDFont *Ddf;

	CXMedia *xmedia;

    TDDFontEngine(CXMedia *xmediain); //LPDIRECTDRAW7 lpdd);
    ~TDDFontEngine();

    void UseFont(TDDFont *ddf);
	int  StrLen(char *str, int numchars = 0);

    HRESULT DrawText(CSurface *dest, int x, int y, char *Text, int numchars = 0);

	//LPDIRECTDRAWSURFACE7 lpDDS
};

//---------------------------------------------------------------------------



#endif

