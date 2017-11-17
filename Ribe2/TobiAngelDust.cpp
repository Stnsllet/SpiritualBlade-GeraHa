// TobiAngelDust.cpp: TobiAngelDust クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiFeather.h"
#include "TobiAngelDust.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiAngelDust::TobiAngelDust()
{
	GenerateTime = GenerateInterval = 200;
}

TobiAngelDust::~TobiAngelDust()
{
}

bool TobiAngelDust::Generate()
{
	TobiRefPtr<TobiFeather> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
