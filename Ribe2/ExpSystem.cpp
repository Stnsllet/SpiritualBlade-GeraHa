// ExpSystem.cpp: ExpSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExpSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

long ExpSystem::NextCount = 50;

ExpSystem::ExpSystem( MemberStatus& ref, int Max ) : Status(ref)
{
	MaxLevel = Max;
}

// 経験値取得(戻り値はレベルアップ数)
long ExpSystem::AddExp( const MemberStatus& Enemy, float UpScale, bool bDead )
{
	if( Status.Level >= MaxLevel )return 0;
	long Add = Exp( Status.Level, Enemy.Level ) * UpScale;
	if( bDead )Add *= 3;

	Status.Exp += Add;

	long LevelUpCount = 0;
	while( Status.Exp > NextExp(Status.Level) )
	{
		Status.Exp -= NextExp(Status.Level);
	//	Status.Level++;
		LevelUpCount++;
	}

	return LevelUpCount;
}

// 取得経験値
long ExpSystem::Exp( long MyLev, long EneLev ) const
{
	long GetExp = EneLev + 4;
	if( MyLev <= EneLev )return GetExp;

	const long Diff = 18*(MyLev - EneLev)-3;
	GetExp *= 10;
	GetExp /= Diff;
	return GetExp;
}

// 次のレベルアップ経験値
long ExpSystem::NextExp( long MyLev ) const
{
	return (MyLev)*NextCount;
}
