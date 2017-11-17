// TobiArrowRain.h: TobiArrowRain クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIARROWRAIN_H__D773C0E1_AAAF_4E02_973A_9D6576AF9956__INCLUDED_)
#define AFX_TOBIARROWRAIN_H__D773C0E1_AAAF_4E02_973A_9D6576AF9956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiArrowRain : public TobiGeneratorBase  
{
public:
	TobiArrowRain();
	virtual ~TobiArrowRain();

	bool Job( BattleSystem& System );
protected:
	bool CreateRain( float Range );
	long CreateCircleRain( float Range, float Dips );
	bool CreateArrow( const RbPosition& Base, const RbPosition& Taget ) const;
	virtual TobiObjectBase* CreateObject() const;
	float CreateDips;
};

#endif // !defined(AFX_TOBIARROWRAIN_H__D773C0E1_AAAF_4E02_973A_9D6576AF9956__INCLUDED_)
