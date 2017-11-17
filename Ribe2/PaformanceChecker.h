// PaformanceChecker.h: PaformanceChecker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef __PAFORMANCE_CECKER__
#define __PAFORMANCE_CECKER__

#define PAFORMANCE_CECKER_MAX_TIME_DROP

class PeformanceMaster
{
public:
	PeformanceMaster():pMsg(_T("PeformanceChecker:")){Zero();}
	PeformanceMaster( LPCTSTR SetMSG ):pMsg(SetMSG){Zero();}
	void Zero()
	{
		CallCount=0;
		AllTime.QuadPart=0;
		MaxTime.QuadPart=0;
	}
	void Add( const LARGE_INTEGER& AddTime )
	{
		if(AddTime.QuadPart>MaxTime.QuadPart)
		{
			MaxTime.QuadPart=AddTime.QuadPart;
#ifdef PAFORMANCE_CECKER_MAX_TIME_DROP
			return;
#endif
		}
		++CallCount;
		AllTime.QuadPart += AddTime.QuadPart;
	}

	~PeformanceMaster()
	{
		double QueryTime = PeformanceMaster::GetQueryTime();
		double dMaxTime = ((MaxTime.QuadPart * QueryTime)*1000000);
		if( CallCount <= 0 )
		{
			ATLTRACE("%s	call %4d,	max %.3f (us)\n", pMsg, CallCount, dMaxTime );
			return;
		}
		double dTime = ((AllTime.QuadPart * QueryTime)*1000000)/CallCount;
		ATLTRACE("%s	call %4d,	max %.3f (us)	ave %.3f (us)\n",
			pMsg, CallCount, dMaxTime, dTime );
	}
	static double GetQueryTime()
	{
		LARGE_INTEGER Query_INT;
		QueryPerformanceFrequency( &Query_INT );
		if( Query_INT.QuadPart <= 0 )return 1;
		return ((double)1)/Query_INT.QuadPart;
	}
private:
	LPCTSTR pMsg;
	LARGE_INTEGER AllTime;
	LARGE_INTEGER MaxTime;
	long CallCount;
};

class PeformanceChecker  
{
public:
	PeformanceChecker( PeformanceMaster& Set ):Master( Set )
	{
		QueryPerformanceCounter( &StartTime );
	}
	~PeformanceChecker()
	{
		LARGE_INTEGER EndTime;
		QueryPerformanceCounter( &EndTime );
		EndTime.QuadPart -= StartTime.QuadPart;
		Master.Add( EndTime );
	}

	LARGE_INTEGER StartTime;
	PeformanceMaster& Master;
};

#ifdef _DEBUG
#define __PEFORMANCE_CHECK__
#endif

#ifdef __HI_TRACE__
#define __PEFORMANCE_CHECK__
#endif

#ifdef __PEFORMANCE_CHECK__
#define PEFORMANCE(x)	static PeformanceMaster __PeformanceMaster(x);PeformanceChecker __PeformanceChecker(__PeformanceMaster);
#else
#define PEFORMANCE(x)
#endif

#endif // __PAFORMANCE_CECKER__
