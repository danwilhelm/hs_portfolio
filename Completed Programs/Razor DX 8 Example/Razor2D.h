#ifndef RAZOR2D_H_
#define RAZOR2D_H_

class RAZOR2D{
public:
	HRESULT Init();
	HRESULT BltSprite(RECT *DestRect, LPDIRECT3DTEXTURE8 SrcTexture, RECT *SrcRect, DWORD dwFlags);
	HRESULT BltSpriteEx(RECT *DestRect, LPDIRECT3DTEXTURE8 SrcTexture, RECT *SrcRect, DWORD dwFlags, D3DCOLOR modulate = 0xFFFFFFFF, float rotation = 0, POINT *rcenter = NULL);
	HRESULT LoadTexture(char *Filename, int *TexNum);
};
#endif