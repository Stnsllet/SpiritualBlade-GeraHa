// RbBattlePaty.cpp: RbBattlePaty2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "RbPosition.h"
#include "RbMember.h"
#include "RbGameData.h"
#include "FomationSystem.h"
#include "BattleSystem.h"
#include "StringSplitter.h"
#include "RbBattlePaty.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

long RbBattlePaty::IDManage = 0;

RbBattlePaty::RbBattlePaty()
: System( GetBattleSystem() )
{
	FormationRange = BattleSystem::FormSize;
	State = RbActStatus::ePatyWait;
	AlgoType = eTacticsNone;
	Brave = 0.0f;
	MaxSize = 24.0f;
	LiveCount = 0;
	EscapeLife = 0;
	BattleID = ::InterlockedIncrement(&IDManage);

	NEW_CHECK(this);
}

RbBattlePaty::~RbBattlePaty()
{
	DropJob();
}

bool RbBattlePaty::DropJob()
{
	std::vector<RefPtr<RbActStatus> >::const_iterator itr = MemberList.begin();
	while( itr != MemberList.end() )
	{
		RbActStatus& Ref = *(*itr++);
		Ref.DropJob();
	}
	MemberList.clear();
	LiveCount = 0;
	return true;
}

bool RbBattlePaty::EscapePaty()
{
	ArmyStatus& ArmyState = System.ArmyData( Army );
	std::vector<RefPtr<RbActStatus> >::const_iterator itr = MemberList.begin();
	while( itr != MemberList.end() )
	{
		RefPtr<RbActStatus> pRef = *itr++;
		if( !pRef->IsDead() )ArmyState.Escape();
		pRef->DropJob();
	}
	MemberList.clear();
	LiveCount = 0;
	return true;
}

bool RbBattlePaty::IsDeleteOK() const
{
	if( LiveCount > 0 )return false;

	int Count = MemberList.size();
	for( int i = 0; i < Count; i++ )
		if( !MemberList[i]->IsDeleteOK() )return false;
	return true;
}

void RbBattlePaty::DeadCheck( long MemberID )
{
	if( pLeader != NULL )if( pLeader->ID()==MemberID )Brave = 0;
	LiveCheck();
}

void RbBattlePaty::LiveCheck()
{
	long OldCount = LiveCount;
	LiveCount = 0;
	int Count = MemberList.size();
	for( int i = 0; i < Count; i++ )if( !MemberList[i]->IsDead() )LiveCount++;
	System.ArmyData(Army).CountChange( LiveCount - OldCount );
}

float RbBattlePaty::BraveUp( float Change )
{
	if( Change == 0 )return Brave;
	RefPtr<RbActStatus> pLeader = GetLeader();
	if( pLeader != NULL )
	{
		const float& Effect = pLeader->LeaderEffect.BraveUp;
		if( Change < 0 )Change *= max(0.0f,(2-Effect)/2.0f);
		else Change *= (1+Effect);
	}
	return ChangeBrave( Change );
}

float RbBattlePaty::ChangeBrave( float Change )
{
	Brave = Trim( Brave+Change, 100.0f, 0.01f );
	return Brave;
}

bool RbBattlePaty::ChangeTactics( TacticsAlgoType NewTactics )
{
	RefPtr<RbActStatus> pLeader = GetLeader();
	if( pLeader == NULL )return false;
	pLeader->AlgoTactics.SetTactics( NewTactics );
	AlgoType = NewTactics;
	return true;
}

bool RbBattlePaty::ChangeState( PatyState NewState )
{
	if( State == NewState )return true;

	if( State == RbActStatus::ePatyBattle )
	{
		if( NewState == RbActStatus::ePatyMove )return false;
	}

	State = NewState;
	long Count = MemberList.size();
	for( int i= 0; i < Count; i++ )
	{
		MemberList[i]->ChangeState( NewState );
	}
	return true;
}

