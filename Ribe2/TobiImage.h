// TobiImage.h: TobiImage クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIIMAGE_H__030CA267_BFB6_4E52_AE34_77BA1ADBD632__INCLUDED_)
#define AFX_TOBIIMAGE_H__030CA267_BFB6_4E52_AE34_77BA1ADBD632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPolyPoly.h"
#include "TobiObjectBase.h"

class TobiImage : public TobiObjectBase  
{
public:
	TobiImage();
	virtual ~TobiImage();
	virtual bool Job( BattleSystem& System );
	virtual void Draw( Display& Disp );

	bool SetErase( RbActStatus& Source );
	bool SetLightBit( RbActStatus& Source );
protected:
	enum eImageType
	{
		eScaleUp = 0,
		eMoveHight,
	};
	bool ResetTrans();
	bool JobErase( double Passed );
	bool JobHightMove( double Passed );

	eImageType ImgType;
	RbTransform StandTrans;
	RbTransform PosTrans;
	bool bSetTrans;
	double EffectTime;

	struct _ViewInfo
	{
		float Scale;
		float Hight;
		float HightMove;
	}ViewInfo;
};

#endif // !defined(AFX_TOBIIMAGE_H__030CA267_BFB6_4E52_AE34_77BA1ADBD632__INCLUDED_)
