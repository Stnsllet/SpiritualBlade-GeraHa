// TobiExAround.cpp: TobiExAround クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExAround.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExAround::TobiExAround()
:pBitCreator(NULL)
{

}

TobiExAround::~TobiExAround()
{

}

bool TobiExAround::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	GenerateTime = GenerateInterval = pNewDef->Shake;
	if( GenerateInterval < 20 )GenerateInterval = 20;
	CreateDips = pNewDef->Range;

	if( pNewDef->pChiled == NULL )return false;
	pBitCreator = System.TobiSys.GetCreator( pNewDef->pChiled->Name );
	ChiledRange = pNewDef->pChiled->Range;

	return true;
}

TobiObjectBase* TobiExAround::CreateObject() const
{
	if( pBitCreator == NULL )return NULL;
	return pBitCreator->Create();
}
