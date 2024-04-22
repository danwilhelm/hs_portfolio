/*--------------------------------------------------------------------

        MMIO.C
        Routines to read from and write to multimedia RIFF files
        with a "WAVE" format.

        PUBLIC FUNCTIONS
            ReadWaveData        read info from a .WAV file
            WriteWaveData       write info to a .WAV file

        PRIVATE FUNCTION
            ReportError         tell user what went wrong

        program by Michael Barnes
        copyright 1992, 1993 by Brian Myers, Chris Doner, & Eric Hamer
        from Mastering Windows NT Programming

  --------------------------------------------------------------------*/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>           // multimedia definitions
#include "rSrc.h"

/*--------------------------------------------------------------------
        FUNCTION PROTOTYPE
  --------------------------------------------------------------------*/

static void ReportError( HWND hWnd, int iErrorID );

BOOL ReadWaveData (
    HWND hWnd,
    LPSTR lpszFileName,
    LPSTR *lplpWaveData,            // points to buffer
    DWORD *lpdwWaveDataSize,        // size of buffer
    DWORD *lpdwSamplesPerSec );      // sampling rate

BOOL WriteWaveData (
    HWND hWnd,                      // main window
    LPSTR lpszFileName,             // destination file
    LPSTR lpWaveData,               // data source buffer
    DWORD dwWaveDataSize,           // size of data in buffer
    DWORD dwSamplesPerSec );         // sampling rate


/*--------------------------------------------------------------------
        READ WAVE DATA
        Read waveform data from a RIFF file into a memory buffer.

        RETURN
        TRUE if we successfully fill the buffer, otherwise FALSE.
        If the function returns TRUE, then the last three parameters
        return information about the new buffer.
  --------------------------------------------------------------------*/

