struct PALETTE_BMP
{
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
};

struct HEADER_BMP
{
	WORD	bfType;			 // must be 0x4D42 for .BMP
	DWORD	bfSize;			 // size of file (in bytes)
	WORD	bfReserved1;	 // must be 0
	WORD	bfReserved2;	 // must be 0
	DWORD	bfOffBits;		 // offset (in bytes) from HEADER to bitmap

	DWORD	biSizeInfo;		 // bytes for info struct
	LONG	biWidth;		 // width of bitmap
	LONG	biHeight;		 // height of bitmap (if positive, flip data)
	WORD	biPlanes;		 // # of color planes
	WORD	biBitCount;		 // bits per pixel
	DWORD	biCompression;	 // type of compression
	DWORD	biSizeImage;	 // size of image in bytes
	LONG	biXPelsPerMeter; // pixels per meter (X-axis)
	LONG	biYPelsPerMeter; // pixels per meter (Y-axis)
	DWORD	biClrUsed;		 // # colors used
	DWORD	biClrImportant;	 // # of important colors

	PALETTE_BMP bmiColors[1];	// palette
};

#define ID_BMP	0x42D2