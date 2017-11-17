// TobiBlink.h: TobiBlink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBLINK_H__673193EC_A23B_443D_A9D9_A24C2A9DF5F5__INCLUDED_)
#define AFX_TOBIBLINK_H__673193EC_A23B_443D_A9D9_A24C2A9DF5F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiBlink : public TobiGeneratorBase  
{
public:
	TobiBlink();
	virtual ~TobiBlink();

protected:
	virtual bool Generate();
};

#endif // !defined(AFX_TOBIBLINK_H__673193EC_A23B_443D_A9D9_A24C2A9DF5F5__INCLUDED_)
