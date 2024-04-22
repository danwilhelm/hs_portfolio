#include <stdio.h>  

void RLEDAT_GetPic(char FileName[], BUFPIC &BufPicture, int PicNum);
void RLEDAT_GetPalette(char FileName[], PALETTE &Palette);

void RLEDAT_GetPic(char FileName[], BUFPIC &BufPicture, int PicNum)
{
	FILE *RLEstream;
	int i, NumImages, Width, Height, Char;
	int bufpos = 0, CurrX = 0, CurrY = 0, Run;
	long DataLen;                    
	int Drawing = 1;
		
		
	if ((RLEstream = fopen(FileName, "rb")) == NULL)
		Exit_Program("Failed to open RLEDAT file!");
		
	fseek(RLEstream, 6, SEEK_SET);
	fread(&NumImages, 2, 1, RLEstream);
		
	fseek(RLEstream, 768, SEEK_CUR);
	
	for(i=0;i<PicNum;i++)
	{
		fread(&DataLen, 4, 1, RLEstream);
		fseek(RLEstream, DataLen + 4, SEEK_CUR);
	}
	
	fread(&DataLen, 4, 1, RLEstream);			// READ PCX HEADER DATA
	fread(&Width  , 2, 1, RLEstream);
	fread(&Height , 2, 1, RLEstream);
	
	BufPicture.Width = Width + 1;				// SAVE WIDTH AND HEIGHT
	BufPicture.Height = Height + 1;
	
	if(BufPicture.Resize != 1)
	{
		if((BufPicture.Buffer = (BUFFER)_fmalloc(BufPicture.Width * BufPicture.Height)) == NULL)
			Exit_Program("Not enough memory!!!");
		BufPicture.Resize = 1;
	}
	else
		if((BufPicture.Buffer = (BUFFER)_frealloc(BufPicture.Buffer, BufPicture.Width * BufPicture.Height)) == NULL)
			Exit_Program("Not enough memory!!!");	
	
	while(Drawing)								// BEGIN EXTRACTING PCX 
	{
		Char = getc(RLEstream);
		if((Char & 0xC0) == 0xC0)				//IF Char >= 192
		{
			Run = Char & 0x3F;					//Run = Char - 192
			Char = getc(RLEstream);
		}
		else
			Run = 1;
						
		do
		{
			BufPicture.Buffer[bufpos++] = Char;
												
			if(++CurrX > Width)
			{
				CurrX = 0;
				CurrY++;
			}
			if(CurrY > Height)
				Drawing = 0;
		} while(Drawing && --Run > 0);
	} 
	
	fclose(RLEstream);
}  


void RLEDAT_GetPalette(char FileName[], PALETTE &Palette)
{   
	FILE *RLEstream;
	int i;

	if ((RLEstream = fopen(FileName, "rb")) == NULL)
		Exit_Program("Failed to open RLEDAT file!"); 
		
	fseek(RLEstream, 8, SEEK_SET);
		
	for(i=0;i<256;i++)							// GET PALETTE
	{
		Palette.Red[i]   = getc(RLEstream);
		Palette.Green[i] = getc(RLEstream);
		Palette.Blue[i]	 = getc(RLEstream);
	}                    
	
	fclose(RLEstream);
}