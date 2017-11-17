// RbStatus.cpp: RbStatus クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "RbCache.h"
#include "RbStatus.h"
#include "BattleSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbStatus::RbStatus()
{
	AllZero( SourceStatus );
	AllZero( LeaderAppend );
	AllZero( NowStatus );
	AllZero( AllStatus );
	CharactorHight = CharactorSize = 32;
	UpdateID = 0;

	NEW_CHECK(this);
}

RbStatus::~RbStatus()
{

}

String RbStatus::GetPasonName() const
{
	return RbMember(SourceStatus).PasonData().Name;
}

long RbStatus::KillUp()
{
	if( SourceStatus.Kill < 999999 )SourceStatus.Kill++;
	return SourceStatus.Kill;
}

void RbStatus::SetStatus( const MemberStatus& Source, const MemberStatus& Leader )
{
	SetLeaderAppend( Source.Status, Leader.Status );
	SetStatus( Source );

}


void RbStatus::SetLeaderAppend( const DefaultStatus& Source, const DefaultStatus& Leader )
{
	AllZero( LeaderAppend );
#define Append( name, x, y )	if( x.name > y.name ) LeaderAppend.name = (int)((x.name-y.name)/2.0f);
	Append( Hp, Leader, Source );
	Append( Mp, Leader, Source );
	Append( Attack, Leader, Source );
	Append( Defence, Leader, Source );
	Append( Magic, Leader, Source );
	Append( Speed, Leader, Source );
}

void RbStatus::SetStatus( const MemberStatus& Source )
{
	SourceStatus = Source;

	RbMember MyState( SourceStatus );

	pTexture.Release();
	bool bOK = GetCache().GetTexture( MyState.GetTexture(), pTexture );
	ATLASSERT( pTexture != NULL );

	if( pTexture != NULL )
	{
		CharactorSize = pTexture->GetSize();
		CharactorHight = pTexture->GetHight();
	}

	SkillList.SetMemberSkill( SourceStatus );
	DefaultStatus BaseStatus = Source.Status;
	BaseStatus *= SkillList.Effect.RefEffect().Pow;
	if( SkillList.Effect.RefEffect().SoulStrength > 0 && Source.Kill > 0 )
	{
		DefaultStatus AddStatus = Source.Status;
		AddStatus *= (SkillList.Effect.RefEffect().SoulStrength * ::powf(Source.Kill,BattleSystem::SoulRate));

		BaseStatus += AddStatus;
	}
	AllStatus = BaseStatus;
	AllStatus += LeaderAppend;

	SetItemStatus();
	NowStatus = AllStatus;

}

const ItemStatus& RbStatus::GetItemStatus( long Type ) const
{
	long ItemNum = 0;
	switch( Type )
	{
	default:
	case 0: ItemNum = SourceStatus.Wepon.Number; break;
	case 1: ItemNum = SourceStatus.Body.Number; break;
	case 2: ItemNum = SourceStatus.Acs.Number; break;
	}
	return GetCache().Item.Get( ItemNum );
}

void RbStatus::SetItemStatus()
{
	AllStatus += GetItemStatus( 0 ).Status;
	AllStatus += GetItemStatus( 1 ).Status;
	AllStatus += GetItemStatus( 2 ).Status;
}

long RbStatus::GetWeponID() const
{
	return SourceStatus.Wepon.Number;
}

void RbStatus::GetWeponImage( BillbordPlane& Output ) const
{
	RefPtr<RefBord> pImage = GetWeponImage2();
	if( pImage == NULL )return;
	Output = *pImage;
}

RefBord* RbStatus::GetWeponImage2() const
{
	return GetCache().Item.GetTexture( GetWeponID() );
}

const RbStatus::ActTime* RbStatus::GetNowImage() const
{
	if( pTexture == NULL )return NULL;

	return &(pTexture->GetAction( _T("image"), 1.25f, 0 ));
}

float RbStatus::HpJob( float AddHp )
{
	float Old = NowStatus.Hp;
	NowStatus.Hp += AddHp;
	if( NowStatus.Hp < 0 )NowStatus.Hp = 0;
	else if( NowStatus.Hp > AllStatus.Hp )NowStatus.Hp = AllStatus.Hp;
	if( Old != NowStatus.Hp )Update();
	return NowStatus.Hp;
}

float RbStatus::MpJob( float AddMp )
{
	float Old = NowStatus.Mp;
	NowStatus.Mp += AddMp;
	if( NowStatus.Mp < 0 )NowStatus.Mp = 0;
	else if( NowStatus.Mp > AllStatus.Mp )NowStatus.Mp = AllStatus.Mp;
	if( Old != NowStatus.Mp )Update();
	return NowStatus.Mp;
}

bool RbStatus::HpCheck( float Per ) const
{
	return (NowStatus.Hp<=(AllStatus.Hp*Per));
}

bool RbStatus::MpCheck( float Per ) const
{
	return (NowStatus.Mp<=(AllStatus.Mp*Per));
}
