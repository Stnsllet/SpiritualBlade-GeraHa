// TobiDarkBlazeGene.cpp: TobiDarkBlazeGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkBlazeBit.h"
#include "TobiDarkBlazeGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiDarkBlazeGene::TobiDarkBlazeGene()
{

}

TobiDarkBlazeGene::~TobiDarkBlazeGene()
{

}

TobiFrameCircleBit* TobiDarkBlazeGene::CreateFlameObject() const
{
	return new TobiDarkBlazeBit;
}
