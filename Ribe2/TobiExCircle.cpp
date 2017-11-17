// TobiExCircle.cpp: TobiExCircle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExCircleBit.h"
#include "TobiExCircle.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExCircle::TobiExCircle()
{
}

TobiExCircle::~TobiExCircle()
{
}

bool TobiExCircle::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	pExDef = pNewDef;
	return true;
}

TobiFrameCircleBit* TobiExCircle::CreateFlameObject() const
{
	TobiExCircleBit* pBit = new TobiExCircleBit;
	pBit->SetExDefine( pExDef );
	return pBit;
}
