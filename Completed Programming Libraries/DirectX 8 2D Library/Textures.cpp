//-----------------------------------------------------------------------------
// File: Valkyrie.cpp
//
// Desc: 2D RPG. Top-down view. Nuff said.
//
// Copyright (c) 2001 Columbus North High School Computer Game Creation Team
//-----------------------------------------------------------------------------
#include <d3dx8.h>
#include <mmsystem.h>

#include "dx8wrappers.cpp"

#define DX_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; }}
#define FILENAME "dx5_logo.bmp"

LPD3DXSPRITE sprite;
IDirect3DTexture8 * g_pTexture = NULL;


HRESULT Init(void)
{
	HRESULT hr;
	
	hr = D3DXCreateTextureFromFile(g_pd3dDevice, FILENAME, &g_pTexture);
    if(FAILED(hr))
        return E_FAIL;

	D3DXCreateSprite(g_pd3dDevice, &sprite);
    
    return S_OK;
}


HRESULT PreRender(void)
{
	return S_OK;
}


HRESULT Render(void)
{
	sprite->Draw(g_pTexture, NULL, &D3DXVECTOR2(1.0, 1.0), NULL, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}


HRESULT Shutdown(void)
{
	DX_RELEASE(g_pTexture);
	
	return S_OK;
}
