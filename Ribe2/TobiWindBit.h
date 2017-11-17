// TobiWindBit.h: TobiWindBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWINDBIT_H__FA827831_25F4_4D4F_B979_0C535D73A0D9__INCLUDED_)
#define AFX_TOBIWINDBIT_H__FA827831_25F4_4D4F_B979_0C535D73A0D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiLinerBase.h"

class TobiWindBit : public TobiLinerBase  
{
public:
	TobiWindBit();
	virtual ~TobiWindBit();

	virtual bool SetResource();
	virtual bool SetEffect( const SkillEffect& Effect );
};

#endif // !defined(AFX_TOBIWINDBIT_H__FA827831_25F4_4D4F_B979_0C535D73A0D9__INCLUDED_)
