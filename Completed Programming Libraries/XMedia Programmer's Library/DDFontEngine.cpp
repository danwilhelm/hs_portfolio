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



#include "DDFontEngine.h"

#define SAFERELEASE(x)      if (x) { x->Release(); x = NULL; }
#define SAFEDELETE(x)       if (x) { delete x; x = NULL; }
#define SAFEDELETEARRAY(x)  if (x) { delete[] x; x = NULL; }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//-------------------------- class TDDFont ----------------------------------
//---------------------------------------------------------------------------



// TDDFont constructor
//      -> ddfe: Initialized TDDFontEngine that will be used to render the font
//      -> fontfile: Name of DDF file that should be loaded
//      -> textcolor: Desired color of text. Use TEXTCOLOR macro.
TDDFont::TDDFont(TDDFontEngine *ddfe, LPCSTR fontfile, DWORD textcolor)
{
    Ddfe = ddfe;
    //fontsurface = NULL;
    FontFile = new char[strlen(fontfile)+1]; strcpy(FontFile, fontfile);
    lpbi = NULL;
    lpBits = NULL;
    TextColor = textcolor;

    LoadFont();                 // Load font file
}

//---------------------------------------------------------------------------

// TDDFont destructor
TDDFont::~TDDFont()
{
    //SAFERELEASE(lpFontSurf);    // Release DirectDraw Surface
    SAFEDELETEARRAY(FontFile);  // Free buffer for file name
    SAFEDELETEARRAY(lpBits);    // Free buffer for bitmap bits
    SAFEDELETEARRAY(lpbi);      // Free buffer for BITMAPINFO
}

//---------------------------------------------------------------------------

// TDDFont::LoadFont
//      Creates a DirectDraw Surface, loads the bitmap in the DDF File
//      into it and reads the text metrics and ABC widths into class structures
//
bool TDDFont::LoadFont()
{
    HANDLE f;
    HDC SurfDC=NULL;
    bool Success=false;

try
{
    // If DDF file is loaded for the first time (i.e. not as a response to
    // DDERR_SURFACELOST), load bitmap data and text metrics.
    // (Otherwise, the structures are already filled. Skip to StretchDIBits.)

    //if (!fontsurface)
    //{

        if ((f = CreateFile(
		        FontFile,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                NULL)) == INVALID_HANDLE_VALUE)
            throw (int)0;

        DWORD BytesRead;
        BITMAPFILEHEADER bmfh;
        ZeroMemory(&bmfh, sizeof(bmfh));
        ReadFile(f, &bmfh, sizeof(bmfh), &BytesRead, NULL);

        // Check whether it's a valid DDF file:
        if ((bmfh.bfType != 19778)  // 'BM'
            || (bmfh.bfReserved1 != 'DD') || (bmfh.bfReserved2 != 'FF'))
            throw (int)1;

        // Read BITMAPINFO and bitmap bits:
        lpbi = (LPBITMAPINFO)(new char[bmfh.bfOffBits - sizeof(bmfh)]);
        if (!lpbi) throw (int)2;    // ("new" should throw anyway but...)
        ReadFile(f, lpbi, bmfh.bfOffBits-sizeof(bmfh), &BytesRead, NULL);

        lpBits = (LPVOID)(new char[bmfh.bfSize-bmfh.bfOffBits]);
        ReadFile(f, lpBits, bmfh.bfSize-bmfh.bfOffBits, &BytesRead, NULL);

        SurfWidth = lpbi->bmiHeader.biWidth;
        SurfHeight = lpbi->bmiHeader.biHeight;

        /*DDSURFACEDESC2 ddsd;
        ZeroMemory(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        ddsd.dwWidth = SurfWidth;
        ddsd.dwHeight = SurfHeight;

        if (FAILED(Ddfe->lpDD->CreateSurface(&ddsd, &lpFontSurf, NULL)))
            throw (int)5;

        DDCOLORKEY ddck;
        ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;
        lpFontSurf->SetColorKey(DDCKEY_SRCBLT, &ddck);*/

		fontsurface.CreateSurface(Ddfe->xmedia, SurfWidth, SurfHeight, 8);


        // Read text metrics:
        ReadFile(f, &TextMetrics, sizeof(TextMetrics), &BytesRead, NULL);
        ReadFile(f, (LPVOID)((DWORD)ABCWidths+32*sizeof(ABC)),
            224*sizeof(ABC), &BytesRead, NULL);
        if (!ReadFile(f, &LogFont, sizeof(LogFont), &BytesRead, NULL))
            throw (int)4;
        if (BytesRead < sizeof(LogFont)) throw (int)4;

        CellWidth = SurfWidth >> 4;
        CellHeight = SurfHeight / 14;

        // Pre-calculate SrcRects:
            ZeroMemory(SrcRects, 32*sizeof(RECT));
            ZeroMemory(ABCWidths, 32*sizeof(ABC));
            ZeroMemory(BPlusC, 32*sizeof(int));

        for (int c=32; c < 256; c++)
        {
            SrcRects[c].left = ((c-32) % 16) * CellWidth;
            SrcRects[c].top = ((c-32) >> 4) * CellHeight;
            SrcRects[c].right = SrcRects[c].left + ABCWidths[c].abcB;
            SrcRects[c].bottom = SrcRects[c].top + CellHeight;
            BPlusC[c] = ABCWidths[c].abcB + ABCWidths[c].abcC;
        }

    //}

    // Change foreground palette entry to requested text color:
    // (Assumes that entry 1 is foreground and 0 is background)
    DWORD *palentry = (DWORD*)&lpbi->bmiColors[1];
    *palentry = TextColor;

    if (FAILED(fontsurface.lpdds->GetDC(&SurfDC))) throw (int)6;
    StretchDIBits(SurfDC,
        0, 0, SurfWidth, SurfHeight,
        0, 0, SurfWidth, SurfHeight,
        lpBits,
        lpbi,
        DIB_RGB_COLORS,
        SRCCOPY);
    fontsurface.lpdds->ReleaseDC(SurfDC);

    Success = true;
}

catch (...)
{
    //SAFERELEASE(lpFontSurf);
}
    CloseHandle(f);

    return Success;
}

