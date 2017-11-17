// TobiBeast.h: TobiBeast クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBEAST_H__E0DF1BA6_190C_4F8A_B681_68E58F92A43F__INCLUDED_)
#define AFX_TOBIBEAST_H__E0DF1BA6_190C_4F8A_B681_68E58F92A43F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiLinerBase.h"

class TobiBeast : public TobiLinerBase  
{
public:
	TobiBeast();
	virtual ~TobiBeast();
	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;

	float VectorAngle;
};

#endif // !defined(AFX_TOBIBEAST_H__E0DF1BA6_190C_4F8A_B681_68E58F92A43F__INCLUDED_)
