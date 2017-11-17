// RbMember.h: RbMember クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBMEMBER_H__77599B76_5438_475B_90EA_26BD03F15D10__INCLUDED_)
#define AFX_RBMEMBER_H__77599B76_5438_475B_90EA_26BD03F15D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
#include "PasonCache.h"

struct MemberStatus;
struct JobStatus;
class XMLPaser;

class RbMember
{
public:
	RbMember( const MemberStatus& AttachState );
	RbMember( MemberStatus& AttachState );
	~RbMember();

	bool ReadXML( const XMLPaser& MemberXML );
	bool ReadStatusXML( const XMLPaser& StatusXML );
	bool Create( LPCTSTR Pason, LPCTSTR Job, long Level=1 );
	bool Create( long PasonID, LPCTSTR Job, long Level=1 );
	bool SetItem( int Wep, int Body, int Acs );
	const ItemID& RefItem( short Type ) const;
	bool UnEquipAll();

	const PasonSt& PasonData() const;
	const JobStatus& JobData() const;
	CComVariant GetParam( const String& Name ) const;

	bool ExecPow( const ItemID& Item );
	bool SetItem( const ItemID& NewItem );
	bool EquipOK( const String& Type ) const;
	bool EquipOK( long ItemNo ) const;
	float GetCharactorSize() const;
	float GetCharactorHight() const;
	float GetWeopnRange() const;
	String GetTexture() const;
	String GetEquip() const;
	String GetMoveType() const;

	long FirstEmptySkill() const;
	long SkillCount() const;
	long CheckSkill( LPCSTR NameID ) const;
	long GetMonLev( long AddLevel=0 ) const;

	operator MemberStatus&(){return Ref;}
	MemberStatus& Ref;
};

#endif // !defined(AFX_RBMEMBER_H__77599B76_5438_475B_90EA_26BD03F15D10__INCLUDED_)
