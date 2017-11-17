// TobiExSimple.cpp: TobiExSimple �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExSimple.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiExSimple::TobiExSimple()
{
}

TobiExSimple::~TobiExSimple()
{
}

bool TobiExSimple::SetExDefine( ExCreator::BaseDef* pNewDef )
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

	return true;
}
