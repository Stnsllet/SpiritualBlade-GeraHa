
#ifndef __MEMORY_CHECKER__
#define __MEMORY_CHECKER__

#include <deque>
#include "CriticalSection.h"

class MemoryPointManager
{
public:
	struct InfoData
	{
		LPCTSTR Filename;
		long Line;
		const void* pPtr;
	};
	MemoryPointManager()
	{
	}
	~MemoryPointManager()
	{
		Dump();
	}
	void Clear()
	{
		Dump();
		CriticalSection Lock(LockObj);
		InfoList.clear();
	}
	bool NewCheck( LPCSTR pFile, long Line, const void* pPtr )
	{
		CriticalSection Lock(LockObj);
		InfoData New;
		New.Filename = pFile;
		New.Line = Line;
		New.pPtr = pPtr;
		InfoList.push_back( New );
		return true;
	}
	bool DelCheck( const void* pPtr )
	{
		CriticalSection Lock(LockObj);
		bool bRet = false;
		std::deque<InfoData>::iterator itr = InfoList.begin();
		while( itr != InfoList.end() )
			if( itr->pPtr != pPtr )itr++;
			else{
				itr = InfoList.erase(itr);
				bRet = true;
			}
		return bRet;
	}
	void Dump() const
	{
		CriticalSection Lock(LockObj);
		std::deque<InfoData>::const_iterator itr = InfoList.begin();
		while( itr != InfoList.end() )
		{
			const InfoData& Ref = *itr++;
			ATLTRACE( _T("Memory Leark !\n") );
			ATLTRACE( _T("%s(%d)\t0x%08x\n"), Ref.Filename, Ref.Line, (long)Ref.pPtr );
		}
	}
private:
	CriticalSection::CriticalSectionObject LockObj;
	std::deque<InfoData> InfoList;
};

#ifdef _POINTER_CHECHK
#define POINTMANAGER() MemoryPointManager g_MemManager;
#define NEW_CHECK(x) g_MemManager.NewCheck( __FILE__,__LINE__, x);
#define NEW_CHECKP(x,y,z) g_MemManager.NewCheck( x,y,z);
#define DEL_CHECK(x) g_MemManager.DelCheck(x);
#define MANAGER_CLEAR() g_MemManager.Clear();
extern MemoryPointManager g_MemManager;
#else
#define POINTMANAGER()
#define NEW_CHECK(x)
#define NEW_CHECKP(x,y,z)
#define DEL_CHECK(x)
#define MANAGER_CLEAR()
#endif

#endif //__MEMORY_CHECKER__