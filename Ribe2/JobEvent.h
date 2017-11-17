// JobEvent.h: JobEvent クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOBEVENT_H__6D2FF309_F0F6_4CE9_B77F_32736CDFD193__INCLUDED_)
#define AFX_JOBEVENT_H__6D2FF309_F0F6_4CE9_B77F_32736CDFD193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbActStatus.h"
#include "PolyStation.h"

struct JobEvent
{
public:

struct EventHouse : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("House");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	String HouseName;
	int Rnd;
	XMLPaser SorceXML;
};

struct EventAct : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Act");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	long ActionID;
	float Direction;
	double LastTime;
	double ActStart;
	double ActTime;
	double ActLife;
	RefPtr<RbActStatus> pStatus;
	RbPosition TagetPos;
};

struct EventState : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("State");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	XMLPaser SorceXML;
	RefPtr<RbActStatus> pStatus;
	BattleAction CheckAct;
	float CheckHp, CheckMp;
	long SpecialID;
};

struct EventDelay : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Delay");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	double StartTime;
	double Time;
	LockCounter::Locker Lock;
	XMLPaser SorceXML;
};

struct EventTimer : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Timer");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	double StartTime;
	double Time;
	long Cycle;
	XMLPaser SorceXML;
};

struct EventCount : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Count");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	String CheckName;
	long CheckCount;
	long ArmyID;
	long Cycle;
	XMLPaser SorceXML;
};

struct EventRange : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Range");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	float Range;
	RefPtr<RbActStatus> pStatus;
	RefPtr<RbActStatus> pTaget;
	RbPosition Pos;
	bool bPos;
	long EneCount;
	XMLPaser SorceXML;
};

struct EventTres : public RefAuto
{
public:
	static LPCTSTR GetEventName(){return _T("Tres");}
	bool ReadEvent( const XMLPaser& EventXML, GameSystem& System );
	bool Job( GameSystem& System );
private:
	RefPtr<PolyStation> pStation;
	String Traps;
	String MonsterPath;
	long TrapRnd;
	float Range;
	RbPosition Pos;
	XMLPaser SorceXML;
};

};	//JobEvent

#endif // !defined(AFX_JOBEVENT_H__6D2FF309_F0F6_4CE9_B77F_32736CDFD193__INCLUDED_)
