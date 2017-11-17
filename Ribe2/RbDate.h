
#ifndef __RBDATE__
#define __RBDATE__

#include "File.h"

class RbDate
{
	RbDate(){AllZero(Time);}

	bool SetBattleTime( double NewTime )
	{
		long NewLongTime = (long) NewTime;
		AllZero(Time);
		Time.wMilliseconds = NewLongTime%1000;
		NewLongTime -= Time.wMilliseconds;
		NewLongTime /= 1000;
		Time.wSecond = NewLongTime%60;
		NewLongTime -= Time.wSecond;
		NewLongTime /= 60;
		Time.wMinute = NewLongTime%60;
		NewLongTime -= Time.wSecond;
		NewLongTime /= 60;
		if( NewLongTime > 60000 )Time.wHour = 60000;
		else Time.wHour = NewLongTime;
		return true;
	}
	bool SetFile( const LPCTSTR FileName )
	{
		File fp;
		if( FAILED(fp.Open( FileName )) )return false;
		return SetFile( fp.GetWriteTime() );
	}
	bool SetFile( const FILETIME& NewTime )
	{
		return !!FileTimeToSystemTime( &NewTime, &Time );
	}

	SYSTEMTIME Time;
};

#endif //__RBDATE__
