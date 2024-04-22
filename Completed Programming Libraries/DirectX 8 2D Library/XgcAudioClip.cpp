#include "XgcAudioClip.h"

HRESULT XgcAudioClip::Load(char *filename)
{
    // Convert to Unicode.

    WCHAR wfilename[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, filename, -1, 
                         wfilename, MAX_PATH );

	// Load the file 

    RETFAIL(xgc->dmloader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,   // Class identifier.
        IID_IDirectMusicSegment8,   // ID of desired interface.
        wfilename,	                // Filename.
        (LPVOID *)&dmsegment		// Pointer that receives interface.
    ));

	return S_OK;
} 


HRESULT XgcAudioClip::Play(DWORD repeats)
{
    RETFAIL(dmsegment->Download(xgc->dmaudiopath));

	RETFAIL(dmsegment->SetRepeats(repeats));

    RETFAIL(xgc->dmperformance->PlaySegmentEx(
        dmsegment,			// Segment to play.
        NULL,				// Used for songs; not implemented.
        NULL,				// For transitions. 
        DMUS_SEGF_DEFAULT | DMUS_SEGF_SECONDARY,  // Flags.
        0,					// Start time; 0 is immediate.
        &dmsegmentstate,    // Pointer that receives segment state.
        NULL,				// Object to stop.
        xgc->dmaudiopath	// Audiopath
    ));
	
	return S_OK;
}


HRESULT XgcAudioClip::IsPlaying(void)
{
	return xgc->dmperformance->IsPlaying(dmsegment, NULL);
}


XgcAudioClip::XgcAudioClip(void)
{
	dmsegment = NULL;
}


XgcAudioClip::Release(void)
{
    if(dmsegment)
		dmsegment->Unload(xgc->dmaudiopath);
	DX_RELEASE(dmsegment);
}


HRESULT XgcAudioClip::Stop(void)
{
    return xgc->dmperformance->Stop(dmsegment, NULL, 0, NULL);
}


HRESULT XgcAudioClip::SetMasterTempo(float tempo)
{
	xgc->dmperformance->SetGlobalParam(GUID_PerfMasterTempo, (void *)&tempo, sizeof(float));                    
	return S_OK;
}


HRESULT XgcAudioClip::SetMasterVolume(long decibels)
{
	xgc->dmperformance->SetGlobalParam(GUID_PerfMasterVolume, (void *)&decibels, sizeof(long));

	return S_OK;
}


HRESULT XgcAudioClip::SetTempo(double tempo)
{
	DMUS_TEMPO_PARAM tempodata = {0, tempo};
	
	dmsegment->SetParam(GUID_TempoParam, 0xFFFFFFFF, DMUS_SEG_ALLTRACKS, 0, (void *)&tempodata);

	return S_OK;
}

/*HRESULT XgcAudioClip::SetVolume(long decibels)
{
	DMUS_TEMPO_PARAM tempodata = {0, tempo};
	
	dmsegment->SetParam(GUID_TempoParam, 0xFFFFFFFF, DMUS_SEG_ALLTRACKS, 0, (void *)&tempodata);

	return S_OK;
}*/