// TobiBall.h: TobiBall クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBALL_H__1A3C1245_0DB5_4ACE_8386_5C2336AC86B5__INCLUDED_)
#define AFX_TOBIBALL_H__1A3C1245_0DB5_4ACE_8386_5C2336AC86B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiBomb.h"

class TobiBall : public TobiBomb  
{
public:
	TobiBall();
	virtual ~TobiBall();

	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual RefPtr<TobiObjectBase> GetBurstObject();
};

#endif // !defined(AFX_TOBIBALL_H__1A3C1245_0DB5_4ACE_8386_5C2336AC86B5__INCLUDED_)
