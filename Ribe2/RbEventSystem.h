// RbEventSystem.h: RbEventSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBEVENTSYSTEM_H__F730DAFC_5D36_4F3D_A73D_D7DA4E955361__INCLUDED_)
#define AFX_RBEVENTSYSTEM_H__F730DAFC_5D36_4F3D_A73D_D7DA4E955361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include "RefObject.h"
#include "XMLPaser.h"
#include "JobEventBase.h"
class XMLList;
class GameSystem;

class RbEventSystem  
{
public:
	RbEventSystem();
	~RbEventSystem();

	HRESULT LoadEvent( LPCTSTR FileName );
	bool Event( const XMLPaser& EventXML );
	bool Event( const XMLPaser& EventXML, GameSystem& System );
	long ChildEvent( const XMLPaser& EventXML );

	long AddList( const XMLList& AddEventList );
	bool ListEvent();
	void Job();
	void ClearJob();
	void ClearListEvent();

	LockCounter ActLock;
protected:
	bool RegisterEvent( _EventCall* pNewEvent );
	bool RegisterJobEvent( _JobEvent* pNewEvent );

	typedef std::map<String,MasterPtr<_EventCall> > EventMap;
	typedef std::map<String,_JobEvent* > JobMap;
	typedef std::list<XMLPaser> WaitType;
	EventMap EventCallMap;
	JobMap JobCallMap;
	WaitType ListEvents;

	LockCounter CallCounter;
};

RbEventSystem& GetEventSystem();

#endif // !defined(AFX_RBEVENTSYSTEM_H__F730DAFC_5D36_4F3D_A73D_D7DA4E955361__INCLUDED_)
