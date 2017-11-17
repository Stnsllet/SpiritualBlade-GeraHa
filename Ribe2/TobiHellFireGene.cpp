// TobiHellFireGene.cpp: TobiHellFireGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiHellFireBit.h"
#include "TobiHellFireGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