RbBattlePaty::PatyMemberData RbBattlePaty::GetMemberData( long MemberID ) const
{
	std::map<long,PatyMemberData>::const_iterator itr = MemberDataList.find( MemberID );
	if( itr != MemberDataList.end() )return itr->second;

	PatyMemberData Zero;
	AllZero( Zero );
	return Zero;
}

RbActStatus* RbBattlePaty::GetMember( long Index ) const
{
	if( Index < 0 || Index >= MemberList.size() )return NULL;
	return MemberList[Index];
}

RbActStatus* RbBattlePaty::AddMember( const MemberStatus& SourceStatus, float formX, float formY )
{
	RefPtr<RbActStatus> pPtr = System.AddCharactor( this, SourceStatus );
	if( pPtr == NULL )return NULL;

	if( pLeader == NULL )
	{
		ChangeBrave(pPtr->LeaderEffect.BraveUp * 10.0f);
		pLeader = pPtr;
	}
	MemberList.push_back( pPtr );

	long MemberID = pPtr->ID();
	ATLASSERT( MemberDataList.find(MemberID) == MemberDataList.end() );

	PatyMemberData MemberData;
	AllZero(MemberData);
	MemberData.formX = formX;
	MemberData.formY = formY;

	MemberDataList[MemberID] = MemberData;

	MemberStatus Chack = SourceStatus;
	RbMember CheckState(Chack);
	float CheckMax = CheckState.GetCharactorSize();
	if( CheckMax > MaxSize )MaxSize = CheckMax;

	return pPtr;
}

bool RbBattlePaty::SetState( const XMLPaser& PatyXML )
{
	static CComBSTR BRAVE = _T("brave");
	static CComBSTR ESCAPE = _T("escape");
	static CComBSTR TACTICS = _T("tactics");

	if( PatyXML.HasAttribute(BRAVE) )
		Brave += PatyXML.AttributeFloat(BRAVE);
	else Brave += 50.0f;
	if( PatyXML.HasAttribute(ESCAPE) )
		EscapeLife = PatyXML.AttributeFloat(ESCAPE)/100;

	String Tactics = PatyXML.AttributeString(TACTICS);
	if( !Tactics.empty() )
		AlgoType = TacticsAlgo::GetTactics( Tactics );

	return true;
}

HRESULT RbBattlePaty::SetPaty( long PatyNo, float x, float y, float angle )
{
	PatyStatus GetState;
	if( !GetGameData().GetPaty( PatyNo, GetState ) )return E_INVALIDARG;
	return SetPaty( GetState, x, y, angle );
}

HRESULT RbBattlePaty::SetPaty( const PatyStatus& Data, float x, float y, float angle )
{
	AlgoType = Data.Algo;
	EscapeLife = Data.EscapeHp / 100;

	MemberStatus GetStatus;

	MemberList.clear();
	Army = _T("user");
	ArmyID = System.GetArmyID( Army );

	for( int i = 0; i < count_of(Data.Member); i++ )
	{
		const PatyStatus::PatyMember& RefMem = Data.Member[i];
		if( RefMem.id.IsNone() )continue;
		if( !GetGameData().GetMember( RefMem.id, GetStatus ) )continue;

		AddMember( GetStatus, RefMem.x, RefMem.y );
	}
	LiveCheck();
	ResetPosition( x, y, angle);

	return (MemberList.size()>0)?(S_OK):(E_FAIL);
}

RbPosition RbBattlePaty::GetEventPos( const BattleSystem& System, const XMLPaser& PatyXML, long ArmyID )
{
	String ParamPos = PatyXML.AttributeString(TAG::PARAMPOS);
	if( ParamPos.empty() )
		return RbPosition( PatyXML.AttributeFloat(TAG::X),
		PatyXML.AttributeFloat(TAG::Y),
		PatyXML.AttributeFloat(TAG::DIR) );

	if( ParamPos == _T("rnd") )
	{
		RbPosition Pos = System.GetRndPosition( ArmyID, System.FormSize*3 );
		Pos.z = Rndf(2.0f);
		return Pos;
	}
	StringSplitter<> Split(ParamPos);
	return RbPosition( Split[0].ToFloat(),
		Split[1].ToFloat(),
		Split[2].ToFloat() );
}

