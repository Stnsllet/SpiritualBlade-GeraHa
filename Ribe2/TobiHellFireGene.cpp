// TobiHellFireGene.cpp: TobiHellFireGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiHellFireBit.h"
#include "TobiHellFireGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiHellFireGene::TobiHellFireGene()
{

}

TobiHellFireGene::~TobiHellFireGene()
{

}

TobiFrameCircleBit* TobiHellFireGene::CreateFlameObject() const
{
	return new TobiHellFireBit;
}
