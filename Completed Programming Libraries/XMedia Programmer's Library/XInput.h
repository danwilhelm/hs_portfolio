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


/*#define DI_NUMODFS(n) (sizeof(n) / sizeof(n[0]))

DIOBJECTDATAFORMAT univdiodf_mouse[] =
{
	// the mouse buttons
	{ &GUID_Button, 0, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(0), 0 },
	{ &GUID_Button, 1, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(1), 0 },
	{ &GUID_Button, 2, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(2), 0 },
	{ &GUID_Button, 3, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(3), 0 },
	{ &GUID_Button, 4, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(4), 0 },
	{ &GUID_Button, 5, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(5), 0 },
	{ &GUID_Button, 6, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(6), 0 },
	{ &GUID_Button, 7, DIDFT_BUTTON | DIDFT_MAKEINSTANCE(7), 0 },
	
	// standard mouse axis'
	{ &GUID_XAxis, MAX_JOYBUTTONS + MAX_MOUSEBUTTONS + 0, DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_YAxis, MAX_JOYBUTTONS + MAX_MOUSEBUTTONS + 4, DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_ZAxis, MAX_JOYBUTTONS + MAX_MOUSEBUTTONS + 8, DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 }
};

const DIDATAFORMAT univdidf_mouse = 
	{ sizeof(DIDATAFORMAT),
	  sizeof(DIOBJECTDATAFORMAT),
	  DIDF_RELAXIS, 
	  MAX_BUTTONS - MAX_KEYS + 12,
	  DI_NUMODFS(univdiodf_mouse),
	  univdiodf_mouse };


DIOBJECTDATAFORMAT univdiodf_joystick[] =
{
	// joystick buttons (all 128 of them!)
	{ &GUID_XAxis, 0, 0x80FFFF0C, 0 },
	{ &GUID_YAxis, 1, 0x80FFFF0C, 0 },
	{ &GUID_ZAxis, 2, 0x80FFFF0C, 0 },
	{ &GUID_RxAxis, 3, 0x80FFFF0C, 0 },
	{ &GUID_RyAxis, 4, 0x80FFFF0C, 0 },
	{ &GUID_RzAxis, 5, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 6, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 7, 0x80FFFF0C, 0 },
	{ &GUID_XAxis, 8, 0x80FFFF0C, 0 },
	{ &GUID_YAxis, 9, 0x80FFFF0C, 0 },
	{ &GUID_ZAxis, 10, 0x80FFFF0C, 0 },
	{ &GUID_RxAxis, 11, 0x80FFFF0C, 0 },
	{ &GUID_RyAxis, 12, 0x80FFFF0C, 0 },
	{ &GUID_RzAxis, 13, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 14, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 15, 0x80FFFF0C, 0 },
	{ &GUID_XAxis, 16, 0x80FFFF0C, 0 },
	{ &GUID_YAxis, 17, 0x80FFFF0C, 0 },
	{ &GUID_ZAxis, 18, 0x80FFFF0C, 0 },
	{ &GUID_RxAxis, 19, 0x80FFFF0C, 0 },
	{ &GUID_RyAxis, 20, 0x80FFFF0C, 0 },
	{ &GUID_RzAxis, 21, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 22, 0x80FFFF0C, 0 },
	{ &GUID_Slider, 23, 0x80FFFF0C, 0 },
	{ NULL, 24, 0x80FFFF0C, 0 },
	{ NULL, 25, 0x80FFFF0C, 0 },
	{ NULL, 26, 0x80FFFF0C, 0 },
	{ NULL, 27, 0x80FFFF0C, 0 },
	{ NULL, 28, 0x80FFFF0C, 0 },
	{ NULL, 29, 0x80FFFF0C, 0 },
	{ NULL, 30, 0x80FFFF0C, 0 },
	{ NULL, 31, 0x80FFFF0C, 0 },
	{ NULL, 32, 0x80FFFF0C, 0 },
	{ NULL, 33, 0x80FFFF0C, 0 },
	{ NULL, 34, 0x80FFFF0C, 0 },
	{ NULL, 35, 0x80FFFF0C, 0 },
	{ NULL, 36, 0x80FFFF0C, 0 },
	{ NULL, 37, 0x80FFFF0C, 0 },
	{ NULL, 38, 0x80FFFF0C, 0 },
	{ NULL, 39, 0x80FFFF0C, 0 },
	{ NULL, 40, 0x80FFFF0C, 0 },
	{ NULL, 41, 0x80FFFF0C, 0 },
	{ NULL, 42, 0x80FFFF0C, 0 },
	{ NULL, 43, 0x80FFFF0C, 0 },
	{ NULL, 44, 0x80FFFF0C, 0 },
	{ NULL, 45, 0x80FFFF0C, 0 },
	{ NULL, 46, 0x80FFFF0C, 0 },
	{ NULL, 47, 0x80FFFF0C, 0 },
	{ NULL, 48, 0x80FFFF0C, 0 },
	{ NULL, 49, 0x80FFFF0C, 0 },
	{ NULL, 50, 0x80FFFF0C, 0 },
	{ NULL, 51, 0x80FFFF0C, 0 },
	{ NULL, 52, 0x80FFFF0C, 0 },
	{ NULL, 53, 0x80FFFF0C, 0 },
	{ NULL, 54, 0x80FFFF0C, 0 },
	{ NULL, 55, 0x80FFFF0C, 0 },
	{ NULL, 56, 0x80FFFF0C, 0 },
	{ NULL, 57, 0x80FFFF0C, 0 },
	{ NULL, 58, 0x80FFFF0C, 0 },
	{ NULL, 59, 0x80FFFF0C, 0 },
	{ NULL, 60, 0x80FFFF0C, 0 },
	{ NULL, 61, 0x80FFFF0C, 0 },
	{ NULL, 62, 0x80FFFF0C, 0 },
	{ NULL, 63, 0x80FFFF0C, 0 },
	{ NULL, 64, 0x80FFFF0C, 0 },
	{ NULL, 65, 0x80FFFF0C, 0 },
	{ NULL, 66, 0x80FFFF0C, 0 },
	{ NULL, 67, 0x80FFFF0C, 0 },
	{ NULL, 68, 0x80FFFF0C, 0 },
	{ NULL, 69, 0x80FFFF0C, 0 },
	{ NULL, 70, 0x80FFFF0C, 0 },
	{ NULL, 71, 0x80FFFF0C, 0 },
	{ NULL, 72, 0x80FFFF0C, 0 },
	{ NULL, 73, 0x80FFFF0C, 0 },
	{ NULL, 74, 0x80FFFF0C, 0 },
	{ NULL, 75, 0x80FFFF0C, 0 },
	{ NULL, 76, 0x80FFFF0C, 0 },
	{ NULL, 77, 0x80FFFF0C, 0 },
	{ NULL, 78, 0x80FFFF0C, 0 },
	{ NULL, 79, 0x80FFFF0C, 0 },
	{ NULL, 80, 0x80FFFF0C, 0 },
	{ NULL, 81, 0x80FFFF0C, 0 },
	{ NULL, 82, 0x80FFFF0C, 0 },
	{ NULL, 83, 0x80FFFF0C, 0 },
	{ NULL, 84, 0x80FFFF0C, 0 },
	{ NULL, 85, 0x80FFFF0C, 0 },
	{ NULL, 86, 0x80FFFF0C, 0 },
	{ NULL, 87, 0x80FFFF0C, 0 },
	{ NULL, 88, 0x80FFFF0C, 0 },
	{ NULL, 89, 0x80FFFF0C, 0 },
	{ NULL, 90, 0x80FFFF0C, 0 },
	{ NULL, 91, 0x80FFFF0C, 0 },
	{ NULL, 92, 0x80FFFF0C, 0 },
	{ NULL, 93, 0x80FFFF0C, 0 },
	{ NULL, 94, 0x80FFFF0C, 0 },
	{ NULL, 95, 0x80FFFF0C, 0 },
	{ NULL, 96, 0x80FFFF0C, 0 },
	{ NULL, 97, 0x80FFFF0C, 0 },
	{ NULL, 98, 0x80FFFF0C, 0 },
	{ NULL, 99, 0x80FFFF0C, 0 },
	{ NULL, 100, 0x80FFFF0C, 0 },
	{ NULL, 101, 0x80FFFF0C, 0 },
	{ NULL, 102, 0x80FFFF0C, 0 },
	{ NULL, 103, 0x80FFFF0C, 0 },
	{ NULL, 104, 0x80FFFF0C, 0 },
	{ NULL, 105, 0x80FFFF0C, 0 },
	{ NULL, 106, 0x80FFFF0C, 0 },
	{ NULL, 107, 0x80FFFF0C, 0 },
	{ NULL, 108, 0x80FFFF0C, 0 },
	{ NULL, 109, 0x80FFFF0C, 0 },
	{ NULL, 110, 0x80FFFF0C, 0 },
	{ NULL, 111, 0x80FFFF0C, 0 },
	{ NULL, 112, 0x80FFFF0C, 0 },
	{ NULL, 113, 0x80FFFF0C, 0 },
	{ NULL, 114, 0x80FFFF0C, 0 },
	{ NULL, 115, 0x80FFFF0C, 0 },
	{ NULL, 116, 0x80FFFF0C, 0 },
	{ NULL, 117, 0x80FFFF0C, 0 },
	{ NULL, 118, 0x80FFFF0C, 0 },
	{ NULL, 119, 0x80FFFF0C, 0 },
	{ NULL, 120, 0x80FFFF0C, 0 },
	{ NULL, 121, 0x80FFFF0C, 0 },
	{ NULL, 122, 0x80FFFF0C, 0 },
	{ NULL, 123, 0x80FFFF0C, 0 },
	{ NULL, 124, 0x80FFFF0C, 0 },
	{ NULL, 125, 0x80FFFF0C, 0 },
	{ NULL, 126, 0x80FFFF0C, 0 },
	{ NULL, 127, 0x80FFFF0C, 0 },

	// standard joystick readings
	{ &GUID_XAxis, 128, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_YAxis, 132, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_ZAxis, 136, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_RxAxis, 140, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_RyAxis, 144, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_RzAxis, 148, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_Slider, 152, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_Slider, 156, 0x80FFFF03, DIDOI_ASPECTPOSITION },
	{ &GUID_POV, 160, 0x80FFFF10, 0 },
	{ &GUID_POV, 164, 0x80FFFF10, 0 },
	{ &GUID_POV, 168, 0x80FFFF10, 0 },
	{ &GUID_POV, 172, 0x80FFFF10, 0 },

	// force feedback joystick readings
	{ NULL, 176, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 180, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 184, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 188, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 192, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 196, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 200, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 204, 0x80FFFF03, DIDOI_ASPECTVELOCITY },
	{ NULL, 208, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 212, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 216, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 220, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 224, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 228, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 232, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 236, 0x80FFFF03, DIDOI_ASPECTACCEL },
	{ NULL, 240, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 244, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 248, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 252, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 256, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 260, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 264, 0x80FFFF03, DIDOI_ASPECTFORCE },
	{ NULL, 268, 0x80FFFF03, DIDOI_ASPECTFORCE }
};
const DIDATAFORMAT univdidf_joystick = 
	{ sizeof(DIDATAFORMAT),
	  sizeof(DIOBJECTDATAFORMAT),
	  DIDF_RELAXIS, 
	  sizeof(UNIVSTATE) - MAX_KEYS - MAX_MOUSEBUTTONS,
	  DI_NUMODFS(univdiodf_joystick),
	  univdiodf_joystick };*/
