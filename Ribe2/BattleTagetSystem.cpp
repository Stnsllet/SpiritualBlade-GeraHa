// BattleTagetSystem.cpp: BattleTagetSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AngleChecker.h"
#include "PaformanceChecker.h"
#include "BattleTagetSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BattleTagetSystem::BattleTagetSystem()
{
}

BattleTagetSystem::~BattleTagetSystem()
{
}

// 敵味方の数を数える
long BattleTagetSystem::Count( const D3DVECTOR& Pos, float Range, long ArmyID, long* pFriend ) const
{
	CharaList GetList;
	GetCharaList( GetList, Pos, Range, ArmyID, true, true );
	return Count( GetList, ArmyID, pFriend );
}

// 敵味方の数を数える
long BattleTagetSystem::Count( const CharaList& List, long ArmyID, long* pFriend ) const
{
	long Enemy = 0;
	long Friend = 0;
	CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		if( ArmyID == (*itr)->GetArmyID() )Friend++;
		else Enemy++;
		itr++;
	}
	if( pFriend != NULL )(*pFriend) = Friend;
	return Enemy;
}

//	近い味方を取得
RbActStatus* BattleTagetSystem::GetUnHelpFriendPaty( const RbBattleStatus& Source ) const
{
	return GetFriendPaty( Source.GetPosition(), Source.GetArmyID(), RbActStatus::ePatyNoState, NULL, Source.GetPaty(), true );
}

//	近い味方を取得
RbActStatus* BattleTagetSystem::GetFriendPaty( const RbBattleStatus& Source, PatyState CheckState, float* pRange ) const
{
	return GetFriendPaty( Source.GetPosition(), Source.GetArmyID(), CheckState, pRange, Source.GetPaty(), false );
}

//	近い味方を取得
RbActStatus* BattleTagetSystem::GetFriendPaty( const RbPosition& Pos, long ArmyID, PatyState CheckState, float* pRange, RbBattlePaty* pCheckPaty, bool bUnHelpPaty ) const
{
	float MinLength = (1<<30);

	long MyPatyID = 0;
	if( pCheckPaty != NULL )MyPatyID = pCheckPaty->ID();

	RefPtr<RbActStatus> pRet;

	BPatyMap::const_iterator itr = BattlePatys.begin();
	while( itr != BattlePatys.end() )
	{
		const RbBattlePaty& Paty = *((itr++)->second);
		if( ArmyID != Paty.GetArmyID() )continue;
		if( MyPatyID == Paty.ID() && pCheckPaty != NULL )continue;
		if( CheckState != RbActStatus::ePatyNoState )
			if( Paty.GetPatyStatus() != CheckState )continue;
		RefPtr<RbActStatus> pLeader = Paty.GetLeader();
		if( pLeader == NULL )continue;
		if( pLeader->IsDead() )continue;
		if( bUnHelpPaty )
			if( pLeader->AlgoTactics.RefTactics() == eTacticsHelp )continue;

		float Length2 = Pos.Length2(pLeader->GetPosition());
		if( Length2 >= MinLength )continue;
		MinLength = Length2;
		pRet = pLeader;
	}
	if( pRange != NULL )*pRange = ::sqrtf(MinLength);
	return pRet;
}

//	近い敵を取得
RbActStatus* BattleTagetSystem::GetEnemy(  const RbBattleStatus& Source, float* pRange ) const
{
	return GetEnemy( Source.GetPosition(), Source.GetArmyID(), pRange );
}

//	近い敵を取得
RbActStatus* BattleTagetSystem::GetEnemy( const RbPosition& Pos, long ArmyID, float* pRange ) const
{
	float MinLength = (1<<30);

	RefPtr<RbActStatus> pRet;

	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		RbActStatus& State = *(*itr);
		if( ArmyID == State.GetArmyID() )continue;
		if( State.IsDead() )continue;
		float Length = Pos.Length2( State.GetPosition() );
		if( Length >= MinLength )continue;

		pRet = &State;
		MinLength = Length;
	}
	if( pRange != NULL )*pRange = ::sqrt(MinLength);
	return pRet;
}

//	見える近い敵を取得
RbActStatus* BattleTagetSystem::FindEnemy(  const RbBattleStatus& Source, float* pRange ) const
{
	return FindEnemy( Source.GetPosition(), Source.GetArmyID(), pRange );
}

//	近い敵を取得
RbActStatus* BattleTagetSystem::FindEnemy( const RbPosition& Pos, long ArmyID, float* pRange ) const
{
	float MinLength = (1<<30);

	RefPtr<RbActStatus> pRet;

	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		RbActStatus& State = *(*itr);
		if( ArmyID == State.GetArmyID() )continue;
		if( State.IsDead() )continue;
		if( State.AlgoMove.IsUnbush() )continue;
		float Length = Pos.Length2( State.GetPosition() );
		if( Length >= MinLength )continue;

		pRet = &State;
		MinLength = Length;
	}
	if( pRange != NULL )*pRange = ::sqrt(MinLength);
	return pRet;
}

