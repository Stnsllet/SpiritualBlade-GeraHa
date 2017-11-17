// TobiThunder.h: TobiThunder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBITHUNDER_H__E8E9AD40_2EF4_4B57_A778_B59E9B4BB0A3__INCLUDED_)
#define AFX_TOBITHUNDER_H__E8E9AD40_2EF4_4B57_A778_B59E9B4BB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiThunder : public TobiMagicBase  
{
public:
	TobiThunder();
	virtual ~TobiThunder();

	virtual bool SetResource();
	virtual void Draw( Display& Disp );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	float MaxLife;
	float PolyTop;
};

#endif // !defined(AFX_TOBITHUNDER_H__E8E9AD40_2EF4_4B57_A778_B59E9B4BB0A3__INCLUDED_)
