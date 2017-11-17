// TobiDeathSickle.h: TobiDeathSickle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDEATHSICKLE_H__4EC3A573_95DA_49D8_AB8E_773C8F1ECC74__INCLUDED_)
#define AFX_TOBIDEATHSICKLE_H__4EC3A573_95DA_49D8_AB8E_773C8F1ECC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiDeathSickle : public TobiSpreadBase  
{
public:
	TobiDeathSickle();
	virtual ~TobiDeathSickle();

	virtual void Draw( Display& Disp );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
	virtual bool Job( BattleSystem& System );
	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );

	ActLocker Locker;
	float StartAngle, NowAngle;
};

#endif // !defined(AFX_TOBIDEATHSICKLE_H__4EC3A573_95DA_49D8_AB8E_773C8F1ECC74__INCLUDED_)
