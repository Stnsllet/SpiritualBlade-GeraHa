// TobiSoulBurst.cpp: TobiSoulBurst �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBurst.h"
#include "BattleSystem.h"
#include "TobiSoulBurst.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiSoulBurst::TobiSoulBurst()
{
	GenerateTime = GenerateInterval = 300;
}

TobiSoulBurst::~TobiSoulBurst()
{
}

bool TobiSoulBurst::Generate()
{
	TobiRefPtr<TobiBurst> pBit(MyEffect);
	if( !pBit->SetSource(*pMember) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