HRESULT RbBattlePaty::SetPaty( const XMLPaser& PatyXML )
{
	float x = 0;
	float y = 0;
	float angle = 0;
	String ParamPos = PatyXML.AttributeString(TAG::PARAMPOS);

	Army = PatyXML.AttributeString(TAG::ARMY);
	ArmyID = System.GetArmyID( Army );
	bool bUserData = IsUserData();

	if( !ParamPos.empty() )
	{
		if( ParamPos == _T("rnd") )
		{
			const RbPosition& Pos = System.GetRndPosition( ArmyID, System.FormSize*3 );
			x = Pos.x;
			y = Pos.y;
			angle = Rndf(2.0f);
		}
		else
		{
			StringSplitter<> Split(ParamPos);
			x = Split[0].ToFloat();
			y = Split[1].ToFloat();
			angle = Split[2].ToFloat();
		}
	}
	else
	{
		x = PatyXML.AttributeFloat(TAG::X);
		y = PatyXML.AttributeFloat(TAG::Y);
		angle = PatyXML.AttributeFloat(TAG::DIR);
	}
	MaxSize = 24.0f;

	SetState( PatyXML );

	if( PatyXML.HasAttribute(TAG::NO) )
	{
		long PatyNo = 0;
		if( PatyXML.AttributeString(TAG::NO)==L"ex" )PatyNo = GetGameData().RefExPatyNum();
		else PatyNo = PatyXML.AttributeLong(TAG::NO);
		return SetPaty( PatyNo, x, y, angle );
	}

	XMLList MemberXMLList;
	HRESULT ret = MemberXMLList.Query( PatyXML, TAG::MEMBER );
	if( FAILED(ret) )return ret;

	FomationSystem Formation;
	String FormName = PatyXML.AttributeString(TAG::FORM);
	const ArmyFormation::PatyForm* pForm = System.Formation.GetPatyForm( FormName );

	int Count = MemberXMLList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ParamXML MemberXML;
		MemberXMLList.GetPaser( i, MemberXML );

		MemberStatus AddMemberSource;
		RbMember NewMember( AddMemberSource );
		if( !NewMember.ReadXML( MemberXML ) )return E_FAIL;
		if( bUserData )
			if( System.GetCharactor( NewMember.Ref.PasonName )!=NULL )
				continue;

		if( pForm != NULL && i < count_of(pForm->Pos) )
		{
			const ArmyFormation::PatyForm::MemPos& Ref = pForm->Pos[i];
			AddMember( NewMember, Ref.x, Ref.y );
			Formation.SetPosition( Ref.x, Ref.y );
		}else
		{
			FomationSystem::FormationData FData = Formation.GetPosition( NewMember.GetWeopnRange() );
			AddMember( NewMember, FData.x, FData.y );
		}
	}
	LiveCheck();
	ResetPosition( x, y, angle );

	return (MemberList.size()>0)?(S_OK):(E_FAIL);
}

void RbBattlePaty::ResetPosition( float x, float y, float Angle )
{
	RbPosition LeaderPos( x, y, 0 );
	int Count = MemberList.size();
	for( int i = 0; i < Count; i++ )
	{
		PatyMemberData Formation = GetMemberData( MemberList[i]->ID() );
		RbPointF FormPos( Formation.formX, Formation.formY );
		FormationRange = max( FormationRange,
			(FormPos.Length()*BattleSystem::FormSize)+MemberList[i]->Size()/2 );

		MemberList[i]->AlgoMove.SetPosition( LeaderPos, FormPos, Angle );
	}
}

bool RbBattlePaty::InFormation() const
{
	int Count = MemberList.size();
	for( int i = 0; i < Count; i++ )
	{
		if( !MemberList[i]->AlgoMove.InFormation() )return false;
	}
	return true;
}
