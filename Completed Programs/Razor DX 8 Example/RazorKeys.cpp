//--------------------------------------------------------------------
//Name:		RazorKeys.cpp
//Desc:		Functions for Keyboard operations through DInput
//--------------------------------------------------------------------

#include "Main.h"

//-----------------------------------------------------------------------------------------\\
// Initializes the keyboard to take in input
// Only call this once
bool RAZORKEYS::Init()
{
	DIPROPDWORD dipdw;
	HRESULT hr;

	Unacquired	= false;
	DInput		= NULL;
	DIKeyboard	= NULL;

	for(int i = 0; i < 256; i++)
		KeyStates[i] = false;

	if(FAILED(DirectInput8Create(App.hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL)))
		return 0;
	if(FAILED(DInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL)))
		return 0;
	if(FAILED(DIKeyboard->SetCooperativeLevel(App.hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return 0;
	if(FAILED(DIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return 0;

	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = 128; // Arbitary buffer size

	if(FAILED(hr = DIKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		return 0;
	if(FAILED(DIKeyboard->Acquire()))
		return 0;
	SetTimer(App.hWnd, 0, 1000 / 30, NULL);
	return 1;
}
//-----------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------\\
// Reads in the values from the keyboard buffer then calls HandleKeys() to use the input
// Call this in MainLoop()
int RAZORKEYS::KeyboardRead()
{
    DIDEVICEOBJECTDATA	didod[15];  // Receives buffered data 
    DWORD				dwElements;
    DWORD				i;
    HRESULT				hr;
	int					KeyCount = 0;

    if(Unacquired)
	{
		hr = DIKeyboard->Acquire();
		if(hr == DIERR_NOTACQUIRED || DIERR_OTHERAPPHASPRIO)
		{
			MessageBox(App.hWnd, "Alpha Ops Error", "Cannot Capture Keyboard", MB_OK);
			return 2;
		}
	}

	if(NULL == DIKeyboard)
        return 0;
    
    dwElements = 15;
    hr = DIKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
    if(hr != DI_OK) 
    {
        hr = DIKeyboard->Acquire();
        while(hr == DIERR_INPUTLOST)
            hr = DIKeyboard->Acquire();

        // Update the dialog text 
        if(hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED) 
            Unacquired = true;

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return 0; 
    }

    if(FAILED(hr))
        return 0;

    // Study each of the buffer elements and process them.
    //
    // Since we really don't do anything, our "processing"
    // consists merely of squirting the name into our
    // local buffer.
    for(i = 0; i < dwElements; i++) 
    {
        KeyStates[didod[i].dwOfs] = !KeyStates[didod[i].dwOfs];
		if(KeyStates[didod[i].dwOfs])
			KeyCount++;
	}
	if(KeyCount !=0)
		HandleKeys();
	return 1;
}
//-----------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------\\
// Releases the Keyboard and the DirectInput Object
// Called during the shutdown program process
bool RAZORKEYS::Release()
{
	if(DIKeyboard)
	{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
	}
	if(DInput)
		DInput->Release();
	return 1;
}
//-----------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------\\
// Handles what the keys do when pressed
// Called from KeyboardRead
bool RAZORKEYS::HandleKeys()
{
	//Since Gay++ Requires all case statements to be constant
	//We have to use many, many if statements to do the same job
	if(KeyStates[1] == true)//Escape Key
		runnin = false;
	//Example:
	//if(KeyStates[Custom_Keys[Fire_Key]] == true)
	//	  Player.FireGun();
	return 1;
}
//-----------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------\\
// Changes what a the value of a command(KeyNum) to a specified key(Value)
bool RAZORKEYS::BindKey(int KeyNum, int Value)
{
	Custom_Keys[KeyNum] = Value;
	return 1;
}
//-----------------------------------------------------------------------------------------//
