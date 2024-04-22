bool MCI_Open(HWND hwnd, LPSTR filename, MCIDEVICEID *device)
{
	DWORD ret;

	MCI_OPEN_PARMS mciparms;

	mciparms.lpstrDeviceType = "waveaudio";
	mciparms.lpstrElementName = filename;

	ret = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciparms);

	if(ret)  {
		MessageBox(hwnd, "Error playing waveform.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	*device = mciparms.wDeviceID;

	return true;
}

void MCI_Close(HWND hwnd, MCIDEVICEID device)
{
	mciSendCommand(device, MCI_CLOSE, MCI_WAIT, NULL);

	return;
}

bool MCI_SetTime(HWND hwnd, MCIDEVICEID device)
{
	DWORD ret;
	MCI_SET_PARMS mciparms;

	mciparms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
	ret = mciSendCommand(device, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciparms);

	if(ret)	return false;

	return true;
}

bool MCI_Play(HWND hwnd, MCIDEVICEID device, DWORD from)
{
	DWORD ret;
	MCI_PLAY_PARMS mciparms;

	if(!MCI_SetTime(hwnd, device))
		return false;

	mciparms.dwCallback = (DWORD)(LPVOID)hwnd;
	mciparms.dwFrom = from;
	ret = mciSendCommand(device, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&mciparms);

	if(ret)
		return false;

	return true;
}

void MCI_StopPlay(HWND hwnd, MCIDEVICEID device)
{
	mciSendCommand(device, MCI_STOP, MCI_WAIT, NULL);

	return;
}
