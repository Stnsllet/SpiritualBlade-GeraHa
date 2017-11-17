// WeponSystem.cpp: WeponSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "BattleSystem.h"
#include "TobiSystem.h"
#include "WeponSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float WeponSystem::WeponData::UseMp( long Level ) const
{
	if( pCreator == NULL )return 0;
	return pCreator->UseMp( Level );
}

bool WeponSystem::WeponData::CheckMp( const RbActStatus& Source ) const
{
	return (Source.NowState().Mp>=UseMp(Source.Level()));
}

bool WeponSystem::WeponData::IsAttackOK( RbBattleStatus* pTaget, const RbActStatus& Source ) const
{
	if( !InAttackRange( pTaget, Source ) )return false;
	return CheckMp( Source );
}

bool WeponSystem::WeponData::InAttackRange( RbBattleStatus* pTaget, const RbActStatus& Source ) const
{
	if( pTaget == NULL )return false;
	if( pTaget->IsDead() )return false;
	return pTaget->InRange( Source.GetPosition(), AttackRange );
}

bool WeponSystem::WeponData::InTagetRange( RbBattleStatus* pTaget, const RbActStatus& Source ) const
{
	if( pTaget == NULL )return false;
	if( pTaget->IsDead() )return false;
	return pTaget->InRange( Source.GetPosition(), TagetRange );
}

// 武器の設定
bool WeponSystem::WeponData::SetData( long ItemNum, const RbActStatus& Source, long* pChainWepon, bool bWepon )
{
	RbCache& Cache = GetCache();
	const ItemStatus& Item = Cache.Item.Get( ItemNum );
	const TobiSystem& TobiSys = GetBattleSystem().TobiSys;

	bool SubWepon = Item.Ex.IsFirst(_T("sw"));
	bool bZeroWepon = bWepon; //&& (Item.id==0);
	ItemNumber = ItemNum;
	if( bZeroWepon || Item.id.EquipType == 1 )
	{
		ActID = Cache.ActID.GetID( Item.Type );
	}
	else if( !SubWepon )return false;
	if( SubWepon )
	{
		StringSplitter<> Split;
		Split.SetString( Item.Ex );
		ActID = Cache.ActID.GetID( Split[1] );
	}
	if( !Source.IsTexAct( ActID ) )ActID = 0;
	pImage = Cache.Item.GetTexture( ItemNum );

	AttackRange = TagetRange = Source.Size()/2;
	float Wepon = Item.Effect.Range * Source.LeaderEffect.RangeUp;
	AttackRange += Wepon;
	TagetRange += (Wepon*0.66f);

	Effect = Item.Effect;
//	Effect.Pow = 1.0f;
	Effect.Range = AttackRange;
//	Effect.Count = 1;

	if( !Item.MagicType.empty() )pCreator = TobiSys.GetCreator( Item.MagicType );

	if( Item.Ex.IsFirst(_T("ch")) )
	{
		StringSplitter<> Split( Item.Ex );
		long ItemNum = Split[1].ToLong();
		if( ItemNum > 0 && pChainWepon!=NULL )*pChainWepon = ItemNum;
	}
	return true;
}

WeponSystem::WeponList::WeponList()
: pNowWepon(NULL)
{
}

bool WeponSystem::WeponList::Add( long ItemID, const RbActStatus& Source, bool bWepon )
{
	long ChainWep = 0;
	WeponSystem::WeponData Wep;
	if( !Wep.SetData( ItemID, Source, &ChainWep,  bWepon ) )return false;

	if( ChainWep > 0 && ItemID != ChainWep && Wepons.size() < 16 )
		Add( ChainWep, Source, false );
	return Add( Wep );
}

bool WeponSystem::WeponList::Add( const WeponSystem::WeponData& NewData )
{
	std::list<WeponData>::iterator Itr = Wepons.begin();
	for( ;Itr != Wepons.end(); Itr++ )
	{
		WeponSystem::WeponData& Ref = *Itr;
		// 射程短いほう優先
		if( NewData.AttackRange < Ref.AttackRange )break;
		// 射程が近い？
		if( (long)(NewData.AttackRange/10) != (long)(Ref.AttackRange/10) )continue;
		// 消費MP大きいほう優先
		if( NewData.UseMp(10) > Ref.UseMp(10) )break;
	}
	Itr = Wepons.insert( Itr, NewData );
	WeponData& Ref = (*Itr);
	if( Wepons.size() == 1 )pNowWepon = &Ref;
	else if( Ref.pImage == NULL || Ref.pImage->pTexture==NULL )
	{
		if( pNowWepon != NULL )Ref.pImage = pNowWepon->pImage;
	}
	if( pBaseImage == NULL )pBaseImage = NewData.pImage;
	else if( Ref.pImage == NULL )Ref.pImage = pBaseImage;
	return true;
}

const WeponSystem::WeponData& WeponSystem::WeponList::Now() const
{
	return *pNowWepon;
}

bool WeponSystem::WeponList::RandomWepon( RbActStatus& Source )
{
	if( Wepons.size() <= 1 )return true;
	long Number = Rnd(Wepons.size()-1);
	std::list<WeponData>::iterator Itr = Wepons.begin();
	while( (Number--)>0 )++Itr;
	if( !Itr->CheckMp(Source) )return ChangeWepon( Source.TagetSys.GetMinEnemy(), Source );
	pNowWepon = &(*Itr);
	return true;
}

bool WeponSystem::WeponList::ChangeWepon( RbActStatus* pTaget, const RbActStatus& Source )
{
	if( pTaget == NULL )return false;
	std::list<WeponData>::iterator Itr = Wepons.begin();
	WeponData* pWide = pNowWepon;
	for( ;Itr != Wepons.end(); Itr++ )
	{
		WeponSystem::WeponData& Ref = *Itr;
		if( !Ref.CheckMp(Source) )continue;
		pWide = &Ref;
		if(!Ref.InAttackRange(pTaget,Source) )continue;
		pNowWepon = &Ref;
		return true;
	}
	pNowWepon = pWide;
	return false;
}

bool WeponSystem::WeponList::SetWideWepon( const RbActStatus& Source )
{
	std::list<WeponData>::reverse_iterator Itr = Wepons.rbegin();
	for( ;Itr != Wepons.rend(); Itr++ )
	{
		WeponSystem::WeponData& Ref = *Itr;
		if(!Ref.CheckMp(Source) )continue;
		pNowWepon = &Ref;
		return true;
	}
	return false;
}
