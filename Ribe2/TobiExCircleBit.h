// TobiExCircleBit.h: TobiExCircleBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXCIRCLEBIT_H__717F3D5F_3D7B_489B_89CD_477A2E3C1FF6__INCLUDED_)
#define AFX_TOBIEXCIRCLEBIT_H__717F3D5F_3D7B_489B_89CD_477A2E3C1FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFrameCircleBit.h"
#include "TobiExObjectBase.h"

class TobiExCircleBit : public TobiExObjectBase3<TobiFrameCircleBit>  
{
public:
	TobiExCircleBit();
	virtual ~TobiExCircleBit();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
};

#endif // !defined(AFX_TOBIEXCIRCLEBIT_H__717F3D5F_3D7B_489B_89CD_477A2E3C1FF6__INCLUDED_)
