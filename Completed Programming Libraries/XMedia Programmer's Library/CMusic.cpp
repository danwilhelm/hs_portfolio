/************************************************************************
	CMusic.cpp - by Daniel Wilhelm
	This document is part of the XMedia Programmer's Library (XMPL)
	
	No copying or modifying of this code is allowed.
	This source is (C) 2000 by Daniel Wilhelm
************************************************************************/

HRESULT CMusic::ChangeTempo(DWORD speed)
{
	// Disable tempo track in segment so that it does not reset the tempo.
	lpdmp->SetParam( GUID_DisableTempo, 0xFFFF,0,0, NULL );
 
	DMUS_TEMPO_PMSG *tempo;
 
	if( SUCCEEDED(lpdmp->AllocPMsg(
			sizeof(DMUS_TEMPO_PMSG), (DMUS_PMSG**)&tempo)))
	{
		// Queue the tempo event.
		ZeroMemory(tempo, sizeof(DMUS_TEMPO_PMSG));
		tempo->dwSize = sizeof(DMUS_TEMPO_PMSG);
		tempo->dblTempo = speed;
		tempo->dwFlags = DMUS_PMSGF_REFTIME;
		tempo->dwType = DMUS_PMSGT_TEMPO;
		lpdmp->SendPMsg((DMUS_PMSG*)tempo);
	}

	/*DMUS_TEMPO_PARAM tempo;

	tempo.dblTempo = speed;
	if(FAILED(track[tracknum].dmseg->SetParam(GUID_TempoParam, 0xFFFF, 0, 0, &tempo)))
		return FALSE;*/
	
	return XMSUCCESS;
}


HRESULT CMusic::IsPlaying(void)
{
	if(lpdmp->IsPlaying(dmseg, NULL) == S_FALSE)
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CMusic::Stop(void)
{
	if(FAILED(lpdmp->Stop(dmseg, dmsegstate, 0, 0)))
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CMusic::Play(void)
{
	if(FAILED(lpdmp->PlaySegment(dmseg, 0, 0, &dmsegstate)))
		return XMGENERIC;

	return XMSUCCESS;
}


HRESULT CMusic::Load(CXMedia *xmediain, char *filename)
{
	DMUS_OBJECTDESC	objdesc;
	DMSEGMENT		*lpdms = NULL;

	char	szDir[_MAX_PATH];
	WCHAR	wszDir[_MAX_PATH];
	WCHAR	wfilename[_MAX_PATH];
	
	INIT_DDOBJ(objdesc);

	this->Create(xmediain);

	if(_getcwd(szDir, _MAX_PATH) == NULL)
		return XMGENERIC;

	MULTI_TO_WIDE(wszDir, szDir);
	MULTI_TO_WIDE(wfilename, filename);

	if(FAILED(xmedia->lpdml->SetSearchDirectory(GUID_DirectMusicAllTypes,
										wszDir, FALSE)))
		return XMGENERIC;

	objdesc.guidClass = CLSIDDMSEGMENT;
	wcscpy(objdesc.wszFileName, wfilename);
	objdesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

	if(FAILED(xmedia->lpdml->GetObject(&objdesc, IIDDMSEGMENT, (void **)&lpdms)))
		return XMNOFILE;
	
	if(FAILED(lpdms->SetParam(GUID_StandardMIDIFile, -1, 0, 0, (void *)lpdmp)))
		return XMGENERIC;

	if(FAILED(lpdms->SetParam(GUID_Download, -1, 0, 0, (void *)lpdmp)))
		return XMGENERIC;

	dmseg		= lpdms;
	dmsegstate	= NULL;
	state		= MIDI_LOADED;

	return XMSUCCESS;
}


HRESULT CMusic::Create(CXMedia *xmediain)
{
	//track = new xmMUSIC_MIDI[maxtracks];
	//ZeroMemory(track, sizeof(xmMUSIC_MIDI)*maxtracks);

	lpdmp		= xmediain->lpdmp;
	//maxtrack	= maxtracks;
	
	xmedia = xmediain;
	xmedia->musicloaders++;

	return XMSUCCESS;
}


CMusic::CMusic(void)
{
	lpdmp = NULL;
	//track = NULL;
}


CMusic::Release(void)
{
	DWORD i;

	if(lpdmp)
		lpdmp->Stop(NULL, NULL, 0, 0);
	
	if(dmseg)
	{
		dmseg->SetParam(GUID_Unload, -1, 0, 0, (void *)lpdmp);
		dmseg->Release();
		dmsegstate->Release();
	}
}
