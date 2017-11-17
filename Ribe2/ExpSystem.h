// ExpSystem.h: ExpSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPSYSTEM_H__8D1EF817_4E00_462C_98AF_61EFC6061BDF__INCLUDED_)
#define AFX_EXPSYSTEM_H__8D1EF817_4E00_462C_98AF_61EFC6061BDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusStruct.h"

class ExpSystem  
{
public:
	ExpSystem(MemberStatus& ref, int Max=99);
	long AddExp( const MemberStatus& Enemy, float UpScale, bool bDead );

	static long NextCount;
private:
	long Exp( long MyLev, long EneLev ) const;
	long NextExp( long MyLev ) const;

	int MaxLevel;
	MemberStatus& Status;
};

#endif // !defined(AFX_EXPSYSTEM_H__8D1EF817_4E00_462C_98AF_61EFC6061BDF__INCLUDED_)
