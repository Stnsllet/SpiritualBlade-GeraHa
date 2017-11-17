// TobiComet.h: TobiComet クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBICOMET_H__045BEA0F_C737_4131_A691_C9CFF32AD489__INCLUDED_)
#define AFX_TOBICOMET_H__045BEA0F_C737_4131_A691_C9CFF32AD489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiComet : public TobiMagicBase  
{
public:
	TobiComet();
	virtual ~TobiComet();

	virtual void Draw( Display& Disp );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBICOMET_H__045BEA0F_C737_4131_A691_C9CFF32AD489__INCLUDED_)
