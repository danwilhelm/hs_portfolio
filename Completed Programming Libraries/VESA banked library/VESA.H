#define VESA_VER120		0x102
#define VESA_VER200		0x200
#define VESA_VER300		0x300

typedef struct VESAINFO  {
    unsigned char Signature[4]	__attribute__ ((packed));
    unsigned short Ver			__attribute__ ((packed));
    unsigned long OEMNamePtr	__attribute__ ((packed));
    unsigned char Capabil[4]	__attribute__ ((packed));
    unsigned long VidModePtr	__attribute__ ((packed));
    unsigned short VidMem		__attribute__ ((packed));

// VBE 2.0 only
    unsigned short OEMVer		__attribute__ ((packed));
	unsigned long  VendNamPtr	__attribute__ ((packed));
    unsigned long  ProdNamPtr	__attribute__ ((packed));
    unsigned long  ProdRevPtr	__attribute__ ((packed));
    unsigned char Reserved[222]	__attribute__ ((packed));
    unsigned char OEMData[256]	__attribute__ ((packed));
} VESAINFO;

typedef struct VESAMODEINFO  {
    unsigned short ModeAttr		__attribute__ ((packed));
    unsigned char WinAAttr		__attribute__ ((packed));
    unsigned char WinBAttr		__attribute__ ((packed));
    unsigned short Granularity	__attribute__ ((packed));
    unsigned short WinSize		__attribute__ ((packed));
    unsigned short WinASeg		__attribute__ ((packed));
    unsigned short WinBSeg		__attribute__ ((packed));
    unsigned long  PgSwtchPtr   __attribute__ ((packed));
    unsigned short BytsLin		__attribute__ ((packed));

// The following are optional - not all cards support
    unsigned short XRes			__attribute__ ((packed));
    unsigned short YRes			__attribute__ ((packed));
    unsigned char XCharSiz		__attribute__ ((packed));
    unsigned char YCharSiz		__attribute__ ((packed));
    unsigned char NumPlane		__attribute__ ((packed));
    unsigned char BitsPixl		__attribute__ ((packed));
    unsigned char NumBanks		__attribute__ ((packed));
    unsigned char MemModel		__attribute__ ((packed));
    unsigned char BankSize		__attribute__ ((packed));
    unsigned char NumImgPg		__attribute__ ((packed));
    unsigned char Reserved		__attribute__ ((packed));

// The following is VBE v1.2+ only
    unsigned char RdMaskSiz		__attribute__ ((packed));
    unsigned char RdMaskPos		__attribute__ ((packed));
    unsigned char GrMaskSiz		__attribute__ ((packed));
    unsigned char GrMaskPos		__attribute__ ((packed));
    unsigned char BlMaskSiz		__attribute__ ((packed));
    unsigned char BlMaskPos		__attribute__ ((packed));
    unsigned char RsMaskSiz		__attribute__ ((packed));
    unsigned char RsMaskPos		__attribute__ ((packed));
    unsigned char DrScrInfo		__attribute__ ((packed));

// The following is VBE v2.0+ only
    unsigned long PhysBasePtr	__attribute__ ((packed));
    unsigned long OffScrMem		__attribute__ ((packed));
    unsigned short OffMemKB		__attribute__ ((packed));
    char MDummy[206]			__attribute__ ((packed));
} VESAMODEINFO;

typedef struct PMINFO  {
    unsigned short SetWindow    __attribute__ ((packed));
    unsigned short SetDisplay   __attribute__ ((packed));
    unsigned short SetPalette   __attribute__ ((packed));
    unsigned short IOPrivInfo   __attribute__ ((packed));
} PMINFO;

extern void VESAA_SetPixel(short x, short y, char color);
extern void VESAA_FillY(short y, char color);
extern void VESAA_ClearScreen(char color);
extern void VESAA_SetPage(short page);

void VESAC_VertLine (short x, short starty, short endy, char color);
void VESAC_HorizLine(short startx, short y, short endx, char color);
void VESAC_FillY(short y, char color);
void VESAC_ClearScreen(char color);
char *VESA_CalcVid(void);
void VESAC_INTPage(short page);
short VESA_Init(short mode);
short VESA_Initialize(void);
short VESA_SetMode(short mode);
short VESA_GetInfo(void);
short VESA_GetModeInfo(short mode);
short VESA_GetPMInfo(void);

VESAINFO VESAInfo = {"VBE2", 0x200};
VESAMODEINFO VESAModeInfo;
PMINFO *VESAPMInfo;

void *VESA_PMPgFn;
void *VESA_SetPalFn;

char *VidBuf;
int vidmem[768];
char vidbr[768];
char vidpg[768];
short vidx[50];
char *vidoff[768];
short VESA_XRes;
short VESA_YRes;

