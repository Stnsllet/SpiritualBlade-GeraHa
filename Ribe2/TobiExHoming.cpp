// TobiExHoming.cpp: TobiExHoming クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExHoming.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExHoming::TobiExHoming()
{
}

TobiExHoming::~TobiExHoming()
{

}

bool TobiExHoming::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	SetPolygon( pNewDef->PolyID );
	StartSoundNum = pNewDef->WaveID;
	Speed = pNewDef->Speed;
	SmashRange = pNewDef->Range;
	bMagic = pNewDef->bMagic;
	bAllHit = pNewDef->bAllHit;
	KnockPow = pNewDef->KnockPow;
	DmgWait = pNewDef->Shake;
	if( pNewDef->pChiled != NULL )
	{
		ChiledName = pNewDef->pChiled->Name;
		ChiledRange = pNewDef->pChiled->Range;
	}

	MaxSpeed = Speed;
	DownSpeed = MaxSpeed/75.0f;
	TurnSpeed = MaxSpeed/10.0f;
	return true;
}
