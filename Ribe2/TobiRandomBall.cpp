// TobiRandomBall.cpp: TobiRandomBall �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBall.h"
#include "TobiRandomBall.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiRandomBall::TobiRandomBall()
{
	GenerateTime = GenerateInterval = 100;
}

TobiRandomBall::~TobiRandomBall()
{

}

bool TobiRandomBall::Generate()
{
	TobiRefPtr<TobiBall> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
