// TobiExRndHit.h: TobiExRndHit �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXRNDHIT_H__334BFD55_5C08_497B_9D27_3CE9FA70D818__INCLUDED_)
#define AFX_TOBIEXRNDHIT_H__334BFD55_5C08_497B_9D27_3CE9FA70D818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"
#include "TobiExObjectBase.h"

class TobiExRndHit : public TobiExObjectBase<TobiGeneratorBase>  
{
public:
	TobiExRndHit();
	virtual ~TobiExRndHit();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
	bool Generate();
	TobiCreator* pBitCreator;
};

#endif // !defined(AFX_TOBIEXRNDHIT_H__334BFD55_5C08_497B_9D27_3CE9FA70D818__INCLUDED_)
