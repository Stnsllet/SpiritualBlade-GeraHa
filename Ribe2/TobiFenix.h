// TobiFenix.h: TobiFenix クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFENIX_H__846ABEE4_323D_4E1A_B247_A68DBCD0E64B__INCLUDED_)
#define AFX_TOBIFENIX_H__846ABEE4_323D_4E1A_B247_A68DBCD0E64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMissleBase.h"

class TobiFenix : public TobiMissleBase  
{
public:
	TobiFenix();
	virtual ~TobiFenix();
	virtual bool Job( BattleSystem& System );

	virtual bool SetResource();
protected:
	virtual bool TagetHitJob( BattleSystem& System );
	virtual bool ResetTrans();
	virtual void Damege( BattleSystem& System );

	double NextHitTime;
};

#endif // !defined(AFX_TOBIFENIX_H__846ABEE4_323D_4E1A_B247_A68DBCD0E64B__INCLUDED_)
