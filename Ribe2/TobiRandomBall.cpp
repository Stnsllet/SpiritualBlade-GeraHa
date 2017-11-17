// TobiRandomBall.cpp: TobiRandomBall クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBall.h"
#include "TobiRandomBall.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiRandomBall::TobiRandomBall()
{
	GenerateTime = GenerateInterval = 100;
}

TobiRandomBall::~TobiRandomBall()
{

}

bool TobiRandomBall::Generate()
{
	TobiRefPtr<TobiBall> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
