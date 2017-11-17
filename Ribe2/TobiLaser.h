// TobiLaser.h: TobiLaser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBILASER_H__F873C507_3BA5_4110_A2EA_D7387CB05FC4__INCLUDED_)
#define AFX_TOBILASER_H__F873C507_3BA5_4110_A2EA_D7387CB05FC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"

class TobiLaser : public TobiSingleBase  
{
public:
	TobiLaser();
	virtual ~TobiLaser();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBILASER_H__F873C507_3BA5_4110_A2EA_D7387CB05FC4__INCLUDED_)