//---------------------------------------------------------------------------

// TDDFont::SetTextColor
//      Changes the text color of the font
//      -> textcolor: Desired color of text. Use TEXTCOLOR macro.
void TDDFont::SetTextColor(DWORD textcolor)
{
    TextColor = textcolor;

    // Change foreground palette entry to requested text color:
    // (Assumes that entry 1 is foreground and 0 is background)
    DWORD *palentry = (DWORD*)&lpbi->bmiColors[1];
    *palentry = TextColor;

    HDC SurfDC;
    if (!FAILED(fontsurface.lpdds->GetDC(&SurfDC)))
    {    StretchDIBits(SurfDC,
            0, 0, SurfWidth, SurfHeight,
            0, 0, SurfWidth, SurfHeight,
            lpBits,
            lpbi,
            DIB_RGB_COLORS,
            SRCCOPY);
        fontsurface.lpdds->ReleaseDC(SurfDC);
    }
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//-------------------------- class TDDFontEngine ----------------------------
//---------------------------------------------------------------------------

TDDFontEngine::TDDFontEngine(CXMedia *xmediain) //LPDIRECTDRAW7 lpdd);
{
    //lpDD = lpdd;

	xmedia = xmediain;
    Ddf = NULL;
}

//---------------------------------------------------------------------------

TDDFontEngine::~TDDFontEngine()
{
}

//---------------------------------------------------------------------------

void TDDFontEngine::UseFont(TDDFont *ddf)
{
    Ddf = ddf;
}

//---------------------------------------------------------------------------

HRESULT TDDFontEngine::DrawText(CSurface *dest, int x, int y, char *Text, int numchars)
{
    CSurface *SrcSurf = &Ddf->fontsurface;
/*

    if (SrcSurf->IsLost() == DDERR_SURFACELOST)

    {

        SrcSurf->Restore(); Ddf->LoadFont();

    }

*/
    int StringLength=strlen(Text);
    UCHAR ch;

	if(numchars == 0) numchars = StringLength;


    for (int i=0; i < numchars; i++)
    {
        ch = Text[i];
        x += Ddf->ABCWidths[ch].abcA;
        //lpDDS->BltFast(x, y, SrcSurf, &Ddf->SrcRects[ch],
        //    DDBLTFAST_SRCCOLORKEY);

		RECT destrect = {x, y, x + Ddf->ABCWidths[ch].abcB, y + Ddf->CellHeight};
		dest->BlitX(SrcSurf, &destrect, &Ddf->SrcRects[ch]);
        x += Ddf->BPlusC[ch];
    }

    return DD_OK;

}


int TDDFontEngine::StrLen(char *str, int numchars)
{
	int len = 0, i = 0;

	if(numchars == 0)
		numchars = strlen(str);

	for(i=0;i<numchars;i++)
		len += Ddf->ABCWidths[str[i]].abcA + Ddf->BPlusC[str[i]];

	return len;
}

//---------------------------------------------------------------------------



