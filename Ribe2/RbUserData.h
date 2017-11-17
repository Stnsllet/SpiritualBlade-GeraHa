// RbUserData.h: RbUserData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBUSERDATA_H__7BEE6945_5584_460C_8181_5E8AA9E33C23__INCLUDED_)
#define AFX_RBUSERDATA_H__7BEE6945_5584_460C_8181_5E8AA9E33C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include "Version.h"
#include "StatusStruct.h"
#include "StringIDMap.h"
#include "RbParameter.h"

class File;

class RbUserData  
{
public:
	RbUserData();
	virtual ~RbUserData();

	HRESULT InitData();
	void AllClear();
	bool SetHousePos( LPCTSTR House );
	LPCTSTR GetHousePos() const{return GrobalData.NowPos;}

	bool SetFlg( const String& Name, bool Flg=true );
	bool GetFlg( const String& Name ) const;
	bool CheckFlgs( LPCTSTR Names, bool bFlg=true ) const;
	bool ClearHotFlg();

	virtual CComVariant GetUserParam( const String& ParamName ) const;
	long GetItemCount( long ItemNo ) const;
	bool AddItem( long ItemNo, long Count=1 );
	bool InMember( const String& Member ) const;
	bool InMember( long PasonID ) const;
	long GetStockCount() const;
	bool GetLeader( LPCTSTR Name, MemberStatus& Output ) const;
	bool GetMember( long PasonID, MemberStatus& Output ) const;
	bool GetMember( const String& Name, MemberStatus& Output ) const;
	bool GetMember( long PatyNumber, long MemberNumber, MemberStatus& Output ) const;
	bool GetPaty( long PatyNumber, PatyStatus& Output ) const;
	long GetPatyID( LPCTSTR Member ) const;
	long PatyCount() const{return PatyList.size();}
	bool InExPaty( LPCTSTR Member ) const;
	long GetMoney() const{return GrobalData.Money;}
	long Money( long Add );
	const FILETIME& RefFileTime() const{return FileTime;}
	const String& RefStageName() const{return StageName;}
	const std::map<long,HaveItems>& RefHaveItems() const{return HaveItemList;}
	const std::map<long, MemberStatus>& RefMembers() const{return MemberList;}
	long UpDateID() const{return DataUpDateID;}

	long RefExPatyNum() const{return ExPatyNum;}
protected:
	long GetPasonID( const String& PasonName ) const;
	long UpDate();
	HRESULT ReadGlobalFlg();
	HRESULT ReadGlobalFlg( LPCTSTR FileName );

	Version SaveVersion;

	long DataUpDateID;
	FILETIME FileTime;
	BYTE HotFlg[256];
	IDManager HotFlgID;
	StringIDMap GlobalFlgID;
	UserStatus GrobalData;
	String StageName;
	PatyStatus ExPaty;
	std::map<String,RbParameter> UserParams;
	std::map<long,HaveItems> HaveItemList;
	std::vector<PatyStatus> PatyList;
	std::map<long, MemberStatus> MemberList;
	std::map<long, MemberStatus> UnViewMemberList;

	template <class T> HRESULT IteratorSave( File& Save, const T& Data )
	{
		long Size = Data.size();
		HRESULT ret = Save.Write( Size );
		if( FAILED(ret) )return ret;
		T::const_iterator itr = Data.begin();
		while( itr != Data.end() )
		{
			ret = Save.Write( *itr );
			if( FAILED(ret) )return ret;
			itr++;
		}
		return S_OK;
	}
	template <class T> HRESULT MapSave( File& Save, const T& Data )
	{
		long Size = Data.size();
		HRESULT ret = Save.Write( Size );
		if( FAILED(ret) )return ret;
		T::const_iterator itr = Data.begin();
		while( itr != Data.end() )
		{
			ret = Save.Write( itr->second );
			if( FAILED(ret) )return ret;
			itr++;
		}
		return S_OK;
	}
	template <class T> HRESULT StrMapSave( File& Save, const T& Data )
	{
		long Size = Data.size();
		HRESULT ret = Save.Write( Size );
		if( FAILED(ret) )return ret;
		T::const_iterator itr = Data.begin();
		while( itr != Data.end() )
		{
			ret = Save.WriteStr( itr->first );
			if( FAILED(ret) )return ret;
			ret = itr->second.Save( Save );
			if( FAILED(ret) )return ret;
			itr++;
		}
		return S_OK;
	}
	long ExPatyNum;
private:
	template<class T> bool SetFlg( T& FlgBuf, int Num, bool Flg ) const
	{
		if( Num < 0 )return false;
		int BufNum = Num/8;
		if( BufNum >= sizeof(FlgBuf) )return false;
		BYTE Mask = 1<<(Num&7);
		if( Flg ) FlgBuf[BufNum] |= Mask;
		else FlgBuf[BufNum] &= ~Mask;
		return true;
	}
	template<class T> bool GetFlg( const T& FlgBuf, int Num ) const
	{
		if( Num < 0 )return false;
		int BufNum = Num/8;
		if( BufNum >= sizeof(FlgBuf) )return false;
		BYTE Mask = 1<<(Num&7);
		return (FlgBuf[BufNum] & Mask)>0;
	}
};

#endif // !defined(AFX_RBUSERDATA_H__7BEE6945_5584_460C_8181_5E8AA9E33C23__INCLUDED_)
