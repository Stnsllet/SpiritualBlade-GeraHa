// TobiBlueWave.h: TobiBlueWave クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBLUEWAVE_H__7D8225E3_C012_4968_9E92_06E3DE32D99A__INCLUDED_)
#define AFX_TOBIBLUEWAVE_H__7D8225E3_C012_4968_9E92_06E3DE32D99A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiBlueWave : public TobiAttackSpreadBase  
{
public:
	TobiBlueWave();
	virtual ~TobiBlueWave();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIBLUEWAVE_H__7D8225E3_C012_4968_9E92_06E3DE32D99A__INCLUDED_)
