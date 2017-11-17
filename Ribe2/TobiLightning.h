// TobiLightning.h: TobiLightning クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBILIGHTNING_H__7CBE3D76_32F6_4A0A_9AEA_3320F9778EA8__INCLUDED_)
#define AFX_TOBILIGHTNING_H__7CBE3D76_32F6_4A0A_9AEA_3320F9778EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiArrowRain.h"

class TobiLightning : public TobiArrowRain  
{
public:
	TobiLightning();
	virtual ~TobiLightning();

	virtual bool SetEffect( const SkillEffect& Effect );
	bool Job( BattleSystem& System );
protected:
	virtual bool Generate();
	virtual TobiObjectBase* CreateObject() const;
	const static float LightDips;
	float MyRange;
};

#endif // !defined(AFX_TOBILIGHTNING_H__7CBE3D76_32F6_4A0A_9AEA_3320F9778EA8__INCLUDED_)
