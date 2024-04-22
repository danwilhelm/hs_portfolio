
#define TEXT_MODE				0x03
#define VGA256					0x13
#define SCREEN_WIDTH			320
#define SCREEN_HEIGHT			200  
#define NUM_PIXELS				64000 
#define HALF_NUM_PIXELS			32000
#define USEPALETTE				1
#define NOPALETTE				0
#define BLACK					0

//		SCAN CODES

#define SC_ESCAPE			1
#define SC_1				2
#define SC_2				3
#define SC_3				4	
#define SC_4				5
#define SC_5				6
#define SC_6				7
#define SC_7				8
#define SC_8				9
#define SC_9				10
#define SC_0				11
#define SC_MINUS			12
#define SC_EQUALS			13
#define SC_BACKSPACE		14 
#define SC_TAB				15
#define SC_Q				16
#define SC_W				17
#define SC_E				18
#define SC_R				19
#define SC_T				20
#define SC_Y				21
#define SC_U				22
#define SC_I				23
#define SC_O				24
#define SC_P				25
#define SC_LEFT_BRACKET		26
#define SC_RIGHT_BRACKET	27
#define SC_ENTER			28
#define SC_CONTROL			29
#define SC_A				30
#define SC_S				31
#define SC_D				32
#define SC_F				33
#define SC_G				34
#define SC_H				35
#define SC_J				36
#define SC_K				37
#define SC_L				38
#define SC_SEMICOLON		39
#define SC_APOSTROPHE		40
#define SC_LEFT_SHIFT		42
#define SC_BACK_SLASH		43
#define SC_Z				44
#define SC_X				45
#define SC_C				46
#define SC_V				47
#define SC_B				48
#define SC_N				49
#define SC_M				50
#define SC_COMMA			51
#define SC_PERIOD			52
#define SC_FORWARD_SLASH	53
#define SC_RIGHT_SHIFT		54
#define SC_PRINT_SCREEN		55
#define SC_ALT				56
#define SC_SPACE			57
#define SC_CAPS_LOCK		58
#define SC_F1				59
#define SC_F2				60
#define SC_F3				61
#define SC_F4				62
#define SC_F5				63
#define SC_F6				64
#define SC_F7				65
#define SC_F8				66
#define SC_F9				67
#define SC_F10				68
#define SC_F11				133
#define SC_F12				134
#define SC_NUM_LOCK			69
#define SC_SCROLL_LOCK		70
#define SC_HOME				71
#define SC_UP				72
#define SC_PAGE_UP			73
#define SC_NUM_MINUS		74
#define SC_LEFT				75
#define SC_CENTER			76
#define SC_RIGHT			77
#define SC_NUM_PLUS			78
#define SC_END				79
#define SC_DOWN				80
#define SC_PAGE_DOWN		81
#define SC_INSERT			82
#define SC_DELETE			83

//		BITMASKS FOR SHIFT STATE
#define SHIFT_RIGHT			0x0001
#define SHIFT_LEFT			0x0002
#define CONTROL				0x0004
#define ALT					0x0008
#define SCROLL_LOCK_ON		0x0010
#define NUM_LOCK_ON			0x0020
#define CAPS_LOCK_ON		0x0040
#define INSERT_ON			0x0080
#define CONTROL_LEFT		0x0100
#define ALT_LEFT			0x0200
#define CONTROL_RIGHT		0x0400
#define ALT_RIGHT			0x0800
#define SCROLL_LOCK_DOWN	0x1000
#define NUM_LOCK_DOWN		0x2000
#define CAPS_LOCK_DOWN		0x4000
#define SYS_REQ_DOWN		0x8000  

typedef unsigned char far * BUFFER;

unsigned int Mouse_Button_Status, MouseX, MouseY;
unsigned int Mouse_MickeyX, Mouse_MickeyY;
unsigned int Mouse_SensX, Mouse_SensY, Mouse_DoubSpeed;
char Red, Green, Blue;

