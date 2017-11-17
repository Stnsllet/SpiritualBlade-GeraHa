// TobiRuncherGene.h: TobiRuncherGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIRUNCHERGENE_H__6552F0BC_57AC_4206_9CDA_3221DD463270__INCLUDED_)
#define AFX_TOBIRUNCHERGENE_H__6552F0BC_57AC_4206_9CDA_3221DD463270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiRuncherGene : public TobiGeneratorBase  
{
public:
	TobiRuncherGene();
	virtual ~TobiRuncherGene();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIRUNCHERGENE_H__6552F0BC_57AC_4206_9CDA_3221DD463270__INCLUDED_)
