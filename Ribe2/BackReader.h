
#ifndef __BACK_READER__
#define __BACK_READER__

#include <deque>
#include <process.h>
#include "String.h"
#include "CriticalSection.h"

template<class T=String, long Priority=THREAD_PRIORITY_BELOW_NORMAL> class BackReader
{
public:
	BackReader()
	{
		bTermnate = false;
		Que.hWakeup = ::CreateEvent( NULL, FALSE, FALSE, NULL );
		hThread = (HANDLE)::_beginthreadex( NULL, 0, ThreadFunc, this, 0, NULL );
		SetThreadPriority( hThread, Priority );
	}
	virtual ~BackReader()
	{
		TermThread();
	}
	void WaitTerm( DWORD Interval=50 )
	{
		while( Que.Have() )::Sleep( Interval );
		TermThread()
	}
	void TermThread()
	{
		if( bTermnate )return;
		bTermnate = true;
		SetEvent( Que.hWakeup );
		WaitForSingleObject( hThread, INFINITE );
		CloseHandle(hThread);
		hThread = NULL;
		CloseHandle( Que.hWakeup );
		Que.hWakeup = NULL;
	}
	virtual bool LoadCache( const T& ReadData )=0;
	class _Que
	{
	public:
		void Push( const T& Add )
		{
			CriticalSection Locker( Lock );
			que.push_back(Add);
			SetEvent( hWakeup );
		}
		bool Pop( T& Output )
		{
			CriticalSection Locker( Lock );
			if( que.size() <= 0 )return false;
			Output = *(que.begin());
			que.pop_front();
			return true;
		}
		bool Have() const
		{
			CriticalSection Locker( Lock );
			return que.size() > 0;
		}
		HANDLE hWakeup;
	private:
		CriticalSection::CriticalSectionObject Lock;
		std::deque<T> que;
	}Que;
protected:
	CriticalSection::CriticalSectionObject Lock;
private:
	HANDLE hThread;
	bool bTermnate;
	static UINT __stdcall ThreadFunc( void* pThis )
	{
		if( pThis == NULL )return 0;
		CoInitialize(NULL);
		BackReader& Ref = *(BackReader*)pThis;
		do
		{
			T QueData;
			WaitForSingleObject( Ref.Que.hWakeup, INFINITE );
			while( !Ref.bTermnate && Ref.Que.Pop( QueData ) )
			{
				Ref.LoadCache( QueData );
			}
		}while( !Ref.bTermnate );
		CoUninitialize();
		return 0;
	}
};

#endif // __BACK_READER__
