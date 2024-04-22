/************************************************************************
	CSound.h - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

struct xmGENERICSOUND
{
	LPDSBUFFER  lpdsb;
	int			state;
	int			rate;
	int			size;
	int			id;
};


class CSound
{
	private:
		CXMedia			*xmedia;
		//xmGENERICSOUND	*track;

		LPDSOUND	lpds;		
		//DWORD		maxtrack;

		LPDSBUFFER  lpdsb;
		int			state;
		int			rate;
		int			size;
		int			id;


	public:
		void	Release(void);

		HRESULT Create(CXMedia *xmediain);

		HRESULT Load(CXMedia *xmediain, char *filename);
		HRESULT LoadWAV(char *filename);

		HRESULT CreateBuffer(DWORD bufferbytes, DWORD samplerate = 11025, DWORD bitspersample = 8, DWORD channels = 1);

		HRESULT Play(DWORD loop = FALSE);
		HRESULT Stop(void);
		HRESULT Volume(LONG level);
		HRESULT Frequency(DWORD freq);
		HRESULT Pan(LONG panval);
};
