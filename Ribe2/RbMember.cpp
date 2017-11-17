// RbMember.cpp: RbMember クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusStruct.h"
#include "Path.h"
#include "ParamXML.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "ShopSystem.h"
#include "LevelUpSystem.h"
#include "RbMember.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbMember::RbMember( MemberStatus& AttachState ) : Ref(AttachState)
{

}

RbMember::RbMember( const MemberStatus& AttachState ) : Ref(const_cast<MemberStatus&>(AttachState))
{
}

RbMember::~RbMember()
{
}

long RbMember::GetMonLev( long AddLevel ) const
{
	if( AddLevel > 0 )return AddLevel;
	return (GetGameData().RefSystemData().MonsterLevel - AddLevel);
}

bool RbMember::ReadXML( const XMLPaser& MemberXML )
{
	String Pason = MemberXML.AttributeString(TAG::PASON);
	String Job = MemberXML.AttributeString(TAG::JOB);
	if( Job.empty() )return GetGameData().GetMember( Pason, Ref );

	long Level = GetMonLev(MemberXML.AttributeLong(TAG::LEV));
	if( !Create( Pason, Job, Level ) )return false;

	long Wep = MemberXML.AttributeLong(TAG::WEP);
	long Body = MemberXML.AttributeLong(TAG::BODY);
	long Acs = MemberXML.AttributeLong(TAG::ACS);
	SetItem( Wep, Body, Acs );

	ParamXML FindXML;
	if( !FAILED(MemberXML.Find(_T("Status"),FindXML)) )ReadStatusXML( FindXML );

	return true;
}

bool RbMember::ReadStatusXML( const XMLPaser& StatusXML )
{
	if( StatusXML.HasAttribute(TAG::LEV) )Ref.Level = GetMonLev(StatusXML.AttributeLong(TAG::LEV));
	const CComBSTR& Job = StatusXML.AttributeString(TAG::JOB);
	if( Job.Length() > 0 )LevelUpSystem(Ref).ClassChange(String(Job));
	long AddLev = StatusXML.AttributeLong(_T("addlev"));
	if( AddLev > 0 )LevelUpSystem(Ref).LevelUp( AddLev );

	Ref.Kill += StatusXML.AttributeLong(TAG::KILL);

	StatusEdit<> Read(Ref.Status);
	Read.SkipReadXML( StatusXML );
	return true;
}

CComVariant RbMember::GetParam( const String& Name ) const
{
	if( Name==_T("pason") )return Ref.PasonName;
	if( Name==_T("job") )return Ref.JobName;
	if( Name==_T("name") )return PasonData().Name;
	if( Name==_T("jobname") )return JobData().Name;
	if( Name==_T("level") )return Ref.Level;
	if( Name==_T("kill") )return Ref.Kill;
	if( Name==_T("wep") )return Ref.Wepon;
	if( Name==_T("body") )return Ref.Body;
	if( Name==_T("acs") )return Ref.Acs;

	return CComVariant();
}

bool RbMember::Create( LPCTSTR Pason, LPCTSTR Job, long Level )
{
	long PasonID = GetCache().Pason.GetPasonID(Pason);
	return Create( PasonID, Job, Level );
}

bool RbMember::Create( long PasonID, LPCTSTR Job, long Level )
{
	if( Job == NULL )return false;
	if( Level <= 0 )return false;
	if( PasonID <= 0 )return false;
	LevelUpSystem Creater( Ref );
	Creater.CreateMember( PasonID, Job, Level );
	return true;
}

bool RbMember::SetItem( int Wep, int Body, int Acs )
{
	const ItemCache& Cache = GetCache().Item;
	Ref.Wepon = Cache.Get( Wep ).id;
	Ref.Body = Cache.Get( Body ).id;
	Ref.Acs = Cache.Get( Acs ).id;

	return true;
}

const ItemID& RbMember::RefItem( short Type ) const
{
	static ItemID Dum = {0,0};
	switch( Type )
	{
	case eWeponTypeItem: return Ref.Wepon;
	case eBodyTypeItem: return Ref.Body;
	case eAcsTypeItem: return Ref.Acs;
	}
	return Dum;
}

const PasonSt& RbMember::PasonData() const
{
	return GetCache().Pason.Get( Ref.PasonName );
}

const JobStatus& RbMember::JobData() const
{
	return GetCache().Job.Get( Ref.JobName );
}

String RbMember::GetMoveType() const
{
	const PasonSt& Pason = PasonData();
	if( !Pason.MoveType.empty() )return Pason.MoveType;

	return JobData().MoveType;
}

String RbMember::GetTexture() const
{
	const PasonSt& Pason = PasonData();
	if( !Pason.Texture.empty() )return Pason.Texture;

	return JobData().Texture;
}

float RbMember::GetCharactorSize() const
{
	RefPtr<RbTexture> pTexture;
	if( !GetCache().GetTexture( GetTexture(), pTexture ) )return 0;
	return pTexture->GetSize();
}

float RbMember::GetCharactorHight() const
{
	RefPtr<RbTexture> pTexture;
	if( !GetCache().GetTexture( GetTexture(), pTexture ) )return 0;
	return pTexture->GetHight();
}

float RbMember::GetWeopnRange() const
{
	return GetCache().Item.Get( Ref.Wepon.Number ).Effect.Range;
}

String RbMember::GetEquip() const
{
	const RbCache& Cache = GetCache();
	const PasonSt& PasonData = Cache.Pason.Get( Ref.PasonName );
	const JobStatus& JobData = Cache.Job.Get( Ref.JobName );

	Path All = JobData.EquipType;
	All += PasonData.EquipType;
	return All;
}

bool RbMember::EquipOK( long ItemNo ) const
{
	return GetShop().EquipOK( ItemNo, GetEquip() );
}

bool RbMember::EquipOK( const String& Type ) const
{
	return GetShop().EquipOK( Type, GetEquip() );
}

bool RbMember::SetItem( const ItemID& NewItem )
{
	switch( NewItem.EquipType )
	{
	default:return false;
	case eWeponTypeItem: Ref.Wepon = NewItem; break;
	case eBodyTypeItem: Ref.Body = NewItem; break;
	case eAcsTypeItem: Ref.Acs = NewItem; break;
	}
	return true;
}

bool RbMember::ExecPow( const ItemID& Item )
{
	return LevelUpSystem(Ref).ExecPow(Item);
}

bool RbMember::UnEquipAll()
{
	return GetShop().UnEquipAll( Ref );
}

long RbMember::FirstEmptySkill() const
{
	for( int i = 0; i<count_of(Ref.Skill); i++ )
		if( Ref.Skill[i].name.IsNone() )return i;
	return -1;
}

long RbMember::SkillCount() const
{
	for( int i = 0; i<count_of(Ref.Skill); i++ )
		if( Ref.Skill[i].name.IsNone() )return i;
	return count_of(Ref.Skill);
}

long RbMember::CheckSkill( LPCSTR NameID ) const
{
	for( int i = 0; i<count_of(Ref.Skill); i++ )
		if( Ref.Skill[i].name == NameID )return i;
	return -1;
}