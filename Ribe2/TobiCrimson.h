// TobiCrimson.h: TobiCrimson クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBICRIMSON_H__E4BAD8BD_6450_4058_A198_6B0BD313D4AB__INCLUDED_)
#define AFX_TOBICRIMSON_H__E4BAD8BD_6450_4058_A198_6B0BD313D4AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draws.h"
#include "TobiSpreadBase.h"

class TobiCrimson : public TobiSpreadBase  
{
public:
	TobiCrimson();
	virtual ~TobiCrimson();
	virtual void Draw( Display& Disp );

	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
	virtual bool Job( BattleSystem& System );
	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetResource();
protected:

	ActLocker Locker;
	long EffectCount;
	double EffectInterval;
	double NowEffectTime;
	double StartTime;

	Draws::Echo<RbTransform> EchoImage;
};

#endif // !defined(AFX_TOBICRIMSON_H__E4BAD8BD_6450_4058_A198_6B0BD313D4AB__INCLUDED_)
