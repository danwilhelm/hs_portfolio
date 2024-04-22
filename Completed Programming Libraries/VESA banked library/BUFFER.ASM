[BITS 32]

[EXTERN _VESA_PMPgFn]
[EXTERN _VidBuf]

[GLOBAL _SetPixel]
[GLOBAL _CopyToScreen]
[GLOBAL _SetPalette]

%imacro SetPage 0
    xor  EBX,EBX
    call [_VESA_PMPgFn]
%endmacro

[SECTION .text]

SetPixel_Buffer    EQU  8
SetPixel_X         EQU  16
SetPixel_Y         EQU  20
SetPixel_Color     EQU  24
_SetPixel:
    push EBP
    mov  EBP,ESP
    
    mov  ESI,[EBP + SetPixel_Buffer]
    movzx EAX,WORD [EBP + SetPixel_Y]           ; EAX = Y location
    movzx EBX,WORD [EBP + SetPixel_Buffer + 4]  ; EBX = Buffer.width
    movzx ECX,WORD [EBP + SetPixel_X]           ; ECX = X location
    mov DL,[EBP + SetPixel_Color]               ; EDX = Color
    imul EBX                                    ; Buffer = Y*width+X
    add  EAX,ECX
    add  ESI,EAX
    mov  [DS:ESI],DL

    pop  EBP
    ret

; ***************************************************
; void VESA_ClearScreen (char Color);
;
; ***************************************************
CopyToScreen_Buffer   EQU  8
_CopyToScreen:
    push EBP
    mov  EBP,ESP

    mov  ESI,[EBP + CopyToScreen_Buffer]
    mov  EDI,[_VidBuf]                 ; start at first page
    xor  DX,DX

.NextSet:
    SetPage
    mov  ECX,65536/4                   
    rep  movsd                         ; blast the vid buffer
    sub  EDI,65536                     ; reset to the beg of the vid
    inc  DL
    cmp  DL,4                          ; rep 4 times for pgs 0-3
    jnz  .NextSet

    SetPage                            ; there's still some left over...
    mov  ECX,11264                     ; (640 * 480) - 65536 * 4
    rep  movsd

    pop  EBP
    ret


