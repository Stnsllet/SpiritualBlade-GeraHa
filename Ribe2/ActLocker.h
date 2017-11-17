
#ifndef __ACT_LOCKER__
#define __ACT_LOCKER__

class ActLocker
{
public:
	class Obj;
	ActLocker():pRef(NULL){}
	ActLocker( Obj& Lock ):pRef(NULL){}
	~ActLocker(){Release();}
	bool Lock( Obj& Lock )
	{
		Release();
		pRef = &Lock;
		++pRef->Count;
		return true;
	}
	bool Release()
	{
		if( pRef == NULL )return false;
		--pRef->Count;
		pRef = NULL;
		return true;
	}
	class Obj
	{
	public:
		Obj():Count(0){}
		bool IsLock() const{return (Count>0);}
	private:
	friend class ActLocker;
		long Count;
	};
private:
	Obj* pRef;
};

#endif //__ACT_LOCKER__
