
void Sound(int freq, long mslen);

void Sound(int freq, long mslen)
{
	int highms = (int) ((mslen >> 16) & 0xFFFF);
	int lowms  = (int) (mslen & 0xFFFF);
	
	_asm
	{
		in		AL,61h					// save speaker state
		push	AX
		mov		BX,WORD PTR freq 
	
		mov		AL,10110110b			// make the sound
		out		43h,AL
		mov		AX,BX
		out		42h,AL
		mov		AL,AH
		out		42h,AL
		in		AL,61h
		or		AL,00000011b
		out		61h,AL 
		
		mov		CX,WORD PTR highms		// sound it for mslen millisecs
		mov		DX,WORD PTR lowms
		mov		AH,86h
		int		15h
		
		pop		AX						// turn off speaker
		out		61h,AL				
	}
}
		
		
