// LandMoveType.cpp: LandMoveType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "LandType.h"
#include "LandMoveType.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandMoveType::LandMoveType()
{
	Speed = 1;
}

LandMoveType::~LandMoveType()
{
}

bool LandMoveType::SetValue( BYTE ID, const MoveData& NewValue )
{
	MoveValue[ID] = NewValue;
	return true;
}

bool LandMoveType::SetData( const LandType& BaseType, const XMLPaser& MoveXML )
{
	MyName = MoveXML.AttributeString(TAG::NAME);
	if( MyName.empty() )return false;
	Speed = MoveXML.AttributeFloat(_T("speed"));
	if( Speed <= 0 )Speed = 1.0f;

	float DefaultValue = MoveXML.AttributeFloat(_T("default"));
	if( DefaultValue <= 0 )DefaultValue = 1.0f;
	AllZero(MoveValue);
	for( int i = 0; i < count_of(MoveValue); i++ )
	{
		MoveValue[i].Move = DefaultValue;
		MoveValue[i].Dmg = 1.0f;
	}
	XMLList TypeList;
	TypeList.Query( MoveXML, _T("Mov") );
	long Count = TypeList.GetCount();
	for( i = 0; i < Count; i++ )
	{
		const XMLPaser& Ref = TypeList.GetItem(i);
		String Name = Ref.AttributeString(TAG::NAME);
		MoveData NewData;
		NewData.Move = Ref.AttributeFloat(_T("mov"));
		NewData.Dmg = Ref.AttributeFloat(_T("dmg"));
		if( NewData.Dmg <= 0 )NewData.Dmg = 1.0f;
		NewData.bUnRadar = Ref.AttributeBool(_T("unradar"));
		BaseType.SetMoveValue( *this,
			Name, NewData );
	}
	return true;
}
