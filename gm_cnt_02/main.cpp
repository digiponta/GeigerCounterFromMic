// gm_cnt_01.cpp : コンソール アプリケーション用のエントリ ポイントの定義
// Visual Studio 2017

#include "pch.h"
#include <iostream>
// #include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>

#include "gm_cnt_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 唯一のアプリケーション オブジェクト

// WinApp theApp;
using namespace std;

WAVEFORMATEX wfe;
WAVEHDR whdr;
short *bWave;
HWAVEOUT hWaveOut;
HWAVEIN hWaveIn;

UINT numDevs;
HANDLE hEvent;

#define MAX_NUM_DEVS 10
HMIXER hMixer[MAX_NUM_DEVS];
int isOpenedMixer[MAX_NUM_DEVS];
MIXERLINE mixerLine[MAX_NUM_DEVS];
UINT idMic;
int onRec;



int MyMain(int argc, char* argv[], char* envp[]);


void CALLBACK MyWaveInProc ( HWAVEIN hwi, UINT uMSG, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ) {
	int ret;

//	return;

//	if ( uMSG != 958 ) fprintf( stderr , "CALLBACKE! (%d) @ %d\n", uMSG, __LINE__ );

	if ( uMSG == WIM_OPEN ) {
//		fprintf( stderr, "WIM_OPEN(%d)\n", __LINE__ );

		if (1) {
			ret = waveInAddBuffer( hwi, &whdr, sizeof(WAVEHDR) );
			if ( ret != MMSYSERR_NOERROR ) {
				fprintf( stderr, "ERROR(%d)\n", __LINE__ );
				return ;
			}
			ret = waveInStart( hwi );
			if ( ret != MMSYSERR_NOERROR ) {
				fprintf( stderr, "ERROR(%d)\n", __LINE__ );
				return ;
			}
		}
	}

	if ( uMSG == WIM_DATA ) {
		ret = waveInStop( hwi );
		if ( ret != MMSYSERR_NOERROR ) {
//			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return ;
		}
		ret = waveInClose( hwi );
		if ( ret != MMSYSERR_NOERROR ) {
//			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return ;
		}
//		(long)hWaveIn = -1;
		fprintf( stderr, "WIM_DATA(%d)\n", __LINE__ );
		onRec = 0;
		SetEvent( hEvent );
	}

}

// char *test = (char *)"C:\\share\\infopub-elem-2017\\demo\\gm\\gm_cnt_02\\Debug";
char *test = (char *)"C:\\share\\infopub-elem-2017\\demo";


