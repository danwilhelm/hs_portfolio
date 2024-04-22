#include <stdio.h>  
#include <stdlib.h> 
#include <conio.h>
#include <malloc.h>

#include "dkwlib.h"

//			PALETTE ROUTINES

void Set_Palette(unsigned char Red, unsigned char Green, unsigned char Blue, int Index)
{
	_asm
	{
		mov		DX,03C6h
		mov		AL,0FFh
		out		DX,AL
		mov		DX,03C8h
		mov		AL,BYTE PTR Index
		out		DX,AL
		
		mov		DX,03C9h		
		mov		AL,BYTE PTR Red
		out		DX,AL
		mov		AL,BYTE PTR Green
		out		DX,AL
		mov		AL,BYTE PTR Blue
		out		DX,AL
	}
}

void Read_Palette(int Index) 
{ 
	_asm
	{
		mov		DX,03C6h
		mov		AL,0FFh
		out		DX,AL
		mov		DX,03C7h
		mov		AL,BYTE PTR Index
		out		DX,AL
		
		mov		DX,03C9h
		in		AL,DX 
		mov		BYTE PTR Red,AL
		in		AL,DX  
		mov		BYTE PTR Green,AL  
		in		AL,DX
		mov		BYTE PTR Blue,AL  
	} 
}


void Set_Mode(int vmode)
{
	_asm
	{
		xor	AH,AH
		mov	AL,BYTE PTR vmode
		int 10h
	}
}

void Set_Video_Page(int page)
{
	_asm
	{
		mov		AH,05h
		mov		AL,BYTE PTR page
		int		10h 
	}
}


void Wait_Until_VRT(void)
{
	_asm
	{
		mov		DX,03DAh
	End_VRT:
		in		AL,DX
		and		AL,08h       
		loopnz 	End_VRT
	Start_VRT:
		in		AL,DX
		and		AL,08h
		loopz  	Start_VRT
	}
} 

void Exit_Program(char ErrMsg[])
{
 	Set_Mode(TEXT_MODE);
 	printf("%s\n", ErrMsg);
	exit(0);
}  

//					GRAPHICS ROUTINES FOR MODE 13h (320x200x256)

void Clear_Screen(int color)
{
	_asm
	{
		les	DI,current_buffer
		xor	DI,DI
		mov	CX,HALF_NUM_PIXELS
		mov	AL,BYTE PTR color
		mov	AH,AL
		rep	stosw
	}
} 

void Set_Pixel(int X, int Y, int Color)
{
	_asm
	{
		les		DI,current_buffer
		mov		DI,Y
		shl		DI,6							//Multiply Y by 320
		mov		BX,DI                           //
		shl		DI,2                            //
		add     DI,BX                           //
		add     DI,X
		mov		AL,BYTE PTR Color
		mov		ES:[DI],AL
	}
}  

int Read_Pixel(int X, int Y)
{
	int Color;
	_asm
	{
		les		DI,current_buffer
		mov		DI,Y
		shl		DI,6
		mov		BX,DI
		shl		DI,2
		add		DI,BX
		add		DI,X
		mov		AX,ES:[DI]
		mov		Color,AX
	} 
	return Color;
}


void Horiz_Line(int StartX, int Y, int EndX, int Color) 
{
	_asm
	{ 
		les		DI,current_buffer
		mov		DI,Y
		shl		DI,6
		mov		BX,DI
		shl		DI,2
		add	    DI,BX
		add  	DI,StartX
		mov		CX,EndX
		sub		CX,StartX
		mov		AL,BYTE PTR Color
		mov		AH,AL  
		shr		CX,1 
		jc		Add_Pixel
		rep		stosw    
		mov		ES:[DI],AL
		jmp		End
	Add_Pixel: 
		inc		CX
		rep		stosw
	End:
	}
}


void Vert_Line(int X, int StartY, int EndY, int Color)
{
	_asm
	{
	 	les		DI,current_buffer
		mov		DI,StartY
		shl		DI,6
		mov		BX,DI
		shl		DI,2
		add	    DI,BX
		add  	DI,X
		mov		AL,BYTE PTR Color
		mov		CX,EndY
		sub  	CX,StartY
	DrawPixel:
		mov		ES:[DI],AL 
		add  	DI,320
		loop	DrawPixel
	}
}  

//					DOUBLE BUFFER

void Init_Double_Buffer(void)
{ 
	double_buffer = (BUFFER)_fmalloc(64000);
	
	if(double_buffer == NULL)
		Exit_Program("Error: Not enough memory!"); 
}

void Show_Double_Buffer(void)
{
	_asm
	{
		push	DS
		les		DI,video_buffer
		lds		SI,double_buffer
		xor		SI,SI
		mov		CX,32000 
		rep		movsw
		pop		DS
	}
}

