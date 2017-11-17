// TobiRandomBall.h: TobiRandomBall クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIRANDOMBALL_H__D222C20C_73E1_4FBE_890B_2FA2134994D6__INCLUDED_)
#define AFX_TOBIRANDOMBALL_H__D222C20C_73E1_4FBE_890B_2FA2134994D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiRandomBall : public TobiGeneratorBase  
{
public:
	TobiRandomBall();
	virtual ~TobiRandomBall();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIRANDOMBALL_H__D222C20C_73E1_4FBE_890B_2FA2134994D6__INCLUDED_)
