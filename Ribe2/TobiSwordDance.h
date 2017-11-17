// TobiSwordDance.h: TobiSwordDance クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISWORDDANCE_H__6DB17372_4671_4BA7_B365_7575894B21D2__INCLUDED_)
#define AFX_TOBISWORDDANCE_H__6DB17372_4671_4BA7_B365_7575894B21D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiSwordDance : public TobiGeneratorBase  
{
public:
	TobiSwordDance();
	virtual ~TobiSwordDance();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBISWORDDANCE_H__6DB17372_4671_4BA7_B365_7575894B21D2__INCLUDED_)
