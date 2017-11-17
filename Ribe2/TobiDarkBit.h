// TobiDarkBit.h: TobiDarkBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDRAKBIT_H__2581295E_8500_4592_AC96_55CDAF6878DA__INCLUDED_)
#define AFX_TOBIDRAKBIT_H__2581295E_8500_4592_AC96_55CDAF6878DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiDarkRainBit.h"

class TobiDarkBit : public TobiDarkRainBit  
{
public:
	TobiDarkBit();
	virtual ~TobiDarkBit();

protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIDRAKBIT_H__2581295E_8500_4592_AC96_55CDAF6878DA__INCLUDED_)
