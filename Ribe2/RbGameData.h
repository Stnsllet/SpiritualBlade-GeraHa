// RbGameData.h: RbGameData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBGAMEDATA_H__ADC31BE3_713E_4A3A_9095_E8559AAF9542__INCLUDED_)
#define AFX_RBGAMEDATA_H__ADC31BE3_713E_4A3A_9095_E8559AAF9542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbUserData.h"
class XMLPaser;
class File;

class RbGameData : public RbUserData
{
public:
	RbGameData();
	virtual ~RbGameData();

	HRESULT SaveData( LPCTSTR FileName );
	HRESULT LoadGlobalData( LPCTSTR FileName );
	HRESULT LoadGlobalData( File& LoadFile );
	HRESULT LoadData( LPCTSTR FileName );

	bool LoadSystemXML( const XMLPaser& ReadXML );

	bool AddBattleTime( double AddTime );
	bool CacheTexture() const;
	bool XMLRead( const XMLPaser& ReadXML );
	bool GetXML( const XMLPaser& ReadXML );
	bool PayMoney( long Price );
	virtual CComVariant GetUserParam( const String& ParamName ) const;
	bool ExecUserParam( const String& Name, const String& Com, const CComBSTR& Value );

	bool ChangePaty( LPCTSTR Name, long NewPatyNum, long NumberIndex=-1 );
	bool ChangePaty( const MemberStatus& Data, long NewPatyNum, long NumberIndex=-1 );

	bool UnViewMember( LPCTSTR Name );
	bool ReViewMember( LPCTSTR Name );
	bool AddMember( const XMLPaser& MemberXML );
	bool AddMember( const MemberStatus& State, bool ForcePaty=false );
	bool DischargeMember( const String& PasonName );
	bool DischargeMember( long PasonID );
	bool DropAllPaty( LPCTSTR MemberName );
	bool DropPatyMember( LPCTSTR Name, long PatyID=-1);
	bool UpDateMemberStatus( const MemberStatus& NewStatus );
	bool UpDatePatyStatus( long PatyNumber, const PatyStatus& NewData );
	bool ChangeEquip( LPCTSTR MemberName, const ItemID& NewItem );
	bool SetStageName( LPCTSTR NewName );
	String GetKillRank( long Rank ) const;
	bool ClearJob();
	long SetMaxCombo( long Combo );

	struct SystemData
	{
		long MonsterLevel;
		String MoneyUnit;
		long MaxPaty;
		long MaxStock;
		String LastParam;
	};
	const SystemData& RefSystemData() const{return SysData;}
	bool IsLockMember( const String& Name ) const;
protected:
	struct Ranking
	{
		long Kill;
		NamedID Name;
	};
	long GetEmptyPaty() const;
	RbParameter& TempParam( const String& name );

	SystemData SysData;
	std::set<String> LockMembers;
	std::map<String,RbParameter> TempParams;
};

RbGameData& GetGameData();

#endif // !defined(AFX_RBGAMEDATA_H__ADC31BE3_713E_4A3A_9095_E8559AAF9542__INCLUDED_)
