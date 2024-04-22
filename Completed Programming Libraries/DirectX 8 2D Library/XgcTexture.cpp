class XgcTexture
{
	private:

	protected:
		IDirect3DTexture8 *texture;

		D3DXIMAGE_INFO imginfo;

	public:
		XgcTexture(void);
		Release(void);
		
		HRESULT CreateTexture(DWORD width, DWORD height, DWORD bpp, UINT flags);
	
		HRESULT Lock(void);
		HRESULT Unlock(void);

		HRESULT Load(char *filename);

		inline void Blit(float x, float y, D3DCOLOR colorfx = WHITE);
		inline void BlitScaled(XgcXYSCALE *xyscale, D3DCOLOR colorfx = WHITE);
		inline void BlitX(LPRECT src, XgcXYSCALE *xyscale, D3DCOLOR colorfx, float rot);
	
		inline UINT Width(void);
		inline UINT Height(void);
		inline UINT Depth(void);
		inline IDirect3DTexture8 * Texture(void);
};

XgcTexture::XgcTexture(void)
{
	texture = NULL;
	ZeroMemory(&imginfo, sizeof(D3DXIMAGE_INFO));
}


XgcTexture::Release(void)
{
	DX_RELEASE(texture);
}


inline void XgcTexture::Blit(float x, float y, D3DCOLOR colorfx)
{
	xgc->sprite->Draw(texture, NULL, NULL, NULL, NULL, &D3DXVECTOR2(x, y), colorfx);
}


inline void XgcTexture::BlitScaled(XgcXYSCALE *xyscale, D3DCOLOR colorfx)
{
	xgc->sprite->Draw(texture, NULL, &D3DXVECTOR2(xyscale->xpercent, xyscale->ypercent), NULL, NULL, &D3DXVECTOR2(xyscale->x, xyscale->y), colorfx);
}


inline void XgcTexture::BlitX(LPRECT src, XgcXYSCALE *xyscale, D3DCOLOR colorfx, float rot)
{
	xgc->sprite->Draw(texture, src, &D3DXVECTOR2(xyscale->xpercent, xyscale->ypercent), &D3DXVECTOR2(100, 100), rot, &D3DXVECTOR2(xyscale->x, xyscale->y), colorfx);
}


HRESULT XgcTexture::Load(char *filename)
{
	return D3DXCreateTextureFromFileEx(XgcDevice,filename,D3DX_DEFAULT,D3DX_DEFAULT,1,
									   D3DUSAGE_RENDERTARGET,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
									   D3DX_FILTER_NONE,D3DX_FILTER_NONE,XgcTransparency,
									   &imginfo,NULL,&texture);
}


inline UINT XgcTexture::Width(void)
{
	return imginfo.Width;
}

inline UINT XgcTexture::Height(void)
{
	return imginfo.Height;
}

inline UINT XgcTexture::Depth(void)
{
	return imginfo.Depth;
}

inline IDirect3DTexture8 * XgcTexture::Texture(void)
{
	return texture;
}
