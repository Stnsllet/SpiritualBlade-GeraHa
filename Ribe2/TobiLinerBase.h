// TobiLinerBase.h: TobiLinerBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBILINERBASE_H__8DA4CB40_F44D_4000_825A_0702DF9F7C8D__INCLUDED_)
#define AFX_TOBILINERBASE_H__8DA4CB40_F44D_4000_825A_0702DF9F7C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draws.h"
#include "TobiSingleBase.h"

class TobiLinerBase : public TobiSingleBase  
{
public:
	TobiLinerBase();
	virtual ~TobiLinerBase();

	virtual bool Job( BattleSystem& System );
	virtual void Draw( Display& Disp );

	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	float SetAutoAngleTaget( float Range, float Dips=2/12.0f, float Width=0 );
	virtual bool ResetTrans();
	void Damege( BattleSystem& System );
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;
	RbPosition GetSideKnockBack( float VecAngle, float KnockVal, const RbPosition& Base, const RbActStatus& Taget ) const;

	float ShotRange;
	Draws::Echo<RbTransform> EchoImage;
};

#endif // !defined(AFX_TOBILINERBASE_H__8DA4CB40_F44D_4000_825A_0702DF9F7C8D__INCLUDED_)
