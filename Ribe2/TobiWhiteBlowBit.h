// TobiWhiteBlowBit.h: TobiWhiteBlowBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWHITEBLOWBIT_H__976A1378_0642_4D42_9DB8_931EDC9BAB82__INCLUDED_)
#define AFX_TOBIWHITEBLOWBIT_H__976A1378_0642_4D42_9DB8_931EDC9BAB82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"

class TobiWhiteBlowBit : public TobiSingleBase  
{
public:
	TobiWhiteBlowBit();
	virtual ~TobiWhiteBlowBit();

	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
};

#endif // !defined(AFX_TOBIWHITEBLOWBIT_H__976A1378_0642_4D42_9DB8_931EDC9BAB82__INCLUDED_)