void Free_Double_Buffer(void)
{
	_ffree(double_buffer);
}     


void Use_Palette(PALETTE Palette)
{    
	int i;
	for(i=0;i<255;i++)
		Set_Palette(Palette.Red[i], Palette.Green[i], Palette.Blue[i], i);
}
	

// Palette = 1 then it uses the files palette, if anything else, it doesn't
void Get_PCX(char FileName[], BUFPIC &BUFPicture, PALETTE &Palette)
{
	FILE *stream;
	int Width, Height;
	int Char, Run;
	int Drawing = 1;
	int i, bufpos = 0;
	int CurrX = 0, CurrY = 0;
	
	if ((stream = fopen(FileName, "rb")) == NULL)
	{
		Set_Mode(TEXT_MODE);
		printf("***ERROR: opening graphics file %s!\n", FileName);
		exit(0);
	}
		
	fseek(stream, 8, 1);						//GET THE HEIGHT AND WIDTH
	fread(&Width, 2, 1, stream);
	fread(&Height, 2, 1, stream);
	
	BUFPicture.Width = Width + 1;
	BUFPicture.Height = Height + 1;
	
	if((BUFPicture.Buffer = (BUFFER)_fmalloc(BUFPicture.Width * BUFPicture.Height)) == NULL)
		Exit_Program("Not enough memory!!!");
	
	fseek(stream, -768, SEEK_END);				//GET THE PALETTE DATA
					
	for(i=0;i<256;i++)
	{
		Palette.Red[i]   = getc(stream) >> 2;
		Palette.Green[i] = getc(stream) >> 2;
		Palette.Blue[i]	 = getc(stream) >> 2;
	}

	
	fseek(stream, 128, SEEK_SET);				//BEGIN EXTRACTING DATA
	
	while(Drawing)
	{
		Char = getc(stream);
		if((Char & 0xC0) == 0xC0)				//IF Char >= 192
		{
			Run = Char & 0x3F;					//Run = Char - 192
			Char = getc(stream);
		}
		else
			Run = 1;
						
		do
		{
			BUFPicture.Buffer[bufpos++] = Char;
												
			if(++CurrX > Width)
			{
				CurrX = 0;
				CurrY++;
			}
			if(CurrY > Height)
				Drawing = 0;
		} while(Drawing && --Run > 0);
	}
			
	fclose(stream);
}

void Get(int StartX, int StartY, int EndX, int EndY, BUFPIC BUFPicture, BUFPIC &BufChange)
{
	int Width = EndX - StartX; 
	int Height = EndY - StartY;
	
	BufChange.Width = Width;
	BufChange.Height = Height;
			
	_asm
	{
		push	DS
		les		DI,BUFPicture.Buffer
		lds		SI,current_buffer
				
		mov		SI,StartY			// offset = (StartY << 8) + (StartY << 6) + StartX
		shl		SI,6                
		mov		BX,SI               
		shl		SI,2                
		add	    SI,BX               
		add  	SI,StartX           
		
		mov		AX,SCREEN_WIDTH		// AX = SCREEN_WIDTH - Width (for offset)
		sub		AX,Width
		
		mov		BX,Width			// is the width odd or even?
		shr		BX,1
		jc		ExtraByte
			
	NextLine:
		mov		CX,BX				// copy the data
		rep		movsw   
		add		SI,AX
		dec		Height
		jnz		NextLine
		jmp		End
		
	ExtraByte:
		mov		CX,BX
		rep		movsw
		movsb
		add		SI,AX
		dec		Height
		jnz		ExtraByte
		
	End: 
		pop		DS
	}	
} 

void Put(int StartX, int StartY, BUFPIC BUFPicture)
{ 
	int Width = BUFPicture.Width;
	int Height = BUFPicture.Height;
	
	_asm
	{
		push	DS
		les		DI,current_buffer
		lds		SI,BUFPicture.Buffer
				
		mov		DI,StartY			// offset = (StartY << 8) + (StartY << 6) + StartX
		shl		DI,6                
		mov		BX,DI 
		shl		DI,2                
		add	    DI,BX               
		add  	DI,StartX           
		
		mov		AX,SCREEN_WIDTH		// AX = SCREEN_WIDTH - Width (for offset)
		sub		AX,Width
						
		mov		BX,Width			// is the width odd or even?
		shr		BX,1
		jc		ExtraByte
			
	NextLine:
		mov		CX,BX				// copy the data
		rep		movsw   
		add		DI,AX
		dec		Height
		jnz		NextLine
		jmp		End
		
	ExtraByte:
		mov		CX,BX
		rep		movsw
		movsb
		add		DI,AX
		dec		Height
		jnz		ExtraByte
		
	End: 
		pop		DS
	}	
	/*int bufpos = 0, x, y; 
	int offset = (StartY << 8) + (StartY << 6) + StartX;
	
	for(y=0;y<Height;y++)
	{
		for(x=0;x<Width;x++)
			current_buffer[offset+x] = BUFPicture.Buffer[bufpos+x]; 
		
		offset += SCREEN_WIDTH;
		bufpos += Width;
	}*/
}

