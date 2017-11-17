// TobiIwasi.h: TobiIwasi クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIIWASI_H__E6E048ED_DD48_4FB8_9A1A_B4971EE7CF70__INCLUDED_)
#define AFX_TOBIIWASI_H__E6E048ED_DD48_4FB8_9A1A_B4971EE7CF70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiHado.h"

class TobiIwasi : public TobiHado  
{
public:
	TobiIwasi();
	virtual ~TobiIwasi();

	virtual bool SetResource();
protected:
	void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIIWASI_H__E6E048ED_DD48_4FB8_9A1A_B4971EE7CF70__INCLUDED_)
