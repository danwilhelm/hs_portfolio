//--------------------------------------------------------------------
//Name:		Razor2D.cpp
//Desc:		Functions for 2D operations through D3D
//--------------------------------------------------------------------

#include "Main.h"

//------------------------------------------------------------------------------------------\\
// Not sure if this is yet needed
//
HRESULT RAZOR2D::Init()
{
	return S_OK;
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// Blit function that allows for specification of a Src and Dest Rectangle
// Supports dwFlags: DDBLTFX_MIRRORLEFTRIGHT and DDBLTFX_MIRRORUPDOWN
HRESULT RAZOR2D::BltSprite(RECT *DestRect, LPDIRECT3DTEXTURE8 SrcTexture, RECT *SrcRect, DWORD dwFlags)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// Blit function that allows for specification of a Src and Dest Rectangle
// Supports dwFlags: DDBLTFX_MIRRORLEFTRIGHT and DDBLTFX_MIRRORUPDOWN
// modulate is Alpha Color, rotation is in Radians, and rcenter is the center of rotation
HRESULT RAZOR2D::BltSpriteEx(RECT *DestRect, LPDIRECT3DTEXTURE8 SrcTexture, RECT *SrcRect, DWORD dwFlags,
					 D3DCOLOR modulate, float rotation, POINT *rcenter)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------\\
// Loads a texture to RazorTextures[] and puts the texture number in TexNum
// TexNum is used to pass the texture to any D3D function called
HRESULT RAZOR2D::LoadTexture(char *Filename, int *TexNum)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------//