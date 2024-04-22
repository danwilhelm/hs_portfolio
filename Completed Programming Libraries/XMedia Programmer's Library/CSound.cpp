/************************************************************************
	CSound.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

void CSound::Release(void)
{
	lpdsb->Release();
}

HRESULT CSound::Create(CXMedia *xmediain)
{
	//track = new xmGENERICSOUND[maxtracks];
	//ZeroMemory(track, sizeof(xmGENERICSOUND)*maxtracks);

	xmedia		= xmediain;
	lpds		= xmedia->lpds;
	//maxtrack	= maxtracks;
	
	return XMSUCCESS;
}

HRESULT CSound::Load(CXMedia *xmediain, char *filename)
{
	xmedia = xmediain;
	lpds   = xmedia->lpds;
	
	return LoadWAV(filename);
}


HRESULT CSound::LoadWAV(char *filename)
{
	HMMIO		 hwav;			// handle to the wave file
	MMCKINFO	 parent, child;	// parent + child chunks
	WAVEFORMATEX wfmtx;			// wave format struct

	UCHAR	*sndbuffer = NULL;					  // temp sound buffer
	UCHAR	*audioptr1 = NULL, *audioptr2 = NULL; // to 1st/2nd write buffer
	DWORD	audiolen1 = 0, audiolen2 = 0;		  // len of 1st/2nd write buffer

	// Initialize chunk info struct
	parent.ckid			= (FOURCC)0;
	parent.cksize		= 0;
	parent.dwDataOffset	= 0;
	parent.dwFlags		= 0;
	parent.fccType		= (FOURCC)0;

	// copy the parent chunk info to the child
	child = parent;

	// open the WAV file
	if(!(hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF)))
		return XMNOFILE;

	// now for the RIFF
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if(mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
		goto OPENERROR;

	// now for the WAVEfmt
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if(mmioDescend(hwav, &child, &parent, 0))
		goto OPENERROR;
	
	// read the info from the file
	if(mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx))
		goto OPENERROR;

	// the format must be PCM
	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM)
		goto OPENERROR;

	// ascend a level to access the data chunk
	if(mmioAscend(hwav, &child, 0))
		goto OPENERROR;

	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	// now for the data chunk
	if(mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
		goto OPENERROR;

	// allocate memory and read in the data
	sndbuffer = (UCHAR *)new UCHAR[child.cksize];
	mmioRead(hwav, (char *)sndbuffer, child.cksize);
	
	// ... and finally close the file
	mmioClose(hwav, 0);

	if(this->CreateBuffer(child.cksize))
		return XMGENERIC;

	//Lock the sound buffer, copy data into it, then unlock it
	if(lpdsb->Lock(0, child.cksize, (void **) &audioptr1, &audiolen1, 
			  (void **) &audioptr2, &audiolen2, DSBLOCK_FROMWRITECURSOR) != DS_OK)
		return XMGENERIC;

	CopyMemory(audioptr1, sndbuffer, audiolen1);
	CopyMemory(audioptr2, (sndbuffer + audiolen1), audiolen2);

	if(lpdsb->Unlock(audioptr1, audiolen1, audioptr2, audiolen2) != DS_OK)
		return XMGENERIC;

	free(sndbuffer);
	
	return XMSUCCESS;

OPENERROR:
	mmioClose(hwav, 0);
	return XMGENERIC;
}



HRESULT CSound::CreateBuffer(DWORD bufferbytes, DWORD samplerate, DWORD bitspersample, DWORD channels)
{
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wfex;

	// initialize the wave format description
	ZeroMemory(&wfex, sizeof(wfex));
	wfex.cbSize			 = 0;
	wfex.nChannels		 = channels;
	wfex.nSamplesPerSec	 = samplerate;
	wfex.wBitsPerSample	 = bitspersample;
	wfex.wFormatTag		 = WAVE_FORMAT_PCM;
	
	// blockalign (on PCM's only) is # channels * bits/sample divided by 8
	wfex.nBlockAlign	 = (channels * bitspersample) >> 3;
	// the avg bytes/sec is the samplerate * the blockalignment
	wfex.nAvgBytesPerSec = samplerate * wfex.nBlockAlign;

	
	// now to initialize the sound buffer description
	INIT_DDOBJ(dsbd);
	dsbd.dwFlags	   = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = bufferbytes;
	dsbd.lpwfxFormat   = &wfex;
	dsbd.dwReserved	   = 0;

	// and, finally, to create the buffer!
	if(FAILED(xmedia->lpds->CreateSoundBuffer(&dsbd, &lpdsb, NULL)))
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CSound::Play(DWORD loop)
{
	if(loop) loop = DSBPLAY_LOOPING;
	
	if (FAILED(lpdsb->SetCurrentPosition(0)))
		return XMGENERIC;

	if(FAILED(lpdsb->Play(0, 0, loop)))
		return XMGENERIC;
	
	return XMSUCCESS;
}


HRESULT CSound::Stop(void)
{
	if(FAILED(lpdsb->Stop()))
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CSound::Volume(LONG level)
{
	level = (LONG)(-30*(100 - level));		// to convert 0-100 to a decibel scale
	
	if(FAILED(lpdsb->SetVolume(level)))
		return XMGENERIC;

	return XMSUCCESS;
}



HRESULT CSound::Frequency(DWORD freq)
{	
	if(FAILED(lpdsb->SetFrequency(freq)))
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CSound::Pan(LONG panval)
{
	panval = (LONG)(30*(100 - panval));		// to convert 0-100 to a decibel scale

	if(FAILED(lpdsb->SetPan(panval)))
		return XMGENERIC;

	return XMSUCCESS;
}
