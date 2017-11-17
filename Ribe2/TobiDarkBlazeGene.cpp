// TobiDarkBlazeGene.cpp: TobiDarkBlazeGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkBlazeBit.h"
#include "TobiDarkBlazeGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