BOOL ReadWaveData (
    HWND hWnd,
    LPSTR lpszFileName,
    LPSTR *lplpWaveData,            // points to buffer
    DWORD *lpdwWaveDataSize,        // size of buffer
    DWORD *lpdwSamplesPerSec )      // sampling rate
{
    HMMIO         hmmio;            // file handle
    MMCKINFO      mmckinfoWave;     // description of "WAVE" chunk
    MMCKINFO      mmckinfoFmt;      // description of "fmt " chunk
    MMCKINFO      mmckinfoData;     // description of "data" chunk
    PCMWAVEFORMAT pcmWaveFormat;    // contents of "fmt " chunk
    LONG          lFmtSize;         // size of "fmt " chunk
    LONG          lDataSize;        // size of "data" chunk
    LPSTR         lpData;           // pointer to data buffer

    /* open the given file for reading using multimedia file I/O */
    hmmio = mmioOpen( lpszFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );
    if (hmmio == NULL)
    {
        ReportError( hWnd, IDS_CANTOPENFILE );
        return( FALSE );
    }

    /* locate a chunk with a "WAVE" form type */
    mmckinfoWave.fccType = mmioFOURCC('W','A','V','E');
    if (mmioDescend( hmmio, &mmckinfoWave, NULL, MMIO_FINDRIFF ) != 0)
    {
        ReportError( hWnd, IDS_NOTWAVEFILE );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* find the format subchunk */
    mmckinfoFmt.ckid = mmioFOURCC('f','m','t',' ');
    if (mmioDescend( hmmio, &mmckinfoFmt, &mmckinfoWave,
                     MMIO_FINDCHUNK ) != 0)
    {
        ReportError( hWnd, IDS_CORRUPTEDFILE );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* read the format subchunk */
    lFmtSize = (LONG)sizeof( pcmWaveFormat );
    if (mmioRead( hmmio, (LPSTR)&pcmWaveFormat, lFmtSize ) != lFmtSize)
    {
        ReportError( hWnd,IDS_CANTREADFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* ascend out of the format subchunk */
    if (mmioAscend( hmmio, &mmckinfoFmt, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTREADFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* make sure the sound file is an 8-bit mono PCM WAVE file */
    if ((pcmWaveFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
       || (pcmWaveFormat.wf.nChannels != 1)
       || (pcmWaveFormat.wBitsPerSample != 8))
    {
        ReportError( hWnd, IDS_UNSUPPORTEDFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* find the data subchunk */
    mmckinfoData.ckid = mmioFOURCC('d','a','t','a');
    if (mmioDescend( hmmio, &mmckinfoData, &mmckinfoWave,
                     MMIO_FINDCHUNK ) != 0)
    {
        ReportError( hWnd, IDS_CORRUPTEDFILE );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* get the size of the data subchunk */
    lDataSize = (LONG)mmckinfoData.cksize;
    if (lDataSize == 0)
    {
        ReportError( hWnd,IDS_NOWAVEDATA );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* allocate and lock memory for the waveform data */
    lpData = (char *)GlobalAllocPtr( GMEM_MOVEABLE, lDataSize );
    if (!lpData)
    {
        ReportError( hWnd, IDS_OUTOFMEMORY );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* read the data subchunk */
    if (mmioRead( hmmio, (LPSTR)lpData, lDataSize ) != lDataSize)
    {
        ReportError( hWnd, IDS_CANTREADDATA );
        GlobalFreePtr( lpData );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* close the file */
    mmioClose( hmmio, 0 );

    /* set return variables */
    *lplpWaveData = lpData;
    *lpdwWaveDataSize = (DWORD)lDataSize;
    *lpdwSamplesPerSec = pcmWaveFormat.wf.nSamplesPerSec;
    return( TRUE );
}
/*--------------------------------------------------------------------
        WRITE WAVE DATA
        Transfer waveform data from a memory buffer to a disk file
  --------------------------------------------------------------------*/

BOOL WriteWaveData (
    HWND hWnd,                      // main window
    LPSTR lpszFileName,             // destination file
    LPSTR lpWaveData,               // data source buffer
    DWORD dwWaveDataSize,           // size of data in buffer
    DWORD dwSamplesPerSec )         // sampling rate
{
    HMMIO         hmmio;            // file handle
    MMCKINFO      mmckinfoWave;     // description of "WAVE" chunk
    MMCKINFO      mmckinfoFmt;      // description of "fmt " chunk
    MMCKINFO      mmckinfoData;     // description of "data" chunk
    PCMWAVEFORMAT pcmWaveFormat;    // contents of "fmt " chunk
    LONG          lFmtSize;         // size of "fmt " chunk
    LONG          lDataSize;        // size of "data" chunk

    /* store size of the "data" subchunk */
    lDataSize = (LONG)dwWaveDataSize;

    /* store size of the format subchunk */
    lFmtSize = (LONG)sizeof( pcmWaveFormat );

    /* open the given file for writing using multimedia file I/O */
    hmmio = mmioOpen( lpszFileName, NULL,
                      MMIO_ALLOCBUF | MMIO_WRITE | MMIO_CREATE );
    if (hmmio == NULL)
    {
        ReportError( hWnd, IDS_CANTOPENFILE );
        return( FALSE );
    }

    /* create a "RIFF" chunk with a "WAVE" form type */
    mmckinfoWave.fccType = mmioFOURCC( 'W','A','V','E' );
    mmckinfoWave.ckid = mmioFOURCC( 'W','A','V','E' );
	mmckinfoWave.cksize = lDataSize + lFmtSize;
	mmckinfoWave.dwDataOffset = 0;
	mmckinfoWave.dwFlags = MMIO_DIRTY;
    if (mmioCreateChunk( hmmio, &mmckinfoWave, MMIO_CREATERIFF ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEWAVE );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    // Create the format subchunk.
    // Since we know the size of this chunk, specify it in the
    // MMCKINFO structure so MMIO doesn't have to seek back and
    // set the chunk size after ascending from the chunk.

    mmckinfoFmt.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    mmckinfoFmt.cksize = lFmtSize;
    if (mmioCreateChunk( hmmio, &mmckinfoFmt, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* initialize PCMWAVEFORMAT structure */
    pcmWaveFormat.wf.wFormatTag      = WAVE_FORMAT_PCM;
    pcmWaveFormat.wf.nChannels       = 1;
    pcmWaveFormat.wf.nSamplesPerSec  = dwSamplesPerSec;
    pcmWaveFormat.wf.nAvgBytesPerSec = dwSamplesPerSec;
    pcmWaveFormat.wf.nBlockAlign     = 1;
    pcmWaveFormat.wBitsPerSample     = 8;

    /* write the format subchunk */
    if (mmioWrite( hmmio, (LPSTR)&pcmWaveFormat, lFmtSize ) != lFmtSize)
    {
        ReportError( hWnd, IDS_CANTWRITEFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* ascend out of the format subchunk */
    if (mmioAscend( hmmio, &mmckinfoFmt, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEFORMAT );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* create the "data" subchunk that holds the waveform samples */
    mmckinfoData.ckid  = mmioFOURCC( 'd', 'a', 't', 'a' );
    mmckinfoData.cksize = lDataSize;
    if (mmioCreateChunk( hmmio, &mmckinfoData, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEDATA );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* write the "data" subchunk */
    if (mmioWrite( hmmio, lpWaveData, lDataSize ) != lDataSize)
    {
        ReportError( hWnd, IDS_CANTWRITEDATA );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* ascend out of the "data" subchunk */
    if (mmioAscend( hmmio, &mmckinfoData, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEDATA );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* ascend out of the "WAVE" chunk -- causes size to be written */
    if (mmioAscend( hmmio, &mmckinfoWave, 0 ) != 0)
    {
        ReportError( hWnd, IDS_CANTWRITEWAVE );
        mmioClose( hmmio, 0 );
        return( FALSE );
    }

    /* close the file */
    mmioClose( hmmio, 0 );

    return( TRUE );
}

/*--------------------------------------------------------------------
        REPORT ERROR
        Report given error to the user
  --------------------------------------------------------------------*/

static void ReportError (
    HWND hWnd,
    int iErrorID )
{
    HINSTANCE hInstance;
    char szErrStr[MAX_RSRC_STRING_LEN];
    char szCaption[MAX_RSRC_STRING_LEN];

    hInstance = GetWindowInstance( hWnd );
    LoadString( hInstance, iErrorID, szErrStr, sizeof(szErrStr) );
    LoadString( hInstance, IDS_CAPTION, szCaption,
                sizeof(szCaption) );
    MessageBox( hWnd, szErrStr, szCaption,
                MB_ICONEXCLAMATION | MB_OK );
    return;
}
