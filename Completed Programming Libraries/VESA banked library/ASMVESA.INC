[EXTERN _VESAInfo]
[EXTERN _VESAModeInfo]

[EXTERN _vidmem]
[EXTERN _vidbr]
[EXTERN _vidpg]
[EXTERN _vidx]
[EXTERN _vidoff]

[EXTERN _VidBuf]

PgFn     EQU  0Ch

;[SECTION .bss]

;vidmem   resd 480
;vidbr    resb 480
;vidpg    resb 480
;vidx     resw 15
;vidoff   resw 480

[SECTION .data]

;VidBuf   dw   0000h

;_VESAInfo:
;    Sig      times 4 db  00
;    Ver      dw  0000
;    OEMName  dd  00000000
;    Capabil  times 4 db  00
;    VidMode  dd  00000000
;    VidMem   dw  0000

; VBE 2.0 only
;    OEMVer   dw  0000
;    VendNam  dd  00000000
;    ProdNam  dd  00000000
;    ProdRev  dd  00000000
;    Dummy    times 256 db  00


;_VESAModeInfo:
;    ModeAttr  dw 0000
;    WinAAttr  db 00
;    WinBAttr  db 00
;    Granular  dw 0000
;    WinSize   dw 0000
;    WinASeg   dw 0000
;    WinBSeg   dw 0000
;    PgSwtch   dd 00000000     ; 0Ch
;    BytsLin   dw 0000

; The following are optional - not all cards support
;    XRes      dw 0000
;    YRes      dw 0000
;    XCharSiz  db 00
;    YCharSiz  db 00
;    NumPlane  db 00
;    BitsPixl  db 00
;    NumBanks  db 00
;    MemModel  db 00
;    BankSize  db 00
;    NumImgPg  db 00
;    Reserved  db 00

; The following is VBE v1.2+ only
;    RdMaskSiz db 00
;    RdMaskPos db 00
;    GrMaskSiz db 00
;    GrMaskPos db 00
;    BlMaskSiz db 00
;    BlMaskPos db 00
;    RsMaskSiz db 00
;    RsMaskPos db 00
;    DrScrInfo db 00

; The following is VBE v2.0+ only
;    LinVidBuf dd 00000000
;    OffScrMem dd 00000000
;    OffMemKB  dw 0000
;    MDummy    times 206 db 00
