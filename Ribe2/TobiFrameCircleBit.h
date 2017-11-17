// TobiFrameCircleBit.h: TobiFrameCircleBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFRAMECIRCLEBIT_H__63605A54_B954_4DA5_8ED5_D59DE73B0D54__INCLUDED_)
#define AFX_TOBIFRAMECIRCLEBIT_H__63605A54_B954_4DA5_8ED5_D59DE73B0D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiFrameCircleBit : public TobiMagicBase  
{
public:
	TobiFrameCircleBit();
	virtual ~TobiFrameCircleBit();

	bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

	virtual bool SetResource();
	void SetStartAngle( float Angle ){MyAngle = Angle;}
	bool SetEffect( const SkillEffect& Effect );
protected:
	bool ResetTrans();
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;
	float MyAngle;
	float MyLength, MaxRange;
};

#endif // !defined(AFX_TOBIFRAMECIRCLEBIT_H__63605A54_B954_4DA5_8ED5_D59DE73B0D54__INCLUDED_)
