// FomationSystem.cpp: FomationSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusStruct.h"
#include "RbPaty.h"
#include "FomationSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/*
const FomationSystem::FormationData FomationSystem::LongDefault[] =
{
	{ 0, 0 },	{ -.5, 1 },	{  .5, 1 },	{ 0, 2 },	{ -1.5, 1 },
	{ 1.5, 1 },	{ -1, 2 },	{ 1, 2 },	{ -2, 2 },	{ 2, 2 },
};
const FomationSystem::FormationData FomationSystem::ShortDefault[] =
{
	{ 0, 0 },	{ -1, 0 },	{  1, 0 },	{ -.5, -1 },	{ .5, -1 },
	{ 0, -2 },	{ -1.5, -1 },	{ 1.5, -1 },	{ -1, -2 },	{ 1, -2 },
};
*/

const FomationSystem::FormationData FomationSystem::AllForm[] =
{
	{-1,-2},	{0,-2},		{1,-2},
	{-1.5f,-1},	{-.5f,-1},	{.5f,-1},	{1.5f,-1},
	{-2,0},		{-1,0},		{0,0},		{1,0},	{2,0},
	{-1.5f,1},	{-.5f,1},	{.5f,1},	{1.5f,1},
	{-1,2},		{0,2},		{1,2},
};
const FomationSystem::FormationData FomationSystem::LongDefault[] =
{
	{ 0, 0 },	{ -1, 0 },	{  1, 0 },	{ -.5, 1 },	{  .5, 1 },
	{ 0, 2 },	{ -1.5, 1 },{ 1.5, 1 },	{ -1, 2 },	{ 1, 2 },
};
const FomationSystem::FormationData FomationSystem::ShortDefault[] =
{
	{ 0, 0 },	{ -.5, -1 },{ .5, -1 },	{ -1.5, -1 },{ 1.5, -1 },
	{ 0, -2 },	{ -1, -2 },	{ 1, -2 },	{ -2, -2 },	{ 2, -2 },
};

long FomationSystem::GetAllFormCount()
{
	return count_of(AllForm);
}

FomationSystem::FomationSystem()
{
}

FomationSystem::~FomationSystem()
{

}

void FomationSystem::SetPosition( float x, float y )
{
	FormationData Add;
	Add.x = x; Add.y = y;
	NowDataSet.push_back( Add );
}

void FomationSystem::SetPosition( const RbPaty& Ref )
{
	long MemberCount = Ref.MemberCount();
	for( int i = 0; i < MemberCount; i++ )
	{
		const PatyStatus::PatyMember& RefMember = Ref.Status().Member[i];
		SetPosition( RefMember.x, RefMember.y );
	}
}

bool FomationSystem::CheckFormation( const FormationData& Ref ) const
{
	std::list<FormationData>::const_iterator itr = NowDataSet.begin();
	while( itr != NowDataSet.end() )if( *(itr++) == Ref )return true;
	return false;
}

FomationSystem::FormationData FomationSystem::GetPosition( float Range )
{
	FormationData Ret;
	AllZero( Ret );

	bool bLongRange = false;
	if( Range >= 100.0f )bLongRange = true;

	if( bLongRange )
	{
		for( int i = 0; i < count_of(LongDefault); i++ )
		{
			Ret = LongDefault[i];
			if( CheckFormation( Ret ) )continue;
			SetPosition( Ret.x, Ret.y );
			return Ret;
		}
		return Ret;
	}
	else
	{
		for( int i = 0; i < count_of(ShortDefault); i++ )
		{
			Ret = ShortDefault[i];
			if( CheckFormation( Ret ) )continue;
			SetPosition( Ret.x, Ret.y );
			return Ret;
		}
		return Ret;
	}
	return Ret;
}
