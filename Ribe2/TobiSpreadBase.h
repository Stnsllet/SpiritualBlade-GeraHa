// TobiSpreadBase.h: TobiSpreadBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISPREADBASE_H__F618A5AF_42ED_408B_9E43_38D0B12E93B0__INCLUDED_)
#define AFX_TOBISPREADBASE_H__F618A5AF_42ED_408B_9E43_38D0B12E93B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiObjectBase.h"

class TobiSpreadBase : public TobiObjectBase  
{
public:
	TobiSpreadBase();
	virtual ~TobiSpreadBase();
	virtual bool Job( BattleSystem& System );
	virtual void Draw( Display& Disp );

	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

protected:
	virtual void Damege( BattleSystem& System );
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;

	void SetBaseInfo( float NewSpreadRange, float NewSpreadKnockBack=2 );

	float SpreadRange;
	float SpreadMaxRange;
	float SpreadKnockBack;

	float SpreadSpeed;

	bool bAllHit;
	long StartSoundNum;
};

#endif // !defined(AFX_TOBISPREADBASE_H__F618A5AF_42ED_408B_9E43_38D0B12E93B0__INCLUDED_)
