// ArmyFormation.cpp: ArmyFormation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "RbBattlePaty.h"
#include "BattleSystem.h"
#include "ArmyFormation.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ArmyFormation::ArmyFormation( BattleSystem& RefSystem )
: System( RefSystem )
{
}

ArmyFormation::~ArmyFormation()
{
}

HRESULT ArmyFormation::LoadFile()
{
	return LoadFile( _T("Data/ArmyFormation.xml") );
}

HRESULT ArmyFormation::LoadFile( const String& FileName )
{
	XMLPaser AllXML;
	HRESULT hr = AllXML.Load( FileName );
	if( FAILED(hr) )return hr;

	XMLList ArmyXMLList;
	ArmyXMLList.Query( AllXML, _T("Formation") );
	long Count = ArmyXMLList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ArmyPos NewData;
		if( !NewData.SetXML( ArmyXMLList.GetItem(i) ) )continue;
		ATLASSERT( ArmyPosList.find(NewData.FormationName)==ArmyPosList.end() );
		ArmyPosList[NewData.FormationName] = NewData;
	}

	XMLList PatyXMLList;
	PatyXMLList.Query( AllXML, _T("Paty") );
	Count = PatyXMLList.GetCount();
	for( i = 0; i < Count; i++ )
	{
		PatyForm NewData;
		if( !NewData.SetXML( PatyXMLList.GetItem(i) ) )continue;
		ATLASSERT( PatyFormList.find(NewData.Name)==PatyFormList.end() );
		PatyFormList[NewData.Name] = NewData;
	}
	return S_OK;
}

bool ArmyFormation::ArmyPos::SetXML( const XMLPaser& ReadXML )
{
	FormationName = ReadXML.AttributeString(TAG::NAME);
	if( FormationName.empty() )return false;

	XMLList PatyXMLList;
	PatyXMLList.Query( ReadXML, _T("Paty") );
	long Count = PatyXMLList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ArmyFormation::PatyPos NewData;
		if( !NewData.SetXML( PatyXMLList.GetItem(i) ) )continue;
		PatyList.push_back( NewData );
	}
	return (PatyList.size()>0);
}

bool ArmyFormation::PatyPos::SetXML( const XMLPaser& ReadXML )
{
	PatyID = ReadXML.AttributeLong(_T("id"));
	x = ReadXML.AttributeFloat(_T("x"));
	y = ReadXML.AttributeFloat(_T("y"));
	Angle = ReadXML.AttributeFloat(_T("dir"));
	return true;
}

bool ArmyFormation::PatyForm::SetXML( const XMLPaser& ReadXML )
{
	AllZero( Pos );
	Name = ReadXML.AttributeString(TAG::NAME);
	if( Name.empty() )return false;

	XMLList MemXMLList;
	MemXMLList.Query( ReadXML, TAG::MEMBER );
	long Count = MemXMLList.GetCount();
	Count = max(Count,count_of(Pos) );
	for( int i = 0; i < Count; i++ )
	{
		MemPos& Ref = Pos[i];
		XMLPaser& RefXML = MemXMLList.GetItem(i);
		Ref.x = RefXML.AttributeFloat(TAG::X);
		Ref.y = RefXML.AttributeFloat(TAG::Y);
	}
	return true;
}

RbBattlePaty* ArmyFormation::AddPatys( const String& FormationName, const XMLPaser& PatyXML ) const
{
	std::map<String,ArmyPos>::const_iterator itr = ArmyPosList.find(FormationName);
	if( itr != ArmyPosList.end() )return AddPatys( System, itr->second, PatyXML );
	if( ArmyPosList.size() <= 0 )return NULL;
	return AddPatys( System, ArmyPosList.begin()->second, PatyXML );
}

const ArmyFormation::PatyForm* ArmyFormation::GetPatyForm( const String& Name ) const
{
	std::map<String,PatyForm>::const_iterator itr = PatyFormList.find(Name);
	if( itr == PatyFormList.end() )return NULL;
	return &(itr->second);
}

RbBattlePaty* ArmyFormation::AddPatys( BattleSystem& System, const ArmyPos& Form, const XMLPaser& PatyXML ) const
{
	const RbPosition& XMLPos = RbBattlePaty::GetEventPos( System, PatyXML, 0 );
	float x = XMLPos.x;
	float y = XMLPos.y;
	float Angle = XMLPos.z;

	std::list<PatyPos>::const_iterator itr = Form.PatyList.begin();
	while( itr != Form.PatyList.end() )
	{
		const PatyPos& Ref = *itr++;
		RbPointF Pos( Ref.x, Ref.y );
		Pos.Rotate( Angle );
		Pos += RbPointF(x,y);

		RefPtr<RbBattlePaty> pPaty( new RbBattlePaty );
		HRESULT hr = pPaty->SetPaty( Ref.PatyID, Pos.x, Pos.y, Ref.Angle+Angle );
		if( FAILED(hr) )continue;
		pPaty->SetState( PatyXML );
		System.AddPaty( pPaty );
	}

	return NULL;
}
