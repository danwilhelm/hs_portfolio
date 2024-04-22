#define MAX_KEYS			256
#define MAX_MOUSEBUTTONS	8
#define MAX_JOYBUTTONS		128
#define MAX_KEYBOARDMOUSE	MAX_MOUSEBUTTONS + MAX_KEYS
#define MAX_BUTTONS			MAX_KEYS + MAX_MOUSEBUTTONS + MAX_JOYBUTTONS
#define MAX_MOUSEAXIS		3
#define MAX_JOYAXIS			36

struct UNIVSTATE
{
	UCHAR	*button[MAX_BUTTONS];
	LONG	*axis[39];
	
	LONG	xaxis;
	LONG	yaxis;
	LONG	zaxis;
};


struct RAWSTATE
{
	UCHAR			keyboard[MAX_KEYS];		
	DIMOUSESTATE2	mouse;
	DIJOYSTATE2		joystick;
};

UNIVSTATE univstate;
RAWSTATE  rawstate;  


struct KEYMAP
{
	UCHAR *action[300];
	LONG  *move[30];
};

#define MOUSE_LEFT		MAX_KEYS
#define	MOUSE_RIGHT		MAX_KEYS + 1
#define MOUSE_CENTER	MAX_KEYS + 2
#define MOUSE_BUTTON(n)	MAX_KEYS + n

#define JOY_FIRE		MAX_KEYBOARDMOUSE
#define JOY_TOP			MAX_KEYBOARDMOUSE + 1
#define JOY_BUTTON(n)	MAX_KEYBOARDMOUSE + n

#define SetButtonUp(btn)   (*univstate.button[btn] = 0)
#define SetButtonDown(btn) (*univstate.button[btn] = 0x80)

#define ButtonDown(btn)	(*univstate.button[btn] & 0x80)
#define GetAxis(axs)	(*univstate.axis[axs])

#define GetXAxis		(univstate.xaxis)
#define GetYAxis		(univstate.yaxis)
#define GetZAxis		(univstate.zaxis)

#define AXIS_MOUSEX			0
#define AXIS_MOUSEY			1
#define AXIS_MOUSEZ			2
#define AXIS_JOYX			3
#define AXIS_JOYY			4
#define AXIS_JOYZ			5
#define AXIS_JOYROTX		6
#define AXIS_JOYROTY		7
#define AXIS_JOYROTZ		8
#define AXIS_SLIDER0		9
#define AXIS_SLIDER1		10
#define AXIS_POV0			11
#define AXIS_POV1			12
#define AXIS_POV2			13
#define AXIS_POV3			14
#define AXIS_JOYXVEL		15
#define AXIS_JOYYVEL		16
#define AXIS_JOYZVEL		17
#define AXIS_JOYXANGVEL		18
#define AXIS_JOYYANGVEL		19
#define AXIS_JOYZANGVEL		20
#define AXIS_SLIDERVEL0		21
#define AXIS_SLIDERVEL1		22
#define AXIS_JOYXACC		23
#define AXIS_JOYYACC		24
#define AXIS_JOYZACC		25
#define AXIS_JOYXANGACC		26
#define AXIS_JOYYANGACC		27
#define AXIS_JOYZANGACC		28
#define AXIS_SLIDERACC0		29
#define AXIS_SLIDERACC1		30
#define AXIS_JOYXFORCE		31
#define AXIS_JOYYFORCE		32
#define AXIS_JOYZFORCE		33
#define AXIS_JOYXTORQUE		34
#define AXIS_JOYYTORQUE		35
#define AXIS_JOYZTORQUE		36
#define AXIS_SLIDERFORCE0	37
#define AXIS_SLIDERFORCE1	38