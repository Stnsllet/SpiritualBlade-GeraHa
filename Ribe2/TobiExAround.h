// TobiExAround.h: TobiExAround クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXAROUND_H__BC2C69BF_EEC8_43E9_ADDF_4EA2040F9AF3__INCLUDED_)
#define AFX_TOBIEXAROUND_H__BC2C69BF_EEC8_43E9_ADDF_4EA2040F9AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiArrowRain.h"
#include "TobiExObjectBase.h"

class TobiExAround : public TobiExObjectBase<TobiArrowRain>  
{
public:
	TobiExAround();
	virtual ~TobiExAround();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
	TobiCreator* pBitCreator;
	virtual TobiObjectBase* CreateObject() const;
};

#endif // !defined(AFX_TOBIEXAROUND_H__BC2C69BF_EEC8_43E9_ADDF_4EA2040F9AF3__INCLUDED_)