// リーダーを数える
long BattleTagetSystem::LeaderCount( long ArmyID ) const
{
	long Count = 0;
	BPatyMap::const_iterator itr = BattlePatys.begin();
	while( itr != BattlePatys.end() )
	{
		const RbBattlePaty& Paty = *((itr++)->second);
		RefPtr<RbActStatus> pLeader = Paty.GetLeader();
		if( ArmyID != Paty.GetArmyID() )continue;
		if( pLeader == NULL )continue;
		if( pLeader->IsDead() )continue;
		Count++;
	}
	return Count;
}

// 一定範囲内のリーダーを数える
long BattleTagetSystem::LeaderCount( RbActStatus* pBase, float Range, bool bEnemy ) const
{
	if( pBase == NULL )return 0;
	return LeaderCount( pBase->GetPosition(), pBase->GetArmyID(), Range, bEnemy );
}

// 一定範囲内のリーダーを数える
long BattleTagetSystem::LeaderCount( const RbPosition& BasePos, long ArmyID, float Range, bool bEnemy ) const
{
	long Count = 0;
	Range = Range*Range;

	BPatyMap::const_iterator itr = BattlePatys.begin();
	for( ;itr != BattlePatys.end();itr++ )
	{
		const RbBattlePaty& Paty = *(itr->second);
		if( !(Paty.IsAlivePaty()) )continue;
		long TagetArmyID = Paty.GetArmyID();
		if( bEnemy )if( ArmyID == TagetArmyID)continue;
		if( !bEnemy )if( ArmyID != TagetArmyID)continue;

		RefPtr<RbActStatus> pLeader = Paty.GetLeader();
		if( pLeader == NULL )continue;
		if( BasePos.Length2( pLeader->GetPosition() ) <= Range )Count++;
	}
	return Count;
}

// 射程内のキャラからランダム選出
RbActStatus* BattleTagetSystem::GetRndEnemy( const RbActStatus& Source ) const
{
	PEFORMANCE("GetRndEnemy");
	long ArmyID = Source.GetArmyID();
	CharaList EnemyList;
	long Count = 0;
	bool bTaget = false;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); ++itr )
	{
		RbActStatus& State = *(*itr);
		if( State.IsDead() )continue;
		if( ArmyID == State.GetArmyID() )continue;
		if( bTaget )
		{
			if( !Source.AttackAlgo.InTagetRange( &State ) )continue;
		}
		else
		{
			if( Source.AttackAlgo.InTagetRange( &State ) )
			{
				bTaget = true;
				EnemyList.clear();
				Count = 0;
			}
			else if( !Source.AttackAlgo.InAttackRange( &State ) )continue;
		}

		EnemyList.push_back( &State );
		Count++;
	}
	if( Count <= 0 )return NULL;
	long RndNum = Rnd(Count-1);
	CharaList::const_iterator Eneitr = EnemyList.begin();
	while( RndNum-- )Eneitr++;
	return (*Eneitr);
}

// 射程内のキャラからランダム選出
RbActStatus* BattleTagetSystem::GetRndEnemy( const RbPosition& Pos, long ArmyID, float Range ) const
{
	PEFORMANCE("GetRndEnemy2");
	CharaList EnemyList;
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); ++itr )
	{
		RbActStatus& State = *(*itr);
		if( State.IsDead() )continue;
		if( ArmyID == State.GetArmyID() )continue;
		if( !State.InRange( Pos, Range ) )continue;

		EnemyList.push_back( &State );
		Count++;
	}
	if( Count <= 0 )return NULL;
	long RndNum = Rnd(Count-1);
	CharaList::const_iterator Eneitr = EnemyList.begin();
	while( RndNum-- )Eneitr++;
	return (*Eneitr);
}

// 一定範囲内のキャラ取得
long BattleTagetSystem::GetCharaList( CharaList& Output, const RbPosition& Pos, float Range, long ArmyID, bool bEnemy, bool bAll ) const
{
	PEFORMANCE("GetCharaList");
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); ++itr )
	{
		RbActStatus& State = *(*itr);
		if( State.IsDead() )continue;
		long TagetArmyID = State.GetArmyID();
		if( !bAll )
		{
			if( bEnemy && ArmyID == TagetArmyID )continue;
			else if( !bEnemy && ArmyID != TagetArmyID )continue;
		}
		if(!State.InRange( Pos, Range ) )continue;

		Output.push_back( &State );
		++Count;
	}
	return Count;
}

