// TobiDeadlySlash.h: TobiDeadlySlash クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDEADLYSLASH_H__AAB355D2_2622_4A58_8D2D_35C6CBE22A72__INCLUDED_)
#define AFX_TOBIDEADLYSLASH_H__AAB355D2_2622_4A58_8D2D_35C6CBE22A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiDeadlySlash : public TobiAttackSpreadBase  
{
public:
	TobiDeadlySlash();
	virtual ~TobiDeadlySlash();

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIDEADLYSLASH_H__AAB355D2_2622_4A58_8D2D_35C6CBE22A72__INCLUDED_)
