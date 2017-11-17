// TobiWindThrustGene.cpp: TobiWindThrustGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiWindBit.h"
#include "TobiWindThrustGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiWindThrustGene::TobiWindThrustGene()
{
	GenerateInterval = 1000.0/3.0f;
	GenerateTime = GenerateInterval;
}

TobiWindThrustGene::~TobiWindThrustGene()
{

}

bool TobiWindThrustGene::Generate()
{
	TobiRefPtr<TobiWindBit> pBit( MyEffect );
	if( !pBit->SetSource( *pMember ) )return false;

	System.TobiSys.Add( pBit );
	return ((--MyEffect.Count)>0);
}
