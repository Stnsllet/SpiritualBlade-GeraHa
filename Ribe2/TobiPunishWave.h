// TobiPunishWave.h: TobiPunishWave クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIPUNISHWAVE_H__9F32F71D_9029_4046_B96E_131063F888DC__INCLUDED_)
#define AFX_TOBIPUNISHWAVE_H__9F32F71D_9029_4046_B96E_131063F888DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiPunishWave : public TobiGeneratorBase  
{
public:
	TobiPunishWave();
	virtual ~TobiPunishWave();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIPUNISHWAVE_H__9F32F71D_9029_4046_B96E_131063F888DC__INCLUDED_)
