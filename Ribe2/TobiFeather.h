// TobiFeather.h: TobiFeather クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFEATHER_H__76E767FA_9A9F_4D80_BE2A_F355DE47B6A4__INCLUDED_)
#define AFX_TOBIFEATHER_H__76E767FA_9A9F_4D80_BE2A_F355DE47B6A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiFeather : public TobiMagicBase  
{
public:
	TobiFeather();
	virtual ~TobiFeather();

	virtual bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
	float MyAngle;
	static const float Direction;
};

#endif // !defined(AFX_TOBIFEATHER_H__76E767FA_9A9F_4D80_BE2A_F355DE47B6A4__INCLUDED_)
