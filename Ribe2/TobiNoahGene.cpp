// TobiNoahGene.cpp: TobiNoahGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiNoahBit.h"
#include "BattleSystem.h"
#include "TobiNoahGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiNoahGene::TobiNoahGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiNoahGene::~TobiNoahGene()
{
}

bool TobiNoahGene::Generate()
{
	TobiRefPtr<TobiNoahBit> pBit(MyEffect);
	if( !pBit->SetSource(*pMember) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
