// TobiFlare.h: TobiFlare クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFLARE_H__0C31A5AD_CBC5_44AA_BAA5_9258C1B280AD__INCLUDED_)
#define AFX_TOBIFLARE_H__0C31A5AD_CBC5_44AA_BAA5_9258C1B280AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiFlare : public TobiSpreadBase  
{
public:
	TobiFlare();
	virtual ~TobiFlare();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIFLARE_H__0C31A5AD_CBC5_44AA_BAA5_9258C1B280AD__INCLUDED_)
