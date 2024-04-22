/************************************************************************
	CMusic.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/
#include <dmusicf.h>

class XgcAudioClip
{
	private:
		IDirectMusicSegment8		*dmsegment;
		IDirectMusicSegmentState	*dmsegmentstate;

	public:
		XgcAudioClip(void);
		Release(void);

		HRESULT IsPlaying(void);
		HRESULT Stop(void);
		HRESULT Play(DWORD repeats = 0);
		HRESULT Load(char *filename);

		HRESULT SetTempo(double tempo);
		//HRESULT SetVolume(long decibels);

		HRESULT SetMasterTempo(float tempo);
		HRESULT SetMasterVolume(long decibels);
};