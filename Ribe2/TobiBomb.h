// TobiBomb.h: TobiBomb クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBOMB_H__D620A534_1E33_4D6F_A270_EC4FEF8A3F38__INCLUDED_)
#define AFX_TOBIBOMB_H__D620A534_1E33_4D6F_A270_EC4FEF8A3F38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiBurstBase.h"

class TobiBomb : public TobiBurstBase  
{
public:
	TobiBomb();
	virtual ~TobiBomb();
	virtual bool SetResource();
protected:
	virtual bool ResetTrans();

	virtual RefPtr<TobiObjectBase> GetBurstObject();
};

#endif // !defined(AFX_TOBIBOMB_H__D620A534_1E33_4D6F_A270_EC4FEF8A3F38__INCLUDED_)
