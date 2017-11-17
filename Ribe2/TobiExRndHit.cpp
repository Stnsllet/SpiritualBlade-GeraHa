// TobiExRndHit.cpp: TobiExRndHit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExRndHit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiExRndHit::TobiExRndHit()
: pBitCreator(NULL)
{
	GenerateTime = GenerateInterval = 100;
}

TobiExRndHit::~TobiExRndHit()
{

}

bool TobiExRndHit::Generate()
{
	if( pBitCreator == NULL )return false;
	RefPtr<TobiObjectBase> pBit = pBitCreator->Create();
	if( pBit == NULL )return false;
	SkillEffect Effect = MyEffect;
	pBit->SetEffect( Effect );
	return AddRandomTaget( pBit, MyEffect.Range );
}

bool TobiExRndHit::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	GenerateTime = GenerateInterval = pNewDef->Shake;
	if( GenerateInterval < 20 )GenerateInterval = 20;

	if( pNewDef->pChiled == NULL )return false;
	pBitCreator = System.TobiSys.GetCreator( pNewDef->pChiled->Name );
	ChiledRange = pNewDef->pChiled->Range;

	return true;
}
