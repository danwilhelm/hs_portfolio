; VESA.ASM -- VESA Library ... ASM functions
; by Daniel Wilhelm
; WORKS :) -- VESA 1.0 SUPPORT NEEDED
; MUST LINK WITH VESA.C!


[BITS 32]

[EXTERN _VESA_XRes]          ; externs from VESA_Init
[EXTERN _VESA_YRes]
[EXTERN _VidBuf]
[EXTERN _VESA_PMPgFn]
[EXTERN _VESA_SetPalFn]
[EXTERN _vidm]
[EXTERN _vidbr]
[EXTERN _vidpg]
[EXTERN _vidx]
[EXTERN _vidoff]
[EXTERN _flagtable]

[GLOBAL _VESAA_VertLine]          ; accessible to VESA.C
[GLOBAL _VESAA_HorizLine]
[GLOBAL _VESAA_SetAll]
[GLOBAL _VESA_SetPalette]
[GLOBAL _VESAA_FillPage]
[GLOBAL _VESAA_SetPixel]
[GLOBAL _VESAA_FillY]
[GLOBAL _VESAA_ClearScreen]
[GLOBAL _VESAA_SetPage]
[GLOBAL _KbdHandler]

[SECTION .data]
lastpg   DB   255

[SECTION .text]

%imacro SetPage 0            ; quick for inline-asm calls here
    xor  EBX,EBX
    call [_VESA_PMPgFn]
%endmacro

_KbdHandler:                 ; to handle the keyboard interrupt
    cli
    push CX
    push BX
    push AX

WaitForIt:                   ; waits until keyboard action complete
    in   AL,64h
    test AL,02h
    loopnz    WaitForIt
    in   AL,60h

    xor  AH,AH               ; now get key state ...
    shl  AX,1
    shr  AL,1
    xor  AH,1
    xor  EBX,EBX
    mov  BL,AL               ; get the important part -- NOT the shift states
    mov  [_flagtable + EBX],AH    ; ... and store in the global flagtable

    in   AL,61h                   ; tell the port to resume its duties
    mov  AH,AL                    ; and to get rid of the last data
    or   AL,80h
    out  61h,AL
    xchg AH,AL
    out  61h,AL

    mov  AL,20h                   ; final cleanup
    out  20h,AL
    pop  AX
    pop  BX
    pop  CX
    sti
    iret

Palette_SetAll     EQU  8
StartIndex_SetAll  EQU  12
EndIndex_SetAll    EQU  16
_VESAA_SetAll:                              ; sets the palette from an array
    push EBP
    mov  EBP,ESP

    mov  ESI,[EBP + Palette_SetAll]          ; ESI = pal array start

    movzx EDI,BYTE [EBP + StartIndex_SetAll] ; EDI = StartIndex
    movzx ECX,BYTE [EBP + EndIndex_SetAll]   ; ECX = EndIndex
    sub  ECX,EDI                             ; ECX = Num of Indexes to change
    mov  EBX,EDI                             ; EBX = StartIndex

    shl  EDI,1                               ; to calc start in pal struct
    add  EDI,EBX                             ; EDI *= 3;
    add  ESI,EDI

.SetIndex:
    mov	 DX,03C6h
    mov	 AL,0FFh
    out	 DX,AL
    mov	 DX,03C8h
    mov	 AL,BL                              ; index = startindex + num index
    add  AL,CL
    out	 DX,AL
		
    mov	 DX,03C9h		
    mov	 AL,[ES:ESI]
    out	 DX,AL
    mov	 AL,[ES:ESI + 1]
    out	 DX,AL
    mov	 AL,[ES:ESI + 2]
    out	 DX,AL

    add  ESI,3                               ; leap to the next set in struct
    loop .SetIndex

    pop  EBP
    ret

