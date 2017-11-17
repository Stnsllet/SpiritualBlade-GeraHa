// TobiHopper.h: TobiHopper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHOPPER_H__908ECE3C_E0FC_4933_AB46_41665025B509__INCLUDED_)
#define AFX_TOBIHOPPER_H__908ECE3C_E0FC_4933_AB46_41665025B509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiArrow.h"

class TobiHopper : public TobiArrow  
{
public:
	TobiHopper();
	virtual ~TobiHopper();

protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIHOPPER_H__908ECE3C_E0FC_4933_AB46_41665025B509__INCLUDED_)
