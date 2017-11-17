// TobiHeatWave.h: TobiHeatWave クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHEATWAVE_H__6BCF6D44_8B33_4C76_8350_73567EEAC422__INCLUDED_)
#define AFX_TOBIHEATWAVE_H__6BCF6D44_8B33_4C76_8350_73567EEAC422__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiHeatWave : public TobiSpreadBase  
{
public:
	TobiHeatWave();
	virtual ~TobiHeatWave();
	void SetInfo( float NewSpreadRange, float NewSpreadKnockBack )
		{ SetBaseInfo( NewSpreadRange, NewSpreadKnockBack ); }

	virtual bool SetResource();
	bool SetAllHit( bool bAll ){return (bAllHit = bAll);}
};

#endif // !defined(AFX_TOBIHEATWAVE_H__6BCF6D44_8B33_4C76_8350_73567EEAC422__INCLUDED_)
