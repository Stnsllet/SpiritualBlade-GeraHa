// Timer.h: Timer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_H__95A67A36_0847_4792_9D5F_81F01C5512E7__INCLUDED_)
#define AFX_TIMER_H__95A67A36_0847_4792_9D5F_81F01C5512E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"winmm.lib")

class Timer  
{
public:
	Timer(){ Time = timeGetTime(); }
	operator DWORD() const{return Time;}
	DWORD Passed( DWORD OldTime )
	{
		if( OldTime > Time )return Time + OldTime;
		return Time - OldTime;
	}
private:
	DWORD Time;
};

class TimeChecker
{
public:
	TimeChecker(){pMsg=_T("TimeChecker");}
	TimeChecker(LPCTSTR pSetMsg){pMsg=pSetMsg;}
	~TimeChecker()
	{
		ATLTRACE( "%s :	%d (ms)\n", pMsg, Timer().Passed( Time ) );
	}
	LPCTSTR pMsg;
	Timer Time;
};

#ifdef _DEBUG
#define TIMECHK(x)	TimeChecker TimeCheckOn(x)
#else
#define TIMECHK(x)
#endif

#endif // !defined(AFX_TIMER_H__95A67A36_0847_4792_9D5F_81F01C5512E7__INCLUDED_)
