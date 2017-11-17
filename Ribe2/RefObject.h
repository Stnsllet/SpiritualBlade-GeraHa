// RefObject.h: RefObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFOBJECT_H__BB239DE7_393A_4EE5_B952_4F397C7B0AB1__INCLUDED_)
#define AFX_REFOBJECT_H__BB239DE7_393A_4EE5_B952_4F397C7B0AB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemoryChecker.h"

class LockCounter
{
public:
	class Locker
	{
	public:
		Locker():pCounter(NULL){}
		Locker( LockCounter& Source ):pCounter(NULL){ Lock( Source ); }
		~Locker(){UnLock();}
		bool IsLock() const{return pCounter!=NULL;}
		long Lock(LockCounter& Source){UnLock();pCounter=&Source;return ::InterlockedIncrement(&(pCounter->LockCount));}
		long UnLock(){if(pCounter==NULL)return 0;long Ret = ::InterlockedDecrement(&(pCounter->LockCount));pCounter=NULL;return Ret;}
	private:
		LockCounter* pCounter;
	};
	friend class Locker;
	LockCounter(){LockCount=0;}
	bool IsLock() const{return (LockCount>0);}
	long Count() const{return LockCount;}
private:
	long LockCount;
};

// 参照カウントつきクラス
class RefObject
{
public:
	RefObject(){ RefCount = 0; }
	virtual ~RefObject(){ATLASSERT(RefCount==0);}
	long GetRef() const{return RefCount;}
	bool IsLast() const{return (RefCount<=1);}
	bool IsNoRef() const{return (RefCount<=0);}

	long AddRef(void) {return ::InterlockedIncrement(&RefCount);}
	virtual long Release(void){return ::InterlockedDecrement(&RefCount);}

private:
	long RefCount;
};

// 自動消滅クラス
class RefAuto : public RefObject
{
public:
	RefAuto(){NEW_CHECK(this);}
	virtual ~RefAuto(){DEL_CHECK(this);}
	virtual long Release(void)
	{
		long Count = RefObject::Release();
		if( Count == 0 )delete this;
		return Count;
	}
};

// 参照保持ポインタ
template <class T=RefObject> class RefPtr
{
public:
	RefPtr():pPtr(NULL){}
	RefPtr(T* pSrc)
	{
		if( pSrc != NULL )pSrc->AddRef();
		pPtr=pSrc;
	}
	RefPtr(const RefPtr<T>& pSrc)
	{
		if( pSrc != NULL )pSrc->AddRef();
		pPtr=pSrc;
	}
//	template<class R> RefPtr(const RefPtr<R>& pSrc){pPtr=NULL;*this=pSrc;}
	~RefPtr(){Release();}
	RefPtr<T>& operator = (T* pSrc)
	{
		Release();
		if( pSrc != NULL )pSrc->AddRef();
		pPtr=pSrc;
		return (*this);
	}
	RefPtr<T>& operator = (const RefPtr<T>& pSrc)
	{
		return ( (*this) = (T*)pSrc );
	}
	long Release(void)
	{
		if( pPtr == NULL )return 0;
		long ret = pPtr->Release();
		pPtr = NULL;
		return ret;
	}
	T* Detach(void)
	{
		T* pRet = pPtr;
		pPtr = NULL;
		return pRet;
	}
	T* operator->() const{return pPtr;}
	operator T*() const {return pPtr;}
protected:
	T* pPtr;
};

#include <memory>

template <class T> class MasterPtr : public std::auto_ptr<T>
{
public:
	typedef std::auto_ptr<T> Base;
	MasterPtr(){}
	MasterPtr(Base* pSrc) : Base(pSrc){}
	MasterPtr(T* pSrc) : Base(pSrc){}
	MasterPtr& operator = (Base& pSrc)
	{
		*((Base*)this) = pSrc;
		return *this;
	}
	MasterPtr& Set( T* pSrc )
	{
		*((Base*)this) = Base(pSrc);
		return *this;
	}
	operator T* () const{return get();}
};

#endif // !defined(AFX_REFOBJECT_H__BB239DE7_393A_4EE5_B952_4F397C7B0AB1__INCLUDED_)
