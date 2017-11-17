// RbUserData.cpp: RbUserData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "StringSplitter.h"
#include "XMLList.h"
#include "RbMember.h"
#include "RbPaty.h"
#include "RbCache.h"
#include "RbUserData.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbUserData::RbUserData() : SaveVersion( 1,0,0 )
{
	AllClear();
	DataUpDateID = 0;
	ExPatyNum = -5;
}

long RbUserData::UpDate()
{
	return ::InterlockedIncrement( &DataUpDateID );
}

RbUserData::~RbUserData()
{
}

long RbUserData::GetPasonID( const String& PasonName ) const
{
	return GetCache().Pason.GetPasonID( PasonName );
}

HRESULT RbUserData::ReadGlobalFlg()
{
	return ReadGlobalFlg( _T("System/GlobalFlg.xml") );
}
// グローバルフラグ読み込み
HRESULT RbUserData::ReadGlobalFlg( LPCTSTR FileName )
{
	XMLPaser AllFlg;
	HRESULT ret = AllFlg.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList FlgList;
	FlgList.Query( AllFlg, _T("Flg") );
	int Count = FlgList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser Flg;
		FlgList.GetPaser( i, Flg );
		long no = Flg.AttributeLong(TAG::NO);
		if( no == 0 )continue;
		String Name = Flg.AttributeString(TAG::NAME);
		if( Name.empty() )continue;
		ATLASSERT( GlobalFlgID.GetID(Name) < 0 );
		GlobalFlgID.AddID( Name, no );
	}
	return S_OK;
}

// フラグセット
bool RbUserData::SetFlg( const String& Name, bool Flg )
{
	long ID = GlobalFlgID.GetID( Name );
	if( ID > 0 )return SetFlg( GrobalData.EventFlag, ID, Flg );
	return SetFlg( HotFlg, HotFlgID.GetID(Name), Flg );
}

// フラグゲット
bool RbUserData::GetFlg( const String& Name ) const
{
	long ID = GlobalFlgID.GetID( Name );
	if( ID > 0 )return GetFlg( GrobalData.EventFlag, ID );
	return GetFlg( HotFlg, HotFlgID.GetID(Name) );
}

// フラグチェック
bool RbUserData::CheckFlgs( LPCTSTR Names, bool bFlg ) const
{
	StringSplitter<> Split( Names );
	int Count = Split.Count();
	for( int i = 0; i < Count; i++ )
	{
		if( GetFlg(Split[i]) != bFlg )return false;
	}
	return true;
}

// ほっとフラグクリア
bool RbUserData::ClearHotFlg()
{
	AllZero( HotFlg );
	return true;
}

// パラメータ取得
CComVariant RbUserData::GetUserParam( const String& ParamName ) const
{
	std::map<String,RbParameter>::const_iterator itr = UserParams.find( ParamName );
	if( itr == UserParams.end() )return CComVariant();
	return itr->second;
}

// アイテム数取得
long RbUserData::GetItemCount( long ItemNo ) const
{
	std::map<long,HaveItems>::const_iterator itr = HaveItemList.find( ItemNo );
	if( itr == HaveItemList.end() )return 0;
	return itr->second.Count;
}

// アイテム数増減
bool RbUserData::AddItem( long ItemNo, long Count )
{
	if( ItemNo <= 0 )return false;
	std::map<long,HaveItems>::iterator itr = HaveItemList.find( ItemNo );
	HaveItems& Ref = (itr==HaveItemList.end())
		?(AllZero(HaveItemList[ItemNo])):(itr->second);
	Ref.id = ItemNo;
	Ref.Count += Count;
	Ref.Count = Trim( Ref.Count, (long)99, (long)0 );
	if( Ref.Count <= 0 )
	{
		HaveItemList.erase( HaveItemList.find( ItemNo ) );
	}
	return true;
}

// リーダー取得
bool RbUserData::GetLeader( LPCTSTR Name, MemberStatus& Output ) const
{
	long PatyID = GetPatyID( Name );
	if( PatyID < 0 )return GetMember( Name, Output );
	return GetMember( PatyID, 0, Output );
}

// メンバーチェック
bool RbUserData::InMember( const String& Member ) const
{
	return InMember( GetPasonID(Member) );
}

// メンバーチェック
bool RbUserData::InMember( long PasonID ) const
{
	return ( MemberList.find( PasonID ) != MemberList.end() );
}

// 待機メンバー数取得
long RbUserData::GetStockCount() const
{
	std::map<long,MemberStatus>::const_iterator itr = MemberList.begin();
	long Count = 0;
	while( itr != MemberList.end() )
		if( 0 > GetPatyID((itr++)->second.PasonName) )++Count;
	return Count;
}

// メンバー取得
bool RbUserData::GetMember( const String& Name, MemberStatus& Output ) const
{
	return GetMember( GetPasonID(Name), Output );
}

// メンバー取得
bool RbUserData::GetMember( long PasonID, MemberStatus& Output ) const
{
	std::map<long, MemberStatus>::const_iterator itr = MemberList.find( PasonID );
	if( itr == MemberList.end() )return false;

	Output = itr->second;
	return true;
}

// メンバー取得
bool RbUserData::GetMember( long PatyNumber, long MemberNumber, MemberStatus& Output ) const
{
	PatyStatus Paty;
	if( !GetPaty( PatyNumber, Paty ) )return false;
	RbPaty State( Paty );
	if( MemberNumber >= State.MemberCount() )return false;
	return GetMember( Paty.Member[MemberNumber].id, Output );
}

// パーティ取得
bool RbUserData::GetPaty( long PatyNumber, PatyStatus& Output ) const
{
	if( PatyNumber == ExPatyNum )
	{
		Output = ExPaty;
		return true;
	}
	if( PatyNumber < 0 )return false;
	if( PatyNumber >= PatyList.size() )return false;

	Output = PatyList[PatyNumber];
	return true;
}

// パーティ取得
long RbUserData::GetPatyID( LPCTSTR Member ) const
{
	if( Member == NULL )return -1;

	for( int i = 0; i < PatyList.size(); i++ )
	{
		RbPaty Paty(PatyList[i]);
		if( Paty.InMember(Member) )return i;
	}
	return -1;
}

// 特殊パーティ？
bool RbUserData::InExPaty( LPCTSTR Member ) const
{
	return RbPaty(ExPaty).InMember(Member);
}

// 初期化
HRESULT RbUserData::InitData()
{
	AllClear();
	ReadGlobalFlg();

	return S_OK;
}

// すべて消す
void RbUserData::AllClear()
{
	AllZero( GrobalData );
	AllZero( HotFlg );
	RbPaty(ExPaty).Create();
	GrobalData.Money = 0;
	HaveItemList.clear();
	PatyList.clear();
	MemberList.clear();
	UserParams.clear();
	SYSTEMTIME Systime;
	GetLocalTime( &Systime );
	SystemTimeToFileTime( &Systime, &FileTime );
	UpDate();
}

// 現在位置変更
bool RbUserData::SetHousePos( LPCTSTR House )
{
	if( House == NULL )return false;
	GrobalData.NowPos = House;
//	UpDate();
	return true;
}

long RbUserData::Money( long Add )
{
	GrobalData.Money += Add;
	GrobalData.Money = Trim( GrobalData.Money, (long)99999999, (long)0 );
	UpDate();
	return GrobalData.Money;
}
