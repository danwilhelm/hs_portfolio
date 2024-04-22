//--------------------------------------------------------------------
//Name:		RazorKeys.cpp
//Desc:		Functions for Keyboard operations through DInput
//--------------------------------------------------------------------

#ifndef RAZORKEYS_H_
#define RAZORKEYS_H_

class RAZORKEYS{
public:
	bool Init ();
	int  KeyboardRead ();
	bool Release ();
	bool HandleKeys();
	bool BindKey(int KeyNum, int Value);

	bool Unacquired;
	int  Custom_Keys[256];
	bool KeyStates[256];

	LPDIRECTINPUT			DInput;
	LPDIRECTINPUTDEVICE		DIKeyboard;
};

#endif