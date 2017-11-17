// TobiSingleBase.h: TobiSingleBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISINGLEBASE_H__B3F87D72_03B8_4718_AAE9_ADA391EFFC1C__INCLUDED_)
#define AFX_TOBISINGLEBASE_H__B3F87D72_03B8_4718_AAE9_ADA391EFFC1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiObjectBase.h"
#include "RbTransform.h"

class TobiSingleBase : public TobiObjectBase
{
public:
	TobiSingleBase();
	virtual ~TobiSingleBase();
	virtual bool Job( BattleSystem& System );
	virtual void Draw( Display& Disp );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

protected:
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;
	virtual void Damege( BattleSystem& System );
	virtual bool ResetTrans();
	float Speed;
	RbPosition Vector;
	RbPosition KnockBack;
	RbTransform Trans;
	bool bTransSetOK;
	float SmashRange;
	float KnockPow;

	long StartSoundNum;

	static const float DefaultSpeed;
};

#endif // !defined(AFX_TOBISINGLEBASE_H__B3F87D72_03B8_4718_AAE9_ADA391EFFC1C__INCLUDED_)
