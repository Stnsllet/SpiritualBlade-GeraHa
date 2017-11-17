// TobiSplash.h: TobiSplash クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISPLASH_H__AAC232F2_9CAF_4C36_B559_C92B3464B39A__INCLUDED_)
#define AFX_TOBISPLASH_H__AAC232F2_9CAF_4C36_B559_C92B3464B39A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiSplash : public TobiSpreadBase  
{
public:
	TobiSplash();
	virtual ~TobiSplash();
	virtual bool SetResource();
};

#endif // !defined(AFX_TOBISPLASH_H__AAC232F2_9CAF_4C36_B559_C92B3464B39A__INCLUDED_)
