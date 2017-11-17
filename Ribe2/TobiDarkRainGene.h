// TobiDarkRainGene.h: TobiDarkRainGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDARKRAINGENE_H__54E53D52_F15A_4060_9697_95806B79A90E__INCLUDED_)
#define AFX_TOBIDARKRAINGENE_H__54E53D52_F15A_4060_9697_95806B79A90E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiArrowRain.h"

class TobiDarkRainGene : public TobiArrowRain  
{
public:
	TobiDarkRainGene();
	virtual ~TobiDarkRainGene();

protected:
	virtual TobiObjectBase* CreateObject() const;
};

#endif // !defined(AFX_TOBIDARKRAINGENE_H__54E53D52_F15A_4060_9697_95806B79A90E__INCLUDED_)
