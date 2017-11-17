// TobiObjectBase.h: TobiObjectBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIOBJECTBASE_H__AE8A09D7_39D5_49F0_8F37_1E218FA6B37A__INCLUDED_)
#define AFX_TOBIOBJECTBASE_H__AE8A09D7_39D5_49F0_8F37_1E218FA6B37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include "RefObject.h"
#include "RbActStatus.h"
#include "RbPolyPoly.h"

class BattleSystem;
class Display;

class TobiObjectBase : public RefAuto
{
public:
	typedef std::set<long> HitTaget;
	TobiObjectBase();
	virtual ~TobiObjectBase();
	virtual bool Job( BattleSystem& System );
	virtual void Draw( Display& Disp );

	virtual bool SetResource();
	virtual bool SetEffect( const SkillEffect& Effect );
	bool SetSource( RbActStatus& Source );
	virtual bool SetTaget( RbActStatus* pSource, RbActStatus* pTaget );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

protected:
	virtual void Damege( BattleSystem& System );
	void DrawPolygon( Display& Disp, const RbTransform& Trans ) const;
	bool IsNewHit( RbActStatus* pTaget );

	bool SetPolygon( long PolyID );
	long GetPolyID( LPCTSTR name ) const;
	bool PlayWave( long WaveID, const D3DVECTOR& Pos ) const;
	long GetWaveID( LPCTSTR name ) const;
	const float GetWait() const{return (DmgWait*(1.0f + AttackPow)/2.0f);}

	float AttackPow;
	float DmgWait;
	HitTaget HitList;
	RefPtr<RbActStatus> pMember;
	RbPosition Pos;
	RbPosition TagetPos;
	long Life;

	RefPtr<RbPolyPoly> pPoly;
	bool bPlanePoly;
	BattleSystem& System;
};

template<class T> class TobiRefPtr : public RefPtr<T>
{
public:
	TobiRefPtr()
		{(*this)=TobiRefPtr<T>::Create();}
	TobiRefPtr( const SkillEffect& Effect )
		{(*this)=TobiRefPtr<T>::Create(Effect);}

	static T* Create()
	{
		T* p = new T;
		p->SetResource();
		return p;
	}
	static T* Create( const SkillEffect& Effect )
	{
		T* p = Create();
		p->SetEffect( Effect );
		return p;
	}
	RefPtr<T>& operator = (T* pSrc)
	{
		return ( ((RefPtr<T>&)(*this)) = pSrc );
	}
	RefPtr<T>& operator = (const RefPtr<T>& pSrc)
	{
		return ( ((RefPtr<T>&)(*this)) = pSrc );
	}
};

#endif // !defined(AFX_TOBIOBJECTBASE_H__AE8A09D7_39D5_49F0_8F37_1E218FA6B37A__INCLUDED_)
