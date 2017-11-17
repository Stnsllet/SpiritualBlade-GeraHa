// TobiDarkRainGene.cpp: TobiDarkRainGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkRainBit.h"
#include "TobiDarkRainGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDarkRainGene::TobiDarkRainGene()
{

}

TobiDarkRainGene::~TobiDarkRainGene()
{

}

TobiObjectBase* TobiDarkRainGene::CreateObject() const
{
	return new TobiDarkRainBit;
}
