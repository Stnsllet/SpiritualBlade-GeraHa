
#ifndef _JOB_EVENT_BASE_
#define _JOB_EVENT_BASE_

#include <list>
#include "String.h"

class XMLPaser;
class GameSystem;

interface _EventCall
{
	virtual ~_EventCall(){}
	virtual bool Event( const XMLPaser& EventXML, GameSystem& System ) = 0;
	virtual LPCTSTR Name() const = 0;
};

interface _JobEvent : public _EventCall
{
	virtual ~_JobEvent(){}
	virtual void Job( GameSystem& System ) = 0;
	virtual void Clear() = 0;
};

template<class T> class JobEventBase : public _JobEvent
{
public:
	JobEventBase(){}
	virtual ~JobEventBase(){}

	virtual LPCTSTR Name() const{return T::GetEventName();}

	virtual bool Event( const XMLPaser& EventXML, GameSystem& System )
	{
		RefPtr<T> pNew(new T);
		NEW_CHECKP(Name(),0,pNew);
		if( !pNew->ReadEvent( EventXML, System ) )return false;
		if( pNew->Job( System ) )return true;	// é¿çsçœÇ›
		EventList.push_back( pNew );
		return false;
	}
	virtual void Job( GameSystem& System )
	{
		std::list<RefPtr<T> >::iterator itr = EventList.begin();
		while( itr != EventList.end() )
		{
			RefPtr<T> pExec = (*itr);
			bool bDeleteOK = pExec->Job( System );
			if( EventList.begin() == EventList.end() )break;

			if( bDeleteOK )itr = EventList.erase(itr);
			else itr++;
		}
	}
	virtual void Clear()
	{
		EventList.clear();
	}

	std::list<RefPtr<T> > EventList;
};

#endif // _JOB_EVENT_BASE_
