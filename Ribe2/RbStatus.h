// RbStatus.h: RbStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBSTATUS_H__09AC1987_3D1E_4EC1_9565_EBC635390722__INCLUDED_)
#define AFX_RBSTATUS_H__09AC1987_3D1E_4EC1_9565_EBC635390722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusStruct.h"
#include "RefObject.h"
#include "RbTexture.h"
#include "EquipSkill.h"
class BillbordPlane;
class RefBord;

class RbStatus : public RefAuto
{
friend class DamegeSystem;
public:
	typedef RbTexture::ActTime ActTime;
	RbStatus();
	virtual ~RbStatus();
	String GetPasonName() const;

	void SetStatus( const MemberStatus& Source );
	void SetStatus( const MemberStatus& Source, const MemberStatus& Leader );
	operator MemberStatus&(){return SourceStatus;}
	const long& Level() const{return SourceStatus.Level;}
	const MemberStatus& MemberState() const{return SourceStatus;}
	const DefaultStatus& NowState() const{return NowStatus;}
	const DefaultStatus& AllState() const{return AllStatus;}
	const DefaultStatus& LAppend() const{return LeaderAppend;}
	long GetUpdateID()const{return UpdateID;}
	const ItemStatus& GetItemStatus( long Type ) const;

	long KillUp();
	virtual long GetWeponID() const;
	virtual void GetWeponImage( BillbordPlane& Output ) const;
	virtual RefBord* GetWeponImage2() const;
	virtual const ActTime* GetNowImage() const;

	float HpJob( float AddHp );
	float MpJob( float AddMp );
	bool HpCheck( float Per ) const;
	bool MpCheck( float Per ) const;

	RbTexture* RefTexture()const{return pTexture;}
	float Size() const{return CharactorSize;}
	float Hight() const{return CharactorHight;}
	void Update(){++UpdateID;}

	EquipSkill SkillList;

protected:
	void SetItemStatus();
	void SetLeaderAppend( const DefaultStatus& Source, const DefaultStatus& Leader );
	MemberStatus SourceStatus;
	DefaultStatus LeaderAppend;
	DefaultStatus AllStatus;
	DefaultStatus NowStatus;
	float CharactorSize;
	float CharactorHight;
	RefPtr<RbTexture> pTexture;

	long UpdateID;
};

#endif // !defined(AFX_RBSTATUS_H__09AC1987_3D1E_4EC1_9565_EBC635390722__INCLUDED_)
