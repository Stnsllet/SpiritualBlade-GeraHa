// TobiAngelDust.cpp: TobiAngelDust �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiFeather.h"
#include "TobiAngelDust.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiAngelDust::TobiAngelDust()
{
	GenerateTime = GenerateInterval = 200;
}

TobiAngelDust::~TobiAngelDust()
{
}

bool TobiAngelDust::Generate()
{
	TobiRefPtr<TobiFeather> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
