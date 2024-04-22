#include <stdio.h> 
#include <stdlib.h>
#include <iostream.h>
#include <string.h>

struct PALETTE
{
	char Red[255];
	char Green[255];
	char Blue[255];
};

char Files[35][15] = { "CGCTLOGO.PCX", "SPMENU.PCX"  , "CREDITS.PCX",
					   "STILLBCK.PCX", "STILLP1.PCX" , "STILLP2.PCX",
					   "STILLP3.PCX" , "STILLP4.PCX" , "STILLP5.PCX",
					   "STILLP6.PCX" , "STILLP7.PCX" , "STILLP8.PCX",
					   "LAS0.PCX"    , "LAS2.PCX"    , "LAS3.PCX",   
                       "LAS4.PCX"    , "LAS5.PCX"	 , "LAS6.PCX",
                       "LAS7.PCX"    , "LAS8.PCX"    , "SPWSHLD0.PCX",
                       "SPWSHLD1.PCX", "NEWROCK1.PCX", "ROCK2.PCX",
                       "ROCK3.PCX"   , "ROCK4.PCX"   , "ROCK5.PCX",
                       "EXPL0.PCX"   , "EXPL1.PCX"   , "EXPL2.PCX",
                       "EXPL3.PCX"   , "EXPL4.PCX"   , "HSCORES.PCX",
                       "NAME.PCX"  };                   
                       
void main(int argc, char *argv[])
{
	int chr, Width, Height;
	long i, j, DataLen;
	
	FILE *oldstream, *newstream;
	static PALETTE PixPal;
	
	int NumImages = 34; //NumImages = argc - 2;
	
	if((newstream = fopen("SPACER.DAT", "w")) == NULL)   
	{
		printf("Failed to create new file!\n");
		exit(0);
	}
	
	if((oldstream = fopen(Files[0], "rb")) == NULL)
	{
		printf("Failed to open gfx file!\n");
		exit(0);
	}
	
	printf("Getting palette...\n");
	
	fseek(oldstream, -768, SEEK_END);				//GET THE PALETTE DATA
	
	for(i=0;i<256;i++)
	{
		PixPal.Red[i]   = getc(oldstream) >> 2;
		PixPal.Green[i] = getc(oldstream) >> 2;
		PixPal.Blue[i]	= getc(oldstream) >> 2;
	}
	
	printf("Writing header...\n\n");
	
	fputs("RLEDAT", newstream);						// MAKE HEADER FOR NEW FILE
	fwrite(&NumImages, 2, 1, newstream);
	
	for(i=0;i<256;i++)								// WRITE PALETTE DATA
	{
		fputc(PixPal.Red[i]  , newstream);
		fputc(PixPal.Green[i], newstream);
		fputc(PixPal.Blue[i] , newstream);
	} 
	
	printf("Packing images...\n");
	for(i=0;i<NumImages;i++)
	{
		if((oldstream = fopen(Files[i], "rb")) == NULL)	// OPEN THE NEW PCX
		{
			printf("Failed to open gfx file!");
			exit(0);
		}
		
		printf("%s\t%l\n", Files[i], DataLen);
		//cout << Files[i] << '\t' << DataLen << '\n';
		
		fseek(oldstream, 8, SEEK_SET);				// GET THE HEIGHT AND WIDTH
		fread(&Width, 2, 1, oldstream);
		fread(&Height, 2, 1, oldstream);
		
		fseek(oldstream, 0, SEEK_END);				// GET LENGTH OF PCX
		DataLen = ftell(oldstream) - 896;			// MINUS THE PALETTE AND HEADER
		                                            
		//fwrite(&argv[i], 8, 1, newstream);			// WRITE PCX HEADER DATA
		fwrite(&DataLen, 4, 1, newstream);
		fwrite(&Width  , 2, 1, newstream);
		fwrite(&Height , 2, 1, newstream);	
		
		fseek(oldstream, 128, SEEK_SET);			// WRITE THE PCX DATA
		for(j=0;j<DataLen;j++)
		{
			chr = fgetc(oldstream);	
			fputc(chr, newstream);
		}
		
		fclose(oldstream);							// CLOSE THE FILES
	}	
	
	fclose(newstream);
}

