// TobiIce.h: TobiIce クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIICE_H__4CA84999_A4A6_4EE7_87F3_4B2021A81752__INCLUDED_)
#define AFX_TOBIICE_H__4CA84999_A4A6_4EE7_87F3_4B2021A81752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiIce : public TobiMagicBase  
{
public:
	TobiIce();
	virtual ~TobiIce();

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIICE_H__4CA84999_A4A6_4EE7_87F3_4B2021A81752__INCLUDED_)