BUFFER rom_chars 	  = (BUFFER)0xF000FA6EL;
BUFFER video_buffer   = (BUFFER)0xA0000000L;
BUFFER double_buffer  = NULL;
BUFFER current_buffer = video_buffer;

struct BUFPIC
{
	int Width;
	int Height;
	int Resize;
	BUFFER Buffer;
};

struct PALETTE
{
	char Red[255];
	char Green[255];
	char Blue[255];
};
	

//			MODE 13h SPECIFIC GRAPHICS
void Clear_Screen(int color);
void Horiz_Line(int StartX, int Y, int EndX, int Color);
void Vert_Line(int X, int StartY, int EndY, int Color);
void Set_Pixel(int X, int Y, int Color);  
int Read_Pixel(int X, int Y);

//			DOUBLE BUFFER 
void Init_Double_Buffer(void);
void Show_Double_Buffer(void);
void Free_Double_Buffer(void);

void Get_PCX(char FileName[], BUFPIC &BufPicture, PALETTE &Palette);
void Get(int StartX, int StartY, int EndX, int EndY, BUFPIC BufPicture, BUFPIC &BufChange);
void Put(int StartX, int StartY, BUFPIC BufPicture);
void Put_Blank(int StartX, int StartY, BUFPIC BUFPicture, int Color);

//			KEYBOARD ROUTINES
int KeyB_Wait(void);
int KeyB_Check(void);
int KeyB_Return_Flags(void);

//			MOUSE ROUTINES
void Mouse_Reset_Driver(void);
void Mouse_Show_Pointer(void); 
void Mouse_Hide_Pointer(void);
void Mouse_Status(void); 
void Mouse_Wait(void);
void Mouse_Movement(void); 
void Mouse_Set_Sensitivity(int X, int Y, int DSV);  
void Mouse_Set_Boundaries(int MinX, int MinY, int MaxX, int MaxY); 
void Mouse_Set_Cursor(unsigned int mask[], int Top_HotSpot, int Left_HotSpot);

/*--------------------------------------------------------------------
	Sets the current video mode to vmode.
	
	Input: vmode(the wanted video mode
	Output: changes the current video mode
---------------------------------------------------------------------*/
void Set_Mode(int vmode); 

/*-------------------------------------------------------------------
	Sets the active video page.
	
	Input: Page(the wanted page to become active)
	Output: sets the active video page
--------------------------------------------------------------------*/
void Set_Video_Page(int Page); 

/*--------------------------------------------------------------------
	Changes a palette register.
	
	Input: Red, Green, Blue(intensities, each between 0 and 63)
		   Index(the index number in the color look-up table)
	Output: sets an index in the color look-up table to another color
		   	
---------------------------------------------------------------------*/ 
void Set_Palette(unsigned char Red, unsigned char Green, unsigned char Blue, int Index);
void Use_Palette(PALETTE Palette);
 
/*--------------------------------------------------------------------
	Reads a palette register.
	
	Input: Index(the index into the color look-up table)
	Output: sets the global variables Red, Green, and Blue to the
			intensities in an index	
---------------------------------------------------------------------*/
void Read_Palette(int Index); 

/*--------------------------------------------------------------------
	Waits until the vertical retrace.
	
	Input: None
	Output: Waits until the vertical retrace begins (approx. 
			60 times/second).
	
	Notes: Can use this for flicker-free graphics by placing this before
		   any graphics drawing routines. Be warned - after calling this
		   you only have 1/70th of a second to draw(usually plenty of time).
---------------------------------------------------------------------*/
void Wait_Until_VRT(void); 

/*--------------------------------------------------------------------
	Sets the video mode to text mode, displays a message, then exits.
	
	Input: ErrMsg(a character array)
	Output: Sets the video mode to text mode(BIOS 7), displays ErrMsg,
			then completely exits the program.
---------------------------------------------------------------------*/
void Exit_Program(char ErrMsg);





				