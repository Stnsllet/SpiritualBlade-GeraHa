// TobiBlastEdge.h: TobiBlastEdge クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBLASTEDGE_H__0A6CE104_4157_4B0E_8737_9647FEDBCE92__INCLUDED_)
#define AFX_TOBIBLASTEDGE_H__0A6CE104_4157_4B0E_8737_9647FEDBCE92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiBlastEdge : public TobiAttackSpreadBase  
{
public:
	TobiBlastEdge();
	virtual ~TobiBlastEdge();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIBLASTEDGE_H__0A6CE104_4157_4B0E_8737_9647FEDBCE92__INCLUDED_)
