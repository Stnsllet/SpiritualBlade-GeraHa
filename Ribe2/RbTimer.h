// RbTimer.h: RbTimer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBTIMER_H__2AD94D16_8CDA_44A0_949F_59A7A3BC5A85__INCLUDED_)
#define AFX_RBTIMER_H__2AD94D16_8CDA_44A0_949F_59A7A3BC5A85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RbMasterTimer  
{
public:
	class RbTimer
	{
	public:
		RbTimer();
		double Passed( double OldTime ) const;
		operator double() const{return NowTime;}
	private:
		double NowTime;
	};
	RbMasterTimer();
	operator double() const{return NowTime;}
	 void Sleep();
	 void Check();
private:
	DWORD OldTime;
	double NowTime;
};

typedef RbMasterTimer::RbTimer RbTimer;

#endif // !defined(AFX_RBTIMER_H__2AD94D16_8CDA_44A0_949F_59A7A3BC5A85__INCLUDED_)