Index_SetPalette   EQU  8
Palette_SetPalette EQU  12
;Red_SetPalette     EQU  12            ; old method (slow, inefficient)
;Green_SetPalette   EQU  16
;Blue_SetPalette    EQU  20
_VESA_SetPalette:                      ; sets one palette entry
    push EBP
    mov  EBP,ESP

    mov	 DX,03C6h
    mov	 AL,0FFh
    out	 DX,AL                         ; tell the port to ready itself
    mov	 DX,03C8h
    mov	 AL,[EBP + Index_SetPalette]        ; send the index number
    out	 DX,AL
		
    mov	 DX,03C9h		
    ;mov	 AL,[EBP + Red_SetPalette]
    mov  AL,[EBP + Palette_SetPalette]      ; send the red value
    out	 DX,AL
    ;mov	 AL,[EBP + Green_SetPalette]
    mov  AL,[EBP + Palette_SetPalette + 1]  ; send the green value
    out	 DX,AL
    ;mov	 AL,[EBP + Blue_SetPalette]
    mov  AL,[EBP + Palette_SetPalette + 2]  ; send the blue value
    out	 DX,AL

    pop  EBP
    ret

Page_FillPage      EQU  8
Color_FillPage     EQU  12
_VESAA_FillPage:                  ; fill a video page (not useful, but fun :)
    push EBP
    mov  EBP,ESP

    mov  AL,[EBP + Color_FillPage]        ; Get the color
    mov  AH,AL
    shl  EAX,16
    mov  AL,[EBP + Color_FillPage]
    mov  AH,AL

    mov  EDI,[_VidBuf]                 ; start at first page
    mov  DL,[EBP + Page_FillPage]

    SetPage
    mov  ECX,65536/4                   
    rep  stosd                         ; blast the vid buffer

    pop  EBP
    ret


; ***************************************************
; void VESA_VertLine (short startx, short starty, short endy, char color);
;
; ***************************************************
X_VertLine   EQU  8
StartY_VertLine   EQU  12
EndY_VertLine     EQU  16
Color_VertLine    EQU  20
_VESAA_VertLine:
    push EBP
    mov  EBP,ESP

    movzx ESI,WORD [EBP + StartY_VertLine]  ; ESI = StartY
    movzx ECX,WORD [EBP + EndY_VertLine]    ; ECX = EndY
    sub  ECX,ESI                            ; ECX = EndY - StartY
    mov  AL,[EBP + Color_VertLine]          ; AL = Color

.NextRow:
    movzx EBX,BYTE [_vidbr + ESI]           ; EBX = break number
    mov DL,[_vidpg + ESI]                   ; DL = page number

    shl  EBX,2
    sub  EBX,0
    jz   .RightBreak

    cmp  EBX,[_vidx + EBX]
    jz   .RightBreak

.LeftBreak:
    ;push ECX
    ;push ESI
    SetPage
    ;pop  ESI
    ;pop  ECX
    shl  ESI,2                              ; to account for word tables
    movzx EDI,WORD [_vidoff + ESI]          ; EDI = offset of StartY
    add  EDI,[EBP + X_VertLine]
    stosb

    shr  ESI,2
    inc  ESI
    loop .NextRow
    jmp  .End
    

.RightBreak:
    ;inc  DL
    ;SetPage

    ;movzx  EDX,WORD [EBP + X_VertLine]
    ;sub  EDX,[_vidx + EBX]
    ;movzx EDI,WORD [_vidoff]
    ;add  EDI,EDX
    ;stosb

    inc  ESI
    loop .NextRow

.End:
    pop  EBP
    ret


; ***************************************************
; void VESA_HorizLine (short startx, short starty, short endx, char color);
; prototype only
; ***************************************************
StartX_HorizLine   EQU  8
StartY_HorizLine   EQU  12
EndX_HorizLine     EQU  16
Color_HorizLine    EQU  20
_VESA_HorizLine:
    push EBP
    mov  EBP,ESP

    pop  EBP
    ret


; ***************************************************
; void VESA_SetPixel (short X, short Y, char Color);
; WORKS PERFECTLY!!!
; ***************************************************
X_SetPixel     EQU  8
Y_SetPixel     EQU  12
Color_SetPixel EQU  16
_VESAA_SetPixel:
    push EBP
    mov  EBP,ESP

    movzx ESI,WORD [EBP + Y_SetPixel]    ; ESI = y coor of pixel
    movzx EBX,BYTE [_vidbr + ESI]        ; EBX = break number
    movzx ECX,WORD [EBP + X_SetPixel]    ; CX = x coor of pixel
    mov DL,[_vidpg + ESI]                ; DL = page number
    
    sub  BL,0
    jz   .NoBreak                 ; if there isn't a break...

