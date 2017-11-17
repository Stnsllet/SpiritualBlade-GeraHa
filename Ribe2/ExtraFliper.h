
#ifndef __EXTRA_FLIPER__
#define __EXTRA_FLIPER__

#include <process.h>
#include "Device.h"

class ExtraFliper
{
public:
	ExtraFliper( Device& Ref ) : RefDevice(Ref)
	{
		bTermnate = false;
		hWakeup = ::CreateEvent( NULL, FALSE, FALSE, NULL );
		hFlipWait = ::CreateEvent( NULL, TRUE, TRUE, NULL );
		hThread = (HANDLE)::_beginthreadex( NULL, 0, ThreadFunc, this, 0, NULL );
	}
	~ExtraFliper()
	{
		TermThread();
	}
	bool FlipWait() const
	{
		DWORD ret = WaitForSingleObject( hFlipWait, 1000 );
		return (ret!=WAIT_TIMEOUT);
	}
	void Flip()
	{
		ResetEvent( hFlipWait );
		SetEvent( hWakeup );
	}
	void TermThread()
	{
		if( bTermnate )return;
		bTermnate = true;
		SetEvent( hWakeup );
		WaitForSingleObject( hThread, 500 );
		CloseHandle(hThread);
		hThread = NULL;
		CloseHandle( hWakeup );
		hWakeup = NULL;
		CloseHandle( hFlipWait );
		hFlipWait = NULL;
	}
private:
	bool bTermnate;
	HANDLE hThread;
	HANDLE hWakeup;
	HANDLE hFlipWait;
	Device& RefDevice;
	static UINT __stdcall ThreadFunc( void* pThis )
	{
		if( pThis == NULL )return 0;
		ExtraFliper& Ref = *(ExtraFliper*)pThis;
		do
		{
			WaitForSingleObject( Ref.hWakeup, INFINITE );
			if( !Ref.bTermnate )Ref.RefDevice.Flip();

			SetEvent( Ref.hFlipWait );
		}while( !Ref.bTermnate );
		return 0;
	}
};

#endif //__EXTRA_FLIPER__
