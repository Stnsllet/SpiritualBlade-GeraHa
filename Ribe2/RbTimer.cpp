// RbTimer.cpp: RbTimer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timer.h"
#include "GameSystem.h"
#include "RbTimer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbMasterTimer::RbTimer::RbTimer()
{
	ATLASSERT( g_pSystem );
	NowTime = g_pSystem->TimeSystem;
}

double RbMasterTimer::RbTimer::Passed( double OldTime ) const
{
	return NowTime - OldTime;
}

RbMasterTimer::RbMasterTimer()
{
	NowTime = 0;
	OldTime = Timer();
}

void RbMasterTimer::Sleep()
{
	OldTime = Timer();
}

void RbMasterTimer::Check()
{
	Timer New;
	NowTime += New.Passed( OldTime );
	OldTime = New;
}
