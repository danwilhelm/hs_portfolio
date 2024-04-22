/************************************************************************
	CMusic.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/


struct xmMUSIC_MIDI
{
	DMSEGMENT	*dmseg;
	DMSEGSTATE	*dmsegstate;
	int			id;
	int			state;
};

#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, y,-1,x,_MAX_PATH);

// midi object state defines
#define MIDI_NULL     0   // this midi object is not loaded
#define MIDI_LOADED   1   // this midi object is loaded
#define MIDI_PLAYING  2   // this midi object is loaded and playing
#define MIDI_STOPPED  3   // this midi object is loaded, but stopped

class CMusic
{
	private:
		CXMedia		 *xmedia;
		DMPERF		 *lpdmp;

		DMLOADER	 *lpdml;
		//xmMUSIC_MIDI *track;

		//DWORD		 maxtrack;

		DMSEGMENT	*dmseg;
		DMSEGSTATE	*dmsegstate;
		int			id;
		int			state;

	public:
		CMusic(void);
		Release(void);

		HRESULT ChangeTempo(DWORD speed);
		HRESULT IsPlaying(void);
		HRESULT Stop(void);
		HRESULT Play(void);
		HRESULT Create(CXMedia *xmediain);
		HRESULT Load(CXMedia *xmediain, char *filename);
};