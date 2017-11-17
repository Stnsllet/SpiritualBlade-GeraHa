// CriticalSection.h: CriticalSection クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSECTION_H__ED36654F_81A4_417F_9146_763F2E144A0A__INCLUDED_)
#define AFX_CRITICALSECTION_H__ED36654F_81A4_417F_9146_763F2E144A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CriticalSection
{
public:
	class CriticalSectionObject
	{
	public:
		CriticalSectionObject() : pSection(&Section)
		{InitializeCriticalSection(&Section);}
		~CriticalSectionObject(){DeleteCriticalSection(&Section);}
		operator LPCRITICAL_SECTION() const{return pSection;}

	private:
		RTL_CRITICAL_SECTION Section;
		LPCRITICAL_SECTION pSection;
	};

public:
	CriticalSection( const CriticalSectionObject& Object )
	{
		pSection = NULL;
		EnterCriticalSection( Object );
		pSection = Object;
	}
	~CriticalSection()
	{
		if( pSection != NULL )LeaveCriticalSection( pSection );
	}
private:
	LPCRITICAL_SECTION pSection;
};

#endif // !defined(AFX_CRITICALSECTION_H__ED36654F_81A4_417F_9146_763F2E144A0A__INCLUDED_)
