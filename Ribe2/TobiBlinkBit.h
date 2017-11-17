// TobiBlinkBit.h: TobiBlinkBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBLINKBIT_H__183CD2D1_52B3_459F_AD98_B206F80527F3__INCLUDED_)
#define AFX_TOBIBLINKBIT_H__183CD2D1_52B3_459F_AD98_B206F80527F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"
#include "Material.h"

class TobiBlinkBit : public TobiSingleBase  
{
public:
	TobiBlinkBit();
	virtual ~TobiBlinkBit();
	virtual void Draw( Display& Disp );
	virtual bool Job( BattleSystem& System );
	virtual bool SetTaget( RbActStatus* pSource, RbActStatus* pEnemy );

protected:
	virtual void Damege( BattleSystem& System );
	long ActID;
	float MyAngle;
	RefPtr<RbActStatus> pTaget;
	float DelayTime;
	float MaxActTime;
	double StartTime;
	static const Material BlinkMate;
};

#endif // !defined(AFX_TOBIBLINKBIT_H__183CD2D1_52B3_459F_AD98_B206F80527F3__INCLUDED_)
