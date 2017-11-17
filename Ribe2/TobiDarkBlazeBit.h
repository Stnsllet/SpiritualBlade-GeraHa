// TobiDarkBlazeBit.h: TobiDarkBlazeBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDARKBLAZEBIT_H__2C8D25A5_3500_49C9_9320_4E95C40ECA37__INCLUDED_)
#define AFX_TOBIDARKBLAZEBIT_H__2C8D25A5_3500_49C9_9320_4E95C40ECA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiHellFireBit.h"

class TobiDarkBlazeBit : public TobiHellFireBit  
{
public:
	TobiDarkBlazeBit();
	virtual ~TobiDarkBlazeBit();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIDARKBLAZEBIT_H__2C8D25A5_3500_49C9_9320_4E95C40ECA37__INCLUDED_)