// 一定範囲内のキャラ取得
long BattleTagetSystem::ReCallTaget( MemCallBack* pCallBack, const RbPosition& Pos, float Range, long ArmyID, bool bEnemy, bool bAll ) const
{
	if( pCallBack == 0 )return 0;
	PEFORMANCE("ReCallTaget");
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); ++itr )
	{
		RbActStatus& State = *(*itr);
		if( State.IsDead() )continue;
		long TagetArmyID = State.GetArmyID();
		if( !bAll )
		{
			if( bEnemy && ArmyID == TagetArmyID )continue;
			else if( !bEnemy && ArmyID != TagetArmyID )continue;
		}
		if(!State.InRange( Pos, Range ) )continue;
		if( pCallBack->Call( State ) )++Count;
	}
	return Count;
}

// 範囲内のキャラ数チェック
long BattleTagetSystem::GetCharaCount( const RbPosition& Pos, float Range, long ArmyID, bool bEnemy, bool bAll ) const
{
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		RbActStatus& State = *(*itr);
		if( State.IsDead() )continue;
		long TagetArmyID = State.GetArmyID();
		if( !bAll )
		{
			if( bEnemy && ArmyID == TagetArmyID )continue;
			else if( !bEnemy && ArmyID != TagetArmyID )continue;
		}
		if(!State.InRange( Pos, Range ) )continue;
		Count++;
	}
	return Count;
}

// 円形攻撃の適切な位置を求める
RbPosition BattleTagetSystem::CircleTaget( const D3DVECTOR& Pos, float BurstRange, float CheckRange, long ArmyID, float FirstAngle ) const
{
	if( CheckRange <= 0 )return Pos;

	RbPosition VectorPos( 0, -1, 0 );
	VectorPos.Rotate( FirstAngle );
	VectorPos *= CheckRange;

	RbPosition RetPos = VectorPos+Pos;
	long Friend = 0;
	long Enemy = Count( RetPos, BurstRange, ArmyID, &Friend );

	for( int i = 2; i <= 11; i ++ )
	{
		int a = i/2;
		float Angle = a / 6.0f *((i&1)*2-1);
		RbPosition TagetPos = VectorPos;
		TagetPos.Rotate( Angle );
		TagetPos += Pos;
		long NewFriend = 0;
		long NewEnemy = Count( TagetPos, BurstRange, ArmyID, &NewFriend );
		if( NewFriend > Friend )continue;
		if( NewFriend < Friend || NewEnemy > Enemy )
		{
			RetPos = TagetPos;
			Friend = NewFriend;
			Enemy = NewEnemy;
		}
	}

	return RetPos;
}

// 全体に対する位置(0:中央 1:右翼 -1:左翼)
float BattleTagetSystem::GetArmyPos( const RbPosition& BasePos, const RbPosition& TagetPos, long ArmyID) const
{
	long AllCount = 0;
	long LeftCount = 0;
	long RightCount = 0;
	const float XRate = TagetPos.y - BasePos.y;
	const float YRate = TagetPos.x - BasePos.x;

	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		RbActStatus& State = *(*itr);
		if( !State.RefBattleStatus().bLeader )continue;
		if( State.IsDead() )continue;
		if( ArmyID != State.GetArmyID() )continue;

		++AllCount;
		const RbPosition& RefPos = State.GetPosition();
		if( BasePos == RefPos )continue;

		const float x = RefPos.x - BasePos.x;
		const float y = RefPos.y - BasePos.y;

		if( XRate*x > YRate*y )++RightCount;
		else ++LeftCount;
	}

	if( AllCount < 3 )return 0;
//	const float CenterCount = (AllCount*2.0f)/3.0f;
//	if( RightCount >= CenterCount )return 1;
//	if( LeftCount >= CenterCount )return -1;
	return (RightCount-LeftCount)/(float)(AllCount);
}

// キャラの多い方向を得る
float BattleTagetSystem::GetDirection( const CharaList& List, const RbPosition& BasePos, float Dips, float Width, float StartAngle ) const
{
	if( Dips <= 0 )return StartAngle;
	if( Width <= 0 )Width = Dips;
	long AngleCount = (long)((2 / Dips)+0.5f);
	if( AngleCount <= 0 )return StartAngle;

	std::vector<AngleChecker::Counter> Checks;
	Checks.resize( AngleCount );
	for( int i = 0; i < AngleCount; i++ )
		Checks[i].Set( StartAngle+(Dips*i), Width );

	CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		const RbActStatus& Ref = *(*itr++);
		float Angle = BasePos.GetDirection( Ref.GetPosition() );
		for( int i = 0; i < AngleCount; i++ )
			Checks[i].Check( Angle );
	}
	float RetAngle = Checks[0].MyAngle;
	long MaxCount = Checks[0].Count;
	for( i = 1; i < AngleCount; i++ )
	{
		const AngleChecker::Counter& Ref = Checks[i];
		if( Ref.Count > MaxCount )
		{
			MaxCount = Ref.Count;
			RetAngle = Ref.MyAngle;
		}
	}
	return RetAngle;
}
