// ArmyStatus.cpp: ArmyStatus クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ArmyStatus.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

long ArmyStatus::MoneyDef[] = {300,100,100};

ArmyStatus::ArmyStatus()
{
	AllZero(Count);
	Medical = MoneyDef[eUser];
}

ArmyStatus::~ArmyStatus()
{
}

// 軍団番号取得
ArmyStatus::eArmy ArmyStatus::GetArmyID( const String& ArmyName )
{
	if( ArmyName == _T("user") )return eUser;
	if( ArmyName == _T("fri") )return eFriend;
	if( ArmyName == _T("friend") )return eFriend;
	if( ArmyName == _T("ene") )return eEnemy;
	if( ArmyName == _T("enemy") )return eEnemy;
	if( ArmyName == _T("ntl") )return eNeutral;
	if( ArmyName == _T("neutral") )return eNeutral;

	return eEnemy;
}

bool ArmyStatus::LoadSystemXML( const XMLPaser& SystemXML )
{
	XMLList MoneyList;
	MoneyList.Query( SystemXML, _T("Money") );
	for( int i = 0; i < MoneyList.GetCount(); i++ )
	{
		XMLPaser MoneyXML = MoneyList.GetItem(i);
		String Army = MoneyXML.AttributeString(TAG::NAME);
		long ID = GetArmyID(Army);
		if( ID >= 0 && ID < count_of(MoneyDef) )
			MoneyDef[ID] = MoneyXML.AttributeLong(_T("val"));
	}
	return true;
}

bool ArmyStatus::SetArmyName( const String& NewName )
{
	Name = NewName;
	ArmyID = GetArmyID( NewName );
	Medical = MoneyDef[ArmyID];
	return true;
}

long ArmyStatus::GetMedical() const
{
	return (Count.Dead * Medical);
}

long ArmyStatus::CountChange( long Change )
{
	if( Change < 0 )while((Change++)<0)Dead();
	else if( Change > 0 )while((Change--)>0)Enter();
	return Count.Live;
}
