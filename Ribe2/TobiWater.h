// TobiWater.h: TobiWater クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWATER_H__B0DF83EC_C966_48AB_B9F7_B184DBE03252__INCLUDED_)
#define AFX_TOBIWATER_H__B0DF83EC_C966_48AB_B9F7_B184DBE03252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiBurstBase.h"

class TobiWater : public TobiBurstBase  
{
public:
	TobiWater();
	virtual ~TobiWater();
	virtual bool Job( BattleSystem& System );
	virtual bool SetResource();
protected:
	virtual RefPtr<TobiObjectBase> GetBurstObject();
};

#endif // !defined(AFX_TOBIWATER_H__B0DF83EC_C966_48AB_B9F7_B184DBE03252__INCLUDED_)
