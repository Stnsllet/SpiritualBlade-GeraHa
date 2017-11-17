// RbEventSystem.cpp: RbEventSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "GameSystem.h"
#include "JobEvent.h"
#include "DirectEvent.h"
#include "RbEventSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbEventSystem::RbEventSystem()
{
	RegisterEvent( new DirectEvent::EventBase );
	RegisterEvent( new DirectEvent::Bat );
	RegisterEvent( new DirectEvent::Move );
	RegisterEvent( new DirectEvent::Sound );
	RegisterEvent( new DirectEvent::Paty );
	RegisterEvent( new DirectEvent::Land );
	RegisterEvent( new DirectEvent::Msg );
	RegisterEvent( new DirectEvent::Cache );
	RegisterEvent( new DirectEvent::LoadEvent );
	RegisterEvent( new DirectEvent::List );
	RegisterEvent( new DirectEvent::ZoomIn );
	RegisterEvent( new DirectEvent::Mode );
	RegisterEvent( new DirectEvent::AddLine );
	RegisterEvent( new DirectEvent::Choice );
	RegisterEvent( new DirectEvent::Tutorial );
	RegisterEvent( new DirectEvent::Rnd );
	RegisterEvent( new DirectEvent::Command );
	RegisterEvent( new DirectEvent::ChkFlg );
	RegisterEvent( new DirectEvent::UserData );
	RegisterEvent( new DirectEvent::Title );
	RegisterEvent( new DirectEvent::Powered );
	RegisterEvent( new DirectEvent::Poly );
	RegisterEvent( new DirectEvent::Shop );
	RegisterEvent( new DirectEvent::Result );
	RegisterEvent( new DirectEvent::Popup );
	RegisterEvent( new DirectEvent::Param );
	RegisterEvent( new DirectEvent::CallTrans );
	RegisterEvent( new DirectEvent::Window );
	RegisterJobEvent( new JobEventBase<JobEvent::EventTimer> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventHouse> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventState> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventAct> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventDelay> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventCount> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventRange> );
	RegisterJobEvent( new JobEventBase<JobEvent::EventTres> );
}

bool RbEventSystem::RegisterEvent( _EventCall* pNewEvent )
{
	if( pNewEvent == NULL )return false;
	ATLASSERT( EventCallMap.find( pNewEvent->Name() ) == EventCallMap.end() );
	EventCallMap[pNewEvent->Name()].Set( pNewEvent );
	return true;
}

bool RbEventSystem::RegisterJobEvent( _JobEvent* pNewEvent )
{
	if( !RegisterEvent( pNewEvent ) )return false;
	ATLASSERT( JobCallMap.find( pNewEvent->Name() ) == JobCallMap.end() );
	JobCallMap[pNewEvent->Name()] = pNewEvent;
	return true;
}

RbEventSystem::~RbEventSystem()
{

}

void RbEventSystem::Job()
{
	JobMap::iterator itr = JobCallMap.begin();
	while( itr != JobCallMap.end() )
	{
		(itr++)->second->Job( *g_pSystem );
	}
}

void RbEventSystem::ClearJob()
{
	JobMap::iterator itr = JobCallMap.begin();
	while( itr != JobCallMap.end() )
	{
		(itr++)->second->Clear();
	}
}

void RbEventSystem::ClearListEvent()
{
	ListEvents.clear();
}

HRESULT RbEventSystem::LoadEvent( LPCTSTR FileName )
{
//	XMLPaser EventXML;
//	HRESULT ret = EventXML.Load( FileName );
//	ATLASSERT( !FAILED(ret) );
//	if( FAILED(ret) )return ret;

	XMLPaser EventXML = GetCache().XML.Get( FileName );
	if( !EventXML )return E_FAIL;

	Event( EventXML );
	g_pSystem->TimeSystem.Sleep();
	return S_OK;
}

bool RbEventSystem::Event( const XMLPaser& EventXML )
{
	if( g_pSystem == NULL )return false;
	return Event( EventXML, *g_pSystem );
}

bool RbEventSystem::Event( const XMLPaser& EventXML, GameSystem& System )
{
	LockCounter::Locker Lock(CallCounter);
	if( CallCounter.Count() > 500 )
	{
		ATLASSERT(_T("Event OverFlow!!\n"));
		return false;
	}

	CComBSTR TagName;
	EventXML.TagName( &TagName );
	String EventName = TagName;
	EventMap::const_iterator itr = EventCallMap.find( EventName );
	if( itr == EventCallMap.end() )return false;

	return itr->second->Event( ParamXML(EventXML), System );
}

long RbEventSystem::ChildEvent( const XMLPaser& EventXML )
{
	if( g_pSystem == NULL )return 0;

	GameSystem& System = *g_pSystem;
	XMLList EventList;
	EventList.Child( EventXML );
	long ExecCount = 0;
	int Count = EventList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		if( Event( EventList.GetItem(i), System ) )++ExecCount;
	}
	return ExecCount;
}

long RbEventSystem::AddList( const XMLList& AddEventList )
{
	long Count = AddEventList.GetCount();
	for( long i = 0; i < Count; i++ )
	{
		ListEvents.push_front( AddEventList.GetItem(Count-i-1) );
	}
	ListEvent();
	return Count;
}

bool RbEventSystem::ListEvent()
{
	if( ListEvents.size() <= 0 )return false;

	RbWindowSystem& System = GetWindowSystem();
	if( System.OnMsg() )return false;

	while( !System.OnMsg() && ListEvents.size() > 0 )
	{
		XMLPaser Exec = *ListEvents.begin();
		ListEvents.pop_front();
		Event( Exec );
	}
	return true;
}
