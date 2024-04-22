/************************************************************************
	CBitmap.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

#include "CBitmap.h"
#include <fstream.h>

BOOL CBitmap::Load(const char *filename)
{
	int i;
	const char *extension = NULL;
	const char *filenamecopy = filename;

	// NOTE: modified from PATH.C, Microsoft libraries
	for (extension = NULL; *filenamecopy; filenamecopy++)
	{         
		switch (*filenamecopy) 
		{         
			case '.':             
				extension = filenamecopy + 1; // remember after the last dot        
			break;
			  
			case '\\':         
			case ' ':			  // extensions can't have spaces             
				extension = NULL; // forget last dot, it was in a directory             
			break;         
		}     
	}      

	if(extension == NULL)
		return FALSE;

	for(i=0;i<3;i++)
		toupper(extension[i]);

	for(i=0;extfunc[i].ExtFunc != NULL;i++)
	{
		if(extension[0] == extfunc[i].ext[0] && 
		   extension[1] == extfunc[i].ext[1] && 
		   extension[2] == extfunc[i].ext[2])
		{
			return (extfunc[i].ExtFunc(filename));
		}
	}
	
	return FALSE;
}


BOOL CBitmap::LoadBMP(const char *filename)
{
	int i;
	fstream file;
	
	HEADER_BMP	bmpheader;
	PALETTE_BMP	bmppal[MAX_PALCOLORS];

	file.open(filename, ios::in);

	if(file.fail())
		return FALSE;

	file.read((LPBYTE)&bmpheader, sizeof(HEADER_BMP));

	if(bmpheader.bfType != ID_BMP)
	{
		file.close();
		return FALSE;
	}

	if(bmpheader.biBitCount == 8)
	{
		file.read((LPBYTE)&bmppal, sizeof(PALETTE_BMP)*MAX_PALCOLORS);
		
		for(i=0;i<MAX_PALCOLORS;i++)
		{
			palette[i].red   = bmppal[i].rgbRed;
			palette[i].green = bmppal[i].rgbGreen;
			palette[i].blue	 = bmppal[i].rgbBlue;
		}
	}

	file.seekg(bmpheader.biSizeImage, ios::end);

	if(bmpheader.biBitCount != 8  && bmpheader.biBitCount != 16 &&
	   bmpheader.biBitCount != 24 && bmpheader.biBitCount != 32)
		return FALSE;
	
	if(buffer)
		delete [] buffer;

	if(!(buffer = new UCHAR[bmpheader.biSizeImage]))
	{
		file.close();
		return FALSE;
	}

	// uncompressed
	if(bmpheader.biCompression == BI_RGB)
		file.read(buffer, bmpheader.biSizeImage);
	else
	{
		delete [] buffer;
		file.close();
		return FALSE;
	}

	file.close();

	width	= bmpheader.biWidth;
	height	= abs(bmpheader.biHeight);
	bpp		= bmpheader.biBitCount;
	size	= bmpheader.biSizeImage;

	if(bmpheader.biHeight > 0)
		this->Flip();

	return TRUE;
}


BOOL CBitmap::Flip(void)
{
	int bytesperline = (bpp >> 3) * width;
	LPBYTE tempbuf = new UCHAR[size];
	
	if(!tempbuf) 
		return FALSE;
	
	memcpy(tempbuf, buffer, size);
	
	for(DWORD i=0;i<height;i++)
		memcpy(&buffer[((height-1) - i) * bytesperline],
			   &tempbuf[i*bytesperline], bytesperline);

	delete [] tempbuf;		 
	
	return TRUE;
}


BOOL CBitmap::Mirror(void)
{
	return TRUE;
}


BOOL CBitmap::AddExtension(char extension[], BOOL (*Func)(const char *))
{
	int i = 0;
	
	while(extfunc[i].ExtFunc != NULL) 
		if(i++ > MAX_EXTFUNC) 
			return FALSE;

	extfunc[i].ext[0] = toupper(extension[0]);
	extfunc[i].ext[1] = toupper(extension[1]);
	extfunc[i].ext[2] = toupper(extension[2]);

	extfunc[i].ExtFunc = Func;	
	
	return TRUE;
}


CBitmap::CBitmap(void)
{
	int i;

	buffer = NULL;

	for(i=0;i<MAX_EXTFUNC;i++)
	{
		extfunc[i].ext[0]  = NULL;
		extfunc[i].ExtFunc = NULL;
	}
	
	AddExtension("BMP", LoadBMP);
}


CBitmap::~CBitmap(void)
{
	if(buffer)
		delete [] buffer;
}

