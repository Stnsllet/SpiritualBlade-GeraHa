// TobiFrameCircleGene.h: TobiFrameCircleGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFRAMECIRCLEGENE_H__2B4AC3A7_3304_419F_9755_C6896BA151A1__INCLUDED_)
#define AFX_TOBIFRAMECIRCLEGENE_H__2B4AC3A7_3304_419F_9755_C6896BA151A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"
class TobiFrameCircleBit;

class TobiFrameCircleGene : public TobiGeneratorBase  
{
public:
	TobiFrameCircleGene();
	virtual ~TobiFrameCircleGene();

	virtual bool SetEffect( const SkillEffect& Effect );
	bool Job( BattleSystem& System );
protected:
	bool CreateCircle( const SkillEffect& Effect );
	bool CreateFlameBit( const SkillEffect& Effect, float StartAngle );
	virtual TobiFrameCircleBit* CreateFlameObject() const;
};

#endif // !defined(AFX_TOBIFRAMECIRCLEGENE_H__2B4AC3A7_3304_419F_9755_C6896BA151A1__INCLUDED_)
