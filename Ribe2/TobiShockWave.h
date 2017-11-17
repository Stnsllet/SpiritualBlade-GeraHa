// TobiShockWave.h: TobiShockWave クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISHOCKWAVE_H__2D36BAFA_5D5E_40C9_B742_35C97C5D9807__INCLUDED_)
#define AFX_TOBISHOCKWAVE_H__2D36BAFA_5D5E_40C9_B742_35C97C5D9807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiShockWave : public TobiAttackSpreadBase  
{
public:
	TobiShockWave();
	virtual ~TobiShockWave();
	virtual bool SetResource();
};

#endif // !defined(AFX_TOBISHOCKWAVE_H__2D36BAFA_5D5E_40C9_B742_35C97C5D9807__INCLUDED_)
