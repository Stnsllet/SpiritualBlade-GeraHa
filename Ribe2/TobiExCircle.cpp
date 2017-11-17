// TobiExCircle.cpp: TobiExCircle �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExCircleBit.h"
#include "TobiExCircle.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