int MyMain(int argc, char* argv[], char* envp[] )
{
	int ret = 0;
	int i;
	FILE *fp_log;
	char fName[1024];


	// if (argc < 2) {
	//	fprintf( stderr, "ERROR(%d) argc < 2\n", __LINE__ );
	//	return -1;
	// }
	// fprintf( stderr, "argv[0]: %s, argv[1]: %s, \n", (char *)argv[0], (char *)argv[1] );

	{
		int mic_num = waveInGetNumDevs();
		fprintf(stderr, "mic_num = %d\n", mic_num );

		for (DWORD i = 0; i < mic_num; i++) {
			// 入力デバイスの性能を取得する.
			WAVEINCAPS caps;
			if (waveInGetDevCaps(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
				fprintf( stderr, "%d - %s\n", i, caps.szPname );
			}
		}








		int j;
		numDevs = mixerGetNumDevs();
		if ( numDevs == 0 ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return -1;
		}
		fprintf( stderr, "numDevs = %d\n", numDevs );
		if ( numDevs > MAX_NUM_DEVS ) numDevs = MAX_NUM_DEVS;
		
		for (i=0, j=0; i<numDevs; i++ ){
			ret = mixerOpen( &(hMixer[i]), (UINT)i, 0, 0, MIXER_OBJECTF_MIXER );	
			if ( ret != MMSYSERR_NOERROR ) {
				fprintf( stderr, "ERROR(%d) can't open mixar[%d]\n", __LINE__,
					i);
				isOpenedMixer[i] = 0;
			} else {
				j++;
				isOpenedMixer[i] = 1;
			}
		}
		if ( j < 1 ) {
			fprintf( stderr, "ERROR(%d) cna't open all mixers\n", __LINE__ );
			return -1;
		} else {
			fprintf( stderr, "%d opendMixars\n", j );
		}


		for (i=0, idMic = -1; i < numDevs; i++ ){
			mixerLine[i].cbStruct = sizeof( mixerLine );
			mixerLine[i].dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
			ret = mixerGetLineInfo( (HMIXEROBJ)hMixer[i], &(mixerLine[i]), 
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE );
			if ( ret == MMSYSERR_NOERROR ) {
				fprintf( stderr, "found hMixer[%d]\n", i );
				fprintf( stderr, 
					"i = %d\nName: %s, \nsName: %s\nTargetName: %s\ndwDeviceID: %d\n",
					i,
					mixerLine[i].szName, 
					mixerLine[i].szShortName,
					mixerLine[i].Target.szPname,
					mixerLine[i].Target.dwDeviceID
				);
				if ( (mixerLine[i].szName)[0] != (char)'R' ){
					idMic = i;
				}
			}
		}
		fprintf( stderr, "Mic ID = %d\n", idMic );

		MIXERCONTROL mixerCtrl;
		MIXERLINECONTROLS mixerLineCtrls;

		mixerLineCtrls.cbStruct = sizeof ( mixerLineCtrls );
		mixerLineCtrls.dwLineID = mixerLine[idMic].dwLineID;
		mixerLineCtrls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		mixerLineCtrls.cControls = 1;
		mixerLineCtrls.cbmxctrl = sizeof ( mixerCtrl );
		mixerLineCtrls.pamxctrl = &mixerCtrl;
		ret = mixerGetLineControls(
			(HMIXEROBJ)hMixer[idMic], &mixerLineCtrls, 
			MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE );
		if ( ret != MMSYSERR_NOERROR ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		}
		{		
			MIXERCONTROLDETAILS_UNSIGNED micVolume;
			MIXERCONTROLDETAILS mixerDts;
			
			mixerDts.cbStruct = sizeof( mixerDts );
			mixerDts.dwControlID = mixerCtrl.dwControlID;
			mixerDts.cChannels = 1;
			mixerDts.cMultipleItems = 0;
			mixerDts.cbDetails = sizeof ( MIXERCONTROLDETAILS_UNSIGNED );
			mixerDts.paDetails = &micVolume;

			ret = mixerGetControlDetails( (HMIXEROBJ)hMixer[idMic],
				&mixerDts, 
				MIXER_GETCONTROLDETAILSF_VALUE );
			if (ret != MMSYSERR_NOERROR ){
				fprintf( stderr, "ERROR(%d)\n", __LINE__ );
				return -1;
			}
			fprintf( stderr, "Vol: %d, Max: %d\n", micVolume.dwValue,
				mixerCtrl.Bounds.dwMaximum );

//			micVolume.dwValue = 24348; // needed value for GM counter
			micVolume.dwValue = 20000; // needed value for GM counter

			mixerDts.cbStruct = sizeof( mixerDts );
			mixerDts.dwControlID = mixerCtrl.dwControlID;
			mixerDts.cChannels = 1;
			mixerDts.cMultipleItems = 0;
			mixerDts.cbDetails = sizeof ( MIXERCONTROLDETAILS_UNSIGNED );
			mixerDts.paDetails = &micVolume;

			ret = mixerSetControlDetails( (HMIXEROBJ)hMixer[idMic],
				&mixerDts, 
				MIXER_SETCONTROLDETAILSF_VALUE );
			if (ret != MMSYSERR_NOERROR ){
				fprintf( stderr, "ERROR(%d)\n", __LINE__ );
				return -1;
			}
			fprintf( stderr, "Changed to Vol: %d, Max: %d\n", micVolume.dwValue,
				mixerCtrl.Bounds.dwMaximum );
		
		}


		for (i=0; i<numDevs; i++ ){
			ret = mixerClose( hMixer[i] );	
		}

	}
//	return 0;

//	hEvent = CreateEvent( NULL, FALSE, FALSE, (LPWSTR)"GM_CAPTURE" );
	hEvent = CreateEvent(NULL, FALSE, FALSE, (LPCSTR)"GM_CAPTURE");
	if ( hEvent == NULL ) {
		fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		return -1;
	}

	

	wfe.nChannels = 1;
	wfe.wFormatTag = WAVE_FORMAT_PCM; //  +(wfe.nChannels << 16);
	wfe.nSamplesPerSec = 16000 ;
	// wfe.nSamplesPerSec = 44100 ;
	wfe.wBitsPerSample = 16;
	wfe.nAvgBytesPerSec = wfe.nSamplesPerSec * wfe.nChannels * wfe.wBitsPerSample / 8;
	wfe.nBlockAlign = wfe.nChannels * ((wfe.wBitsPerSample + 7) >> 3); //  wfe.nChannels * wfe.wBitsPerSample / 16;

//	whdr.dwBufferLength = wfe.nAvgBytesPerSec * 60 ; // 60 sec
	whdr.dwBufferLength = wfe.nAvgBytesPerSec * 120 ; // 120 sec

	bWave = (short *)malloc( whdr.dwBufferLength );
	if ( bWave == NULL ) {
		fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		return -1 ;
	}
	memset( bWave, 0, whdr.dwBufferLength );

	whdr.lpData = (char *)bWave;

	whdr.dwLoops = 0; // 1;
	whdr.dwBytesRecorded = 0;
	whdr.dwFlags = 0;
	whdr.lpNext = NULL;
	whdr.dwUser = 0;
	whdr.reserved = 0;

	onRec = 1;

	ret = waveInOpen(&hWaveIn, WAVE_MAPPER, &wfe, (DWORD_PTR)MyWaveInProc, 0, CALLBACK_NULL);
	// ret = waveInOpen( &hWaveIn, 0, &wfe, (DWORD_PTR)MyWaveInProc, 0, CALLBACK_NULL );
	if ( ret != MMSYSERR_NOERROR ) {
		fprintf( stderr, "ERROR(%d): %d\n", __LINE__, ret );
		fprintf( stderr, "MMSYSERR_ALLOCATED = %d\n", MMSYSERR_ALLOCATED);
		fprintf(stderr, "MMSYSERR_BADDEVICEID = %d\n", MMSYSERR_BADDEVICEID);
		fprintf(stderr, "MMSYSERR_NODRIVER = %d\n", MMSYSERR_NODRIVER);
		fprintf(stderr, "MMSYSERR_NOMEM = %d\n", MMSYSERR_NOMEM);
		fprintf(stderr, "WAVERR_BADFORMAT = %d\n", WAVERR_BADFORMAT);
		return -1;
	}
	ret = waveInPrepareHeader( hWaveIn, &whdr, sizeof(WAVEHDR) );
	if ( ret != MMSYSERR_NOERROR ) {
		fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		return -1;
	}

	if (1) {	

		ret = waveInAddBuffer( hWaveIn, &whdr, sizeof(WAVEHDR) );
		if ( ret != MMSYSERR_NOERROR ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return -1;
		}
		ret = waveInStart( hWaveIn );
		if ( ret != MMSYSERR_NOERROR ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return -1;
		}
	}

	fprintf( stderr, "wait for GM capture (%d)\n", __LINE__ );
//	Sleep( 70 * 1000  ); // wait 60 sec
	
//	ret = WaitForSingleObject( hEvent, 70000 );
	ret = WaitForSingleObject( hEvent, 130000 );

	if ( ret == WAIT_FAILED ) {
		fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		return -1;
	}
	fprintf( stderr, "finished to wait\n" );

	if ( onRec != 0 ) 
	{
		ret = waveInStop( hWaveIn );
		if ( ret != MMSYSERR_NOERROR ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return -1;
		}
		ret = waveInClose( hWaveIn );
		if ( ret != MMSYSERR_NOERROR ) {
			fprintf( stderr, "ERROR(%d)\n", __LINE__ );
			return -1;
		}
	}

	short max, min, cur, prv, nxt, thr;
	short thr2;
	int cnt;
	int cnt2;

	max = min = 0;
	thr = 27000;

	SYSTEMTIME mytm;
	int micro_svh;


	GetLocalTime( &mytm );
	// fprintf( stderr, "%d\n", whdr.dwBufferLength );

	for (i=0, prv = cur = 0; i<whdr.dwBufferLength/2; i++ ){
//		fprintf( stdout, "%d,\n", (short)bWave[i] );
		cur = bWave[i];
//		if ( cur  == 0xffffcdcd ) break;
		if ( max < cur ) max = cur;
		if ( min > cur ) min = cur;
	}

	if ( thr >= (-min) ) {
		thr = (-min ) ;	
	}
	if ( thr < 1000 ) thr = 1000;
	
	//	if ( thr + min <= 0 ) thr = (max * 7 ) /10;

	thr2 = (- min) * 7 / 10 ;
	if (thr2 >= max ) thr2 = (max * 7 ) /10;
	if ( thr2 < 1000 ) thr2 = 1000;


	for (i=0, nxt = cur = 0, cnt = cnt2 = 0; i<whdr.dwBufferLength/2; i++ ){
		cur = bWave[i];
		if ( i < whdr.dwBufferLength/2 - 1 ) nxt = bWave[i+1];
		else nxt = 0;
		
		
//		if ( (prv < thr) && (nxt >= thr) ) 
		if ( (prv < thr)  ) 
		
		{
			if ( cur >= thr ) cnt++;
		}

		if ( (prv < thr2)  ) 
		{
			if ( cur >= thr2 ) cnt2++;
		}

		prv = cur;
	}
//	micro_svh = ((cnt - 60) * 1000) / (3100 - 60);
	micro_svh = ((cnt) * 1000) / (3100);

	fprintf( stdout, "%04d/%02d/%02d %02d:%02d:%02d, %d, %d, %d, %d, %d\n", 
		mytm.wYear, mytm.wMonth, mytm.wDay, mytm.wHour, mytm.wMinute, mytm.wSecond,
		cnt/2 , max, min, thr, cnt2/2 );

	sprintf_s( fName, "%s\\gmdata_%04d%02d%02d.csv",
				test, mytm.wYear, mytm.wMonth, mytm.wDay	);
		//		(char *)argv[1], mytm.wYear, mytm.wMonth, mytm.wDay	);

	fprintf( stderr, "fName: %s\n", fName );

	fopen_s( &fp_log, fName, "a" );
	if ( fp_log == NULL ) {
		fprintf( stderr, "ERROR(%d)\n", __LINE__ );
		return -1;
	}

	fprintf( fp_log, "%04d/%02d/%02d %02d:%02d:%02d, %5d, %6d, %6d, %6d, %6d\n", 
		mytm.wYear, mytm.wMonth, mytm.wDay, mytm.wHour, mytm.wMinute, mytm.wSecond,
		cnt/2 , max, min, thr, cnt2/2 );

	fclose( fp_log );
	
	free( bWave );
	return ret;
}

