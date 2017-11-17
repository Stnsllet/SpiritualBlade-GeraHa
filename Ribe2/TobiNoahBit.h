// TobiNoahBit.h: TobiNoahBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBINOAHBIT_H__EF72F87F_FE17_40B4_ACB5_2FCC778AD5EC__INCLUDED_)
#define AFX_TOBINOAHBIT_H__EF72F87F_FE17_40B4_ACB5_2FCC778AD5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiNoahBit : public TobiSpreadBase  
{
public:
	TobiNoahBit();
	virtual ~TobiNoahBit();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBINOAHBIT_H__EF72F87F_FE17_40B4_ACB5_2FCC778AD5EC__INCLUDED_)