void Put_Blank(int StartX, int StartY, BUFPIC BUFPicture, int Color)
{ 
	int Width = BUFPicture.Width;
	int Height = BUFPicture.Height;
	
	_asm
	{
		les		DI,current_buffer
						
		mov		DI,StartY			// offset = (StartY << 8) + (StartY << 6) + StartX
		shl		DI,6                
		mov		BX,DI 
		shl		DI,2                
		add	    DI,BX               
		add  	DI,StartX           
		
		mov		DX,SCREEN_WIDTH		// AX = SCREEN_WIDTH - Width (for offset)
		sub		DX,Width
						
		mov		AL,BYTE PTR Color
		mov		AH,AL
		mov		BX,Width			// is the width odd or even?
		shr		BX,1
		jc		ExtraByte
			
	NextLine:
		mov		CX,BX				// copy the data
		rep		stosw   
		add		DI,DX
		dec		Height
		jnz		NextLine
		jmp		End
		
	ExtraByte:
		mov		CX,BX
		rep		stosw
		movsb
		add		DI,DX
		dec		Height
		jnz		ExtraByte
		
	End: 
	}
}		


//			KEYBOARD ROUTINES
int KeyB_Wait(void)
{
    int ScanCode;
	_asm
	{
		xor		AH,AH
		int		16h
		mov		AL,AH
		xor		AH,AH
		mov		ScanCode,AX
	}
	return ScanCode;
}
		

int KeyB_Check(void)
{
	int ScanCode;
	_asm
	{
		mov		AH,01h
		int		16h
		jz		NoCode
		xor		AH,AH
		int		16h
		mov		AL,AH
		xor		AH,AH
		jmp		End
	
	NoCode:
		xor		AX,AX
	End:             
		mov		ScanCode,AX
	}
	return ScanCode;
}


int KeyB_Return_Flags(void) 
{
	int FlagByte;
	_asm
	{
		mov		AH,02h
		int		16h
		xor		AH,AH  
		mov		FlagByte,AX
	}
	return FlagByte;					
}

//			MOUSE ROUTINES

void Mouse_Reset_Driver(void)
{
	_asm
	{
		mov		AH,0000h
		int		33h
	}
}

void Mouse_Show_Pointer(void)
{
	_asm
	{
		mov		AX,0001h
		int		33h
	}
}

void Mouse_Hide_Pointer(void)
{
	_asm
	{
		mov		AX,0002h
		int		33h
	}
}

void Mouse_Status(void)
{
	_asm
	{
		mov		AX,0003h          
		int		33h
		shr		CX,1						
		mov		Mouse_Button_Status,BX
		mov		MouseX,CX
		mov		MouseY,DX
	}
}

void Mouse_Wait(void)
{
	Mouse_Button_Status = 0;
	while(Mouse_Button_Status != 1)
		Mouse_Status();     
}

void Mouse_Movement(void)
{
	_asm
	{
		mov		AX,000Bh
		int		33h
		mov		Mouse_MickeyX,CX
		mov		Mouse_MickeyY,DX
	}
}
 
void Mouse_Set_Sensitivity(int X, int Y, int DSV)
{
	_asm
	{
		mov		AX,001Ah
		mov		BX,WORD PTR X
		mov		CX,WORD PTR Y
		mov		DX,WORD PTR DSV
		int		33h
		mov		Mouse_SensX,BX
		mov		Mouse_SensY,CX
		mov		Mouse_DoubSpeed,DX
	}
}

void Mouse_Set_Boundaries(int MinX, int MinY, int MaxX, int MaxY)
{
	_asm
	{
		mov		AX,07h				// Set X boundaries
		mov		CX,MinX
		mov		DX,MaxX
		shl		CX,1				// Multiply by 2 for 320x200
		shl		DX,1
		int		33h
		
		mov		AX,08h				// Set Y boundaries
		mov		CX,MinY
		mov		DX,MaxY
		int		33h
	}
}

void Mouse_Set_Cursor(unsigned int mask[], int Top_HotSpot, int Left_HotSpot)
{
	_asm
	{
		push	DS
		mov		DX,mask             //pointer ES:DX
		mov		BX,DS
		mov		ES,BX
		
		mov		AX,0009h
		mov		BX,Top_HotSpot		//hot spot from top
		mov		CX,Left_HotSpot     //hot spot from left
		int		33h 
		pop		DS
	}
}

