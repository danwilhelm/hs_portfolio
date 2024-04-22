//-----------------------------------------------------------------------------
// File: Valkyrie.cpp
//
// Desc: 2D RPG. Top-down view. Nuff said.
//
// Copyright (C) 2001 Columbus North High School Computer Game Creation Team
//-----------------------------------------------------------------------------
#define INITGUID

#include <d3dx8.h>
#include <dinput.h>
#include <dxerr8.h>
#include <dmusici.h>
#include <objbase.h>

#include "dx8wrappers.cpp"
#include "Xgc.cpp"

Xgc *xgc = NULL;

#include "XgcTexture.cpp"
#include "XgcAudioClip.cpp"

#define FILENAME "dx5_logo.bmp"

XgcTexture		*dxtexture	= NULL;
XgcAudioClip	*audio		= NULL;
XgcAudioClip	*music		= NULL;


HRESULT Init(void)
{
	SetWindow(FALSE, "DirectX 8 2D Library Demonstration", 800,600);
	
	xgc = new Xgc;
	HLTERR(xgc->Create());

	dxtexture	= new XgcTexture;
	audio		= new XgcAudioClip;
	music		= new XgcAudioClip;

	HLTERR(dxtexture->Load(FILENAME));

	HLTERR(xgc->SetAudioDirectory("C:/Programming/Microsoft Visual Studio/dx8vcsdk/Samples/Multimedia/Media"));
	HLTERR(audio->Load("audiopath3.wav"));
	HLTERR(music->Load("canyon.mid"));

	HLTERR(music->Play());

	HLTERR(audio->Play(5));
	
    return S_OK;
}


HRESULT PreRender(void)
{
	return S_OK;
}


HRESULT Render(void)
{
	HLTERR(xgc->GetInput());
	xgc->Clear();

	dxtexture->Blit(0,0, BLUE);

	if(ButtonDown(MOUSE_LEFT))
		dxtexture->Blit(dxtexture->Width(), dxtexture->Height());

	//for(int i=0;i<200;i++)
	//	xgc->PutPixel(i,i, D3DCOLOR_ARGB(255, 255, 255, 255));

	//xgc->Line(0,0, 100,100, WHITE);

	if(ButtonDown(MOUSE_RIGHT))
		return 1;

	return S_OK;
}


HRESULT Shutdown(void)
{
	audio->Release();
	music->Release();
	xgc->Release();
	
	return S_OK;
}
