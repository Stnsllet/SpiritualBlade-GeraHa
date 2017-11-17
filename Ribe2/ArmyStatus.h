// ArmyStatus.h: ArmyStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMYSTATUS_H__E7506AF5_D07B_4356_97F5_6A1951A8BF2B__INCLUDED_)
#define AFX_ARMYSTATUS_H__E7506AF5_D07B_4356_97F5_6A1951A8BF2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
class XMLPaser;

class ArmyStatus  
{
public:
	enum eArmy
	{
		eUser = 0,
		eFriend = 0,
		eEnemy,
		eNeutral,

		eMaxArmyID,
	};
	struct Counter
	{
		long Live;
		long Enter;
		long Dead;
	};
	ArmyStatus();
	~ArmyStatus();
	bool SetArmyName( const String& NewName );
	static eArmy GetArmyID( const String& ArmyName );
	static bool LoadSystemXML( const XMLPaser& SystemXML );

	long CountChange( long Change );
	long Enter(){Count.Enter++;return ++Count.Live;}
	long Dead(){Count.Dead++;return --Count.Live;}
	long Escape(){return --Count.Live;}

	eArmy RefArmyID() const{return ArmyID;}
	long RefLiveCount() const{return Count.Live;}
	long GetMedical() const;
	const Counter& RefCounter() const{return Count;}
protected:
	String Name;
	eArmy ArmyID;
	Counter Count;
	static long MoneyDef [eMaxArmyID];
	long Medical;
};

#endif // !defined(AFX_ARMYSTATUS_H__E7506AF5_D07B_4356_97F5_6A1951A8BF2B__INCLUDED_)
