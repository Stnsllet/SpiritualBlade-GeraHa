// TobiHado.h: TobiHado クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHADO_H__3E3E9AFF_0DAF_4186_8598_8652900C64A0__INCLUDED_)
#define AFX_TOBIHADO_H__3E3E9AFF_0DAF_4186_8598_8652900C64A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiLinerBase.h"

class TobiHado : public TobiLinerBase  
{
public:
	TobiHado();
	virtual ~TobiHado();

	virtual bool SetResource();
	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
};

#endif // !defined(AFX_TOBIHADO_H__3E3E9AFF_0DAF_4186_8598_8652900C64A0__INCLUDED_)
