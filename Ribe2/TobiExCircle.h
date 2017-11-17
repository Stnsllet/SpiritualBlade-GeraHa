// TobiExCircle.h: TobiExCircle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXCIRCLE_H__61DA6966_EB50_4176_A5D6_2F11404CE074__INCLUDED_)
#define AFX_TOBIEXCIRCLE_H__61DA6966_EB50_4176_A5D6_2F11404CE074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFrameCircleGene.h"
#include "TobiExObjectBase.h"

class TobiExCircle : public TobiExObjectBase<TobiFrameCircleGene>  
{
public:
	TobiExCircle();
	virtual ~TobiExCircle();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
	virtual TobiFrameCircleBit* CreateFlameObject() const;
	RefPtr<ExCreator::BaseDef> pExDef;
};

#endif // !defined(AFX_TOBIEXCIRCLE_H__61DA6966_EB50_4176_A5D6_2F11404CE074__INCLUDED_)
