// TobiExBurst.cpp: TobiExBurst �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExBurst.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiExBurst::TobiExBurst()
{

}

TobiExBurst::~TobiExBurst()
{

}

bool TobiExBurst::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	SetPolygon( pNewDef->PolyID );
	StartSoundNum = pNewDef->WaveID;
	Speed = pNewDef->Speed;
	SpreadRange = SmashRange = pNewDef->Range;
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
