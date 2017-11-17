// TobiRandom.cpp: TobiRandom クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiCreator.h"
#include "TobiRandom.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiRandom::RndList TobiRandom::CreateList;

bool TobiRandom::RndList::Init( BattleSystem& System )
{
	static LPCTSTR DefNames[]={
		"f","w","i","b","d","t","s"
	};
	static LPCTSTR RareNames[]={
		"magswd","fenix","comet","roket","ball","bwave","feather"
	};

	if( Default.size()>0 )return false;

	int Count = count_of(DefNames);
	Default.reserve( Count );
	for( int i = 0; i < Count; i++ )
	{
		TobiCreator* pCreator = System.TobiSys.GetCreator( DefNames[i] );
		if( pCreator != NULL )Default.push_back( pCreator );
	}

	Count = count_of(RareNames);
	Rare.reserve( Count );
	for( i = 0; i < Count; i++ )
	{
		TobiCreator* pCreator = System.TobiSys.GetCreator( RareNames[i] );
		if( pCreator != NULL )Rare.push_back( pCreator);
	}

	return true;
}

TobiObjectBase* TobiRandom::RndList::Create() const
{
	if( Default.size() <= 0 )return NULL;
	if( Rnd(20) <= 0 )return Rare[Rnd(Rare.size()-1)]->Create();
	return Default[Rnd(Default.size()-1)]->Create();
}

TobiRandom::TobiRandom()
{
}

TobiRandom::~TobiRandom()
{
}

// 仕事
bool TobiRandom::Job( BattleSystem& System )
{
	return false;
}

bool TobiRandom::SetResource()
{
	CreateList.Init( System );
	return true;
}

// スキル効果設定
bool TobiRandom::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	return true;
}

// データセット
bool TobiRandom::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	RefPtr<TobiObjectBase> pBit = CreateList.Create();
	if( pBit == NULL )return false;
	pBit->SetEffect( MyEffect );
	if( !pBit->SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;
	System.TobiSys.Add( pBit );

	return true;
}
