// RbPaty.cpp: RbPaty クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "RbPaty.h"
#include "RbGameData.h"
#include "FomationSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbPaty::RbPaty( const PatyStatus& Source )
: RefPaty( const_cast<PatyStatus&>(Source) ), MaxMember( count_of(Source.Member) )
{

}

RbPaty::RbPaty( PatyStatus& Source )
: RefPaty( Source ), MaxMember( count_of(Source.Member) )
{

}

RbPaty::~RbPaty()
{
}

float RbPaty::SetEscapeHp( float NewData )
{
	RefPaty.EscapeHp = Trim( NewData, 100.0f, 0.0f );
	return RefPaty.EscapeHp;
}


bool RbPaty::SetTactics( TacticsAlgoType NewData )
{
	if( NewData < 0 || NewData >= eTacticsMax )return false;
	RefPaty.Algo = NewData;
	return true;
}

bool RbPaty::AddOK() const
{
	return RefPaty.Member[MaxMember-1].id.IsNone();
}

bool RbPaty::Create()
{
	AllZero(RefPaty);
	RefPaty.EscapeHp = 40;
	return true;
}

long RbPaty::MemberCount() const
{
	for( long i = 0; i < MaxMember; i++ )
	{
		if( RefPaty.Member[i].id.IsNone() )return i;
	}
	return MaxMember;
}

bool RbPaty::IsMaxMember() const
{
	return (MemberCount()>=MaxMember);
}

bool RbPaty::InMember( LPCTSTR Name ) const
{
	return (FindMember(Name)>=0);
}

long RbPaty::FindMember( LPCTSTR Name ) const
{
	if( Name == NULL )return -1;
	for( long i = 0; i < MaxMember; i++ )
	{
		if( RefPaty.Member[i].id == Name )return i;
	}
	return -1;
}

bool RbPaty::DropMember( LPCTSTR Name )
{
	long Index = FindMember( Name );
	if( Index < 0 )return false;
	for( long i = Index; i < MaxMember-1; i++ )
	{
		RefPaty.Member[i] = RefPaty.Member[i+1];
	}
	AllZero( RefPaty.Member[MaxMember-1].id );
	RefPaty.Member[0].x = RefPaty.Member[0].y = 0;
	return true;
}

void RbPaty::Insert( long Index, const PatyStatus::PatyMember& Data )
{
	if( Index < 0 )return;
	for( long i = MaxMember-1; i > Index; i-- )
	{
		RefPaty.Member[i] = RefPaty.Member[i-1];
	}
	RefPaty.Member[Index] = Data;
	RefPaty.Member[0].x = RefPaty.Member[0].y = 0;
}

bool RbPaty::AddMember( const MemberStatus& AddMember, long Index )
{
	if( AddMember.PasonName.IsNone() )return false;
	if( InMember( AddMember.PasonName ) )return ChangeIndex( AddMember.PasonName, Index );
	long Count = MemberCount();
	if( Count >= MaxMember )return false;
	if( InMember( AddMember.PasonName ) )return false;
	if( Index < 0 || Index >= Count )Index = Count;

	PatyStatus::PatyMember NewData;
	ResetFormation( AddMember, NewData );
	Insert( Index, NewData );
	return true;
}

bool RbPaty::ChangeIndex( LPCTSTR Name, long Index )
{
	long OldIndex = FindMember( Name );
	if( OldIndex < 0 )return false;
	PatyStatus::PatyMember OldData = RefPaty.Member[OldIndex];
	DropMember( Name );

	long Count = MemberCount();
	if( Index < 0 || Index >= Count )Index = Count;

	Insert( Index, OldData );

	return true;
}

bool RbPaty::ResetFormation( const MemberStatus& Member, PatyStatus::PatyMember& Data )
{
	RbMember Check( Member );

	FomationSystem Form;
	Form.SetPosition( *this );
	FomationSystem::FormationData Formation = Form.GetPosition( Check.GetWeopnRange() );

	Data.id = Member.PasonName;
	Data.x = Formation.x;
	Data.y = Formation.y;
	return true;
}

bool RbPaty::ResetFormation( const RbGameData& UserData )
{
	FomationSystem Form;
	Form.SetPosition( *this );

	bool bRet = false;
	for( int i = MaxMember-1; i >= 1; i-- )
	{
		PatyStatus::PatyMember& Ref = RefPaty.Member[i];
		if( Ref.id.IsNone() )continue;
		for( int j = 0; j < i; j++ )
		{
			const PatyStatus::PatyMember& RefOther = RefPaty.Member[j];
			if( RefOther.id.IsNone() )continue;
			if( Ref.x != RefOther.x || Ref.y != RefOther.y )continue;
			float WeponRange = GetWeponRange( Ref.id, UserData );
			FomationSystem::FormationData Formation = Form.GetPosition( WeponRange );
			Ref.x = Formation.x;
			Ref.y = Formation.y;
			bRet = true;
			break;
		}
	}
	return bRet;
}

float RbPaty::GetWeponRange( LPCTSTR Member, const RbGameData& UserData ) const
{
	MemberStatus Mem;
	if( !UserData.GetMember( Member, Mem ) )return 0;
	RbMember Dat(Mem);
	return Dat.GetWeopnRange();
}