.Break:
    shl  BX,1                     ; _vidx is a word table
    movzx EAX,WORD [_vidx + EBX]  ; AX = x coor of break
    sub  EAX,ECX
    jg   .NoBreak                 ; if the pixel is before the break...
    inc  DL
    SetPage

    mov  EDI,[_VidBuf]            ; start at the beginning of the page
    sub  EDI,EAX
    jmp  .Exit

.NoBreak:
    SetPage
    shl  ESI,2                    ; to account for _vidoff pointers
    mov  EDI,[_vidoff + ESI]      ; EDI = vid offset
    add  EDI,ECX                  ; add the offset to the buffer

.Exit:
    mov  AL,[EBP + Color_SetPixel]
    stosb
    pop  EBP
    ret

; ***************************************************
; void VESA_FillY (short Y, char Color);
;
; ***************************************************
Y_FillY     EQU  8
Color_FillY EQU  12
_VESAA_FillY:
    push EBP
    mov  EBP,ESP

    movzx ESI,WORD [EBP + Y_FillY] ; ESI = Y coor
    mov  DL,[_vidpg + ESI]         ; DX = Page number
    SetPage

    movzx ESI,WORD [EBP + Y_FillY] ; ESI = Y coor
    mov  DL,[_vidpg + ESI]         ; DX = Page number

    mov  AL,[EBP + Color_FillY]    ; EAX = color repped
    mov  AH,AL
    shl  EAX,16
    mov  AL,[EBP + Color_FillY]
    mov  AH,AL
    
    movzx EBX,BYTE [_vidbr + ESI] ; EBX = Break number
    shl  ESI,2                    ; _vidoff is a dword pointer table
    mov  EDI,[_vidoff + ESI]      ; EDI = video offset
    sub  BL,0
    jz   .NoBreak                 ; if page break, goto .Break

.Break:
    shl  EBX,1                    ; because the _vidx is a word table
    movzx ECX,WORD [_vidx + EBX]  ; ECX = Length until break
    shr  CX,2                     ; ONLY ON 640x480 - ALL BREAKS ARE /4!
    rep  stosd
    inc  DL                       ; goto the next page
    push EBX
    SetPage
    pop  EBX
    mov  EDI,[_vidoff]            ; start at the beginning of the page
    mov  CX,[_VESA_XRes]          ; Length remaining = XRes - vidx[y]
    mov  DX,[_vidx + EBX]         ; DX = vidx of the break
    sub  CX,DX                    ; ECX = Length after break
    shr  CX,2                     ; ONLY ON 640x480 - ALL BREAKS ARE /4!
    jmp  .Exit

.NoBreak:
    mov  ECX,[_VESA_XRes]         ; ECX = Length of row
    shr  ECX,2

.Exit:
    rep  stosd
    pop  EBP
    ret

; ***************************************************
; void VESA_ClearScreen (char Color);
;
; ***************************************************
Color_ClearScreen   EQU  8
_VESAA_ClearScreen:
    push EBP
    mov  EBP,ESP

    mov  AL,[EBP + Color_ClearScreen]        ; Get the color
    mov  AH,AL
    shl  EAX,16
    mov  AL,[EBP + Color_ClearScreen]
    mov  AH,AL

    mov  EDI,[_VidBuf]                 ; start at first page
    xor  DX,DX

.NextSet:
    SetPage
    mov  ECX,65536/4                   
    rep  stosd                         ; blast the vid buffer
    sub  EDI,65536                     ; reset to the beg of the vid
    inc  DL
    cmp  DL,4                          ; rep 4 times for pgs 0-3
    jnz  .NextSet

    SetPage                            ; there's still some left over...
    mov  ECX,11264                     ; (640 * 480) - 65536 * 4
    rep  stosd

    pop  EBP
    ret

; **********************************************
; void VESA_SetPage (short page);
; Input: DX - requested page
; **********************************************
page_SetPage  EQU  8
_VESAA_SetPage:
    push EBP
    mov  EBP,ESP

    xor  BX,BX
    mov  DX,[EBP + page_SetPage]       ; call the page switch function
    call [_VESA_PMPgFn]

    pop  EBP
    ret















