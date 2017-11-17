// TobiDarkIraption.h: TobiDarkIraption クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDARKIRAPTION_H__CC08DAF7_E174_4DE2_9768_FD292D38352F__INCLUDED_)
#define AFX_TOBIDARKIRAPTION_H__CC08DAF7_E174_4DE2_9768_FD292D38352F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiDarkIraption : public TobiGeneratorBase  
{
public:
	TobiDarkIraption();
	virtual ~TobiDarkIraption();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIDARKIRAPTION_H__CC08DAF7_E174_4DE2_9768_FD292D38352F__INCLUDED_)
