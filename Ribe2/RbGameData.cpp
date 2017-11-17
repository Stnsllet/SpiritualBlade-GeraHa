// RbGameData.cpp: RbGameData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "RbMember.h"
#include "RbPaty.h"
#include "RbCache.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "StrCommander.h"
#include "RbGameData.h"
#include "ShopSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

bool UserOrStr( String& InOut )
{
	if( InOut.length() <= 1 )return false;
	if( ((LPCTSTR)InOut)[0] != _T('=') )return false;
	InOut.SetVariant( GetGameData().GetUserParam( &( ((LPCTSTR)InOut)[1] ) ));
	return true;
}

bool UserOrStr( CComBSTR& InOut )
{
	if( InOut.Length() <= 1 )return false;
	if( InOut[0] != L'=' )return false;
	CComVariant Value = GetGameData().GetUserParam( String(&InOut[1]) );
	InOut.Empty();
	if( FAILED(Value.ChangeType(VT_BSTR)) )return true;
	InOut.Attach( Value.bstrVal );
	Value.bstrVal = NULL;
	return true;
}

RbGameData::RbGameData()
{
	AllZero( SysData );
	SysData.MonsterLevel = 1;
}

RbGameData::~RbGameData()
{
}

bool RbGameData::LoadSystemXML( const XMLPaser& ReadXML )
{
	SysData.MoneyUnit = ReadXML.AttributeString(_T("money"));
	if( SysData.MoneyUnit.empty() )SysData.MoneyUnit = _T("E");
	SysData.MaxPaty = ReadXML.AttributeLong(_T("maxpaty"));
	if( SysData.MaxPaty <= 0 )SysData.MaxPaty = 8;
	SysData.MaxStock = ReadXML.AttributeLong(_T("maxstock"));
	if( SysData.MaxStock <= 0 )SysData.MaxStock = 20;

	StringSplitter<> Locks( String(ReadXML.AttributeString(_T("lock"))) );
	for( int i = 0; i < Locks.Count(); i++ )
	{
		LockMembers.insert( Locks[i] );
	}
	return true;
}

bool RbGameData::XMLRead( const XMLPaser& ReadXML )
{
	long AddMoney = ReadXML.AttributeLong(_T("money"));
	Money( AddMoney );

	XMLList MemberListXML;
	MemberListXML.Query( ReadXML, TAG::MEMBER );
	long Count = MemberListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ParamXML Ref = MemberListXML.GetItem(i);
		String UnView = Ref.AttributeString(_T("unview"));
		String ReView = Ref.AttributeString(_T("review"));
		if( !UnView.empty() )UnViewMember( UnView );
		else if( !ReView.empty() )ReViewMember( ReView );
		else AddMember( Ref );
	}
	XMLList GetListXML;
	GetListXML.Query( ReadXML, _T("Get") );
	Count = GetListXML.GetCount();
	for( i = 0; i < Count; i++ )
	{
		GetXML( ParamXML(GetListXML.GetItem(i)) );
	}

	long MonLev = ReadXML.AttributeLong(_T("monlev"));
	if( MonLev > 0 )SysData.MonsterLevel = MonLev;

	String NewStageName = ReadXML.AttributeString(_T("stage"));
	if( !NewStageName.empty() )SetStageName( NewStageName );

	UpDate();
	return true;
}

bool RbGameData::GetXML( const XMLPaser& ReadXML )
{
	String AddMoney = ReadXML.AttributeString(_T("money"));
	UserOrStr(AddMoney);
	if( !AddMoney.empty() )Money( AddMoney.ToLong() );
//	long AddMoney = ReadXML.AttributeLong(_T("money"));
//	Money( AddMoney );

	String AddItemNum = ReadXML.AttributeString(_T("item"));
	UserOrStr(AddItemNum);
	if( !AddItemNum.empty() )AddItem( AddItemNum.ToLong() );
//	long AddItemNum = ReadXML.AttributeLong(_T("item"));
//	AddItem( AddItemNum );

	if( ReadXML.AttributeBool(_T("result")) )
		GetBattleSystem().BStatus.RefResult().SetResult(*this);

	return true;
}

bool RbGameData::SetStageName( LPCTSTR NewName )
{
	if( NewName == NULL )return false;
	StageName = NewName;
	return true;
}

bool RbGameData::IsLockMember( const String& Name ) const
{
	return (LockMembers.find(Name)!=LockMembers.end());
}

// デバッグパラメータ取得
RbParameter& RbGameData::TempParam( const String& name )
{
	LPCTSTR ParamName = &((LPCTSTR)name)[6];
	return TempParams[ ParamName ];
}

// パラメータ取得
CComVariant RbGameData::GetUserParam( const String& ParamName ) const
{
	if( ParamName.empty() )return CComVariant();

	if( !RbParameter::IsSysCommand(ParamName) )return RbUserData::GetUserParam(ParamName);
	if( ParamName==_T("_last") )return RbUserData::GetUserParam(SysData.LastParam);
	if( ParamName.IsFirst(_T("_temp")) )
		return const_cast<RbGameData*>(this)->TempParam(ParamName );

	if( ParamName==_T("_money") )return GetMoney();
	if( ParamName==_T("_rnd") )return Rndf(1.0f);
	if( ParamName==_T("_monlev") )return RefSystemData().MonsterLevel;
	if( ParamName==_T("_battime") )return GrobalData.BattleTime;
	if( ParamName==_T("_nowpos") )return GrobalData.NowPos;
	if( ParamName==_T("_maxcombo") )return GrobalData.MaxCombo;
	if( ParamName==_T("_stage") )return StageName;
	return CComVariant();
}

// パラメータ実行
bool RbGameData::ExecUserParam( const String& Name, const String& Com, const CComBSTR& Value )
{
	if( Name.empty() )return false;
	if( RbParameter::IsSysCommand(Name) )
	{
		if( Name.IsFirst(_T("_temp")) )return TempParam(Name).GoCommand( Com, Value );
		RbParameter Stock( GetUserParam(Name) );
		return Stock.GoCommand( Com, Value );
	}

	SysData.LastParam = Name;
	return UserParams[Name].GoCommand( Com, Value );
}

bool RbGameData::CacheTexture() const
{
	std::map<long, MemberStatus>::const_iterator itr = MemberList.begin();

	std::set<String> ReadList;
	while( itr != MemberList.end() )
	{
		RbMember Mem((itr++)->second);
		ReadList.insert( Mem.GetTexture() );
	}

	TextureCache& Cache = GetCache().RbTex;
	std::set<String>::const_iterator itr2 = ReadList.begin();
	while( itr2 != ReadList.end() )
	{
		Cache.Cache( *itr2++ );
	}
	return true;
}

long RbGameData::GetEmptyPaty() const
{
	for( int i = 0; i < PatyList.size(); i++ )
	{
		RbPaty Paty(PatyList[i]);
		if( Paty.AddOK() )return i;
	}
	if( i < SysData.MaxPaty )return i;
	return -1;
}

bool RbGameData::PayMoney( long Price )
{
	if( GrobalData.Money < Price )
	{
		SystemPopup( _T("poormoney") );
		return false;
	}
	Money( -Price );
	return true;
}

long RbGameData::SetMaxCombo( long Combo )
{
	if( Combo > GrobalData.MaxCombo )GrobalData.MaxCombo = Combo;
	return GrobalData.MaxCombo;
}

bool RbGameData::AddBattleTime( double AddTime )
{
	GrobalData.BattleTime += AddTime;
	UpDate();
	return true;
}

bool RbGameData::UnViewMember( LPCTSTR Name )
{
	long PasonID = GetPasonID(Name);
	std::map<long, MemberStatus>::iterator itr = MemberList.find( PasonID );
	if( itr == MemberList.end() )return false;

	UnViewMemberList[PasonID] = itr->second;
	DropAllPaty(Name);
	MemberList.erase( itr );
	UpDate();
	return true;
}

bool RbGameData::ReViewMember( LPCTSTR Name )
{
	long PasonID = GetPasonID(Name);
	std::map<long, MemberStatus>::iterator itr = UnViewMemberList.find( PasonID );
	if( itr == UnViewMemberList.end() )return false;

	if( !AddMember(itr->second) )return false;
	UnViewMemberList.erase(itr);
	return true;
}

bool RbGameData::AddMember( const XMLPaser& MemberXML )
{
	MemberStatus ReadData;
	RbMember Read( ReadData );
	if( !Read.ReadXML( MemberXML ) )return false;

	bool ForcePaty = MemberXML.AttributeBool(_T("inpaty"));
	return AddMember( ReadData, ForcePaty );
}

bool RbGameData::AddMember( const MemberStatus& State, bool ForcePaty )
{
	long PasonID = GetPasonID( State.PasonName );
	if( PasonID <= 0 )return false;
	if( MemberList.find( PasonID ) != MemberList.end() )return false;

	MemberList[PasonID] = State;
	if( ForcePaty )
	{
		ChangePaty( State, ExPatyNum );
		ChangePaty( State, GetEmptyPaty() );
	}

	UpDate();
	return true;
}

bool RbGameData::DischargeMember( const String& PasonName )
{
	return DischargeMember( GetPasonID(PasonName) );
}

bool RbGameData::DischargeMember( long PasonID )
{
	std::map<long, MemberStatus>::iterator itr = MemberList.find( PasonID );
	if( itr == MemberList.end() )return false;

	RbMember Mem(itr->second);
	Mem.UnEquipAll();
	DropAllPaty(Mem.Ref.PasonName);
	MemberList.erase( itr );
	UpDate();
	return true;
}

bool RbGameData::ChangeEquip( LPCTSTR MemberName, const ItemID& NewItem )
{
	if( NewItem.EquipType > 4 )return false;

	MemberStatus State;
	if( !GetMember( MemberName, State ) )return false;
	if( NewItem.Number > 0 && GetItemCount( NewItem ) <= 0 )return false;

	RbMember Mem(State);
	if( NewItem.Number != 0 && !Mem.EquipOK( NewItem ) )
	{
		SystemPopup( _T("dontequip") );
		return false;
	}
	if( !Mem.ExecPow(NewItem) )
	{
		if( NewItem.EquipType > 3 )return false;

		ItemID OldItem = Mem.RefItem( NewItem.EquipType );
		AddItem( OldItem, 1 );
		Mem.SetItem( NewItem );
	}
	GetSoundSystem().Play(_T("useitem"));
	AddItem( NewItem, -1 );

	return UpDateMemberStatus( Mem );
}

bool RbGameData::UpDateMemberStatus( const MemberStatus& NewStatus )
{
	long PasonID = GetPasonID( NewStatus.PasonName );
	std::map<long, MemberStatus>::iterator itr = MemberList.find( PasonID );
	if( itr == MemberList.end() )return false;

	itr->second = NewStatus;
	UpDate();
	return true;
}

bool RbGameData::UpDatePatyStatus( long PatyNumber, const PatyStatus& NewData )
{
	if( PatyNumber == ExPatyNum )
	{
		ExPaty = NewData;
		UpDate();
		return true;
	}
	if( PatyNumber < 0 )return false;
	if( PatyNumber >= PatyList.size() )return false;

	PatyList[PatyNumber] = NewData;
	UpDate();
	return true;
}

bool RbGameData::DropAllPaty( LPCTSTR MemberName )
{
	bool bRet = DropPatyMember( MemberName, RefExPatyNum() );
	bRet |= DropPatyMember( MemberName );
	return bRet;
}

bool RbGameData::DropPatyMember( LPCTSTR Name, long PatyID)
{
	if( IsLockMember(Name) )
	{
		SystemPopup(_T("lockmember"));
		return false;
	}

	if( PatyID == ExPatyNum )
	{
		RbPaty Paty(ExPaty);
		bool bRet = Paty.DropMember( Name );
		UpDate();
		return bRet;
	}

	if( PatyID < 0 )PatyID = GetPatyID( Name );
	if( PatyID < 0 )return false;
	if( PatyID >= PatyList.size() )return false;

	RbPaty Paty(PatyList[PatyID]);
	if( !Paty.DropMember(Name) )return false;
	if( Paty.MemberCount() <= 0 )
	{
		std::vector<PatyStatus>::iterator itr = PatyList.begin();
		while( PatyID-- > 0 )itr++;
		PatyList.erase(itr);
	}
	UpDate();
	return true;
}

bool RbGameData::ChangePaty( LPCTSTR Name, long NewPatyNum, long NumberIndex )
{
	MemberStatus Data;
	if( !GetMember( Name, Data ) )return false;
	return ChangePaty( Data, NewPatyNum, NumberIndex );
}

bool RbGameData::ChangePaty( const MemberStatus& Data, long NewPatyNum, long NumberIndex )
{
	if( NewPatyNum == ExPatyNum )
	{
		RbPaty PatyData( ExPaty );
		if( PatyData.IsMaxMember() )
		{
			SystemPopup( _T("memberover") );
			return false;
		}
		return PatyData.AddMember( Data, NumberIndex );
	}
	if( NewPatyNum < 0 )return false;
	if( NewPatyNum >= SysData.MaxPaty )return false;
	if( Data.PasonName.IsNone() )return false;
	long OldPatyNum = GetPatyID( Data.PasonName );

	if( OldPatyNum != -1 && OldPatyNum != NewPatyNum && IsLockMember(Data.PasonName) )
	{
		SystemPopup(_T("lockmember"));
		return false;
	}

	if( NewPatyNum >= PatyList.size() )
	{
		NewPatyNum = PatyList.size();
		PatyStatus Paty;
		RbPaty(Paty).Create();
		PatyList.push_back( Paty );
	}
	RbPaty PatyData( PatyList[NewPatyNum] );
	if( PatyData.IsMaxMember() && OldPatyNum != NewPatyNum )
	{
		SystemPopup( _T("memberover") );
		return false;
	}

	bool bRet = PatyData.AddMember( Data, NumberIndex );
	PatyData.ResetFormation( *this );

	if( OldPatyNum >= 0 && OldPatyNum != NewPatyNum )
	{
		if( !DropPatyMember( Data.PasonName, OldPatyNum ) )return false;
	}

	UpDate();
	return bRet;
}

// Save
HRESULT RbGameData::SaveData( LPCTSTR FileName )
{
	File SaveFile;
	HRESULT ret = SaveFile.Open( FileName, true );
	if( FAILED(ret) )return ret;

	ret = SaveFile.Write( SaveVersion );
	if( FAILED(ret) )return ret;

	ret = SaveFile.Write( GrobalData );
	if( FAILED(ret) )return ret;

	ret = SaveFile.WriteStr( StageName );
	if( FAILED(ret) )return ret;

	ret = StrMapSave( SaveFile, UserParams );
	if( FAILED(ret) )return ret;

	ret = MapSave( SaveFile, HaveItemList );
	if( FAILED(ret) )return ret;

	ret = SaveFile.Write( ExPaty );
	if( FAILED(ret) )return ret;

	ret = SaveFile.WriteArray( PatyList.begin(), PatyList.size() );
	if( FAILED(ret) )return ret;

	ret = MapSave( SaveFile, MemberList );
	if( FAILED(ret) )return ret;

	ret = MapSave( SaveFile, UnViewMemberList );
	if( FAILED(ret) )return ret;

	return S_OK;
}

HRESULT RbGameData::LoadGlobalData( LPCTSTR FileName )
{
	File LoadFile;
	HRESULT ret = LoadFile.Open( FileName );
	if( FAILED(ret) )return ret;

	ret = LoadGlobalData( LoadFile );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// Load
HRESULT RbGameData::LoadGlobalData( File& LoadFile )
{
	Version LoadVersion(1,0,0);
	HRESULT ret = LoadFile.Read( LoadVersion );
	if( FAILED(ret) )return ret;

	FileTime = LoadFile.GetWriteTime();

	if( !(LoadVersion==SaveVersion) )return E_INVALIDARG;
	AllClear();
	SaveVersion = LoadVersion;

	ret = LoadFile.Read( GrobalData );
	if( FAILED(ret) )return ret;

	ret = LoadFile.ReadStr( StageName );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// Load
HRESULT RbGameData::LoadData( LPCTSTR FileName )
{
	File LoadFile;
	HRESULT ret = LoadFile.Open( FileName );
	if( FAILED(ret) )return ret;

	ret = LoadGlobalData( LoadFile );
	if( FAILED(ret) )return ret;

	long Count = 0;
	ret = LoadFile.Read( Count );
	if( FAILED(ret) )return ret;
	for( long i = 0; i < Count; i++ )
	{
		String ReadName;
		ret = LoadFile.ReadStr( ReadName );
		if( FAILED(ret) )return ret;
		RbParameter ReadParam;
		ret = ReadParam.Load( LoadFile );
		if( FAILED(ret) )return ret;
		if( ReadName.empty() )continue;

		UserParams[ReadName] = ReadParam;
	}

	Count = 0;
	ret = LoadFile.Read( Count );
	if( FAILED(ret) )return ret;
	for( i = 0; i < Count; i++ )
	{
		HaveItems Data;
		ret = LoadFile.Read( Data );
		if( FAILED(ret) )return ret;
		HaveItemList[ Data.id ] = Data;
	}

	ret = LoadFile.Read( ExPaty );
	if( FAILED(ret) )return ret;

	Count = 0;
	ret = LoadFile.Read( Count );
	if( FAILED(ret) )return ret;
	PatyList.resize( Count );
	LoadFile.ReadArray( PatyList.begin(), Count );

	Count = 0;
	ret = LoadFile.Read( Count );
	if( FAILED(ret) )return ret;
	for( i = 0; i < Count; i++ )
	{
		MemberStatus Data;
		ret = LoadFile.Read( Data );
		if( FAILED(ret) )return ret;
		long PasonID = GetPasonID( Data.PasonName );
		MemberList[ PasonID ] = Data;
	}

	Count = 0;
	ret = LoadFile.Read( Count );
	if( FAILED(ret) )return ret;
	for( i = 0; i < Count; i++ )
	{
		MemberStatus Data;
		ret = LoadFile.Read( Data );
		if( FAILED(ret) )return ret;
		long PasonID = GetPasonID( Data.PasonName );
		UnViewMemberList[ PasonID ] = Data;
	}

	UpDate();
	CacheTexture();
	return S_OK;
}

String RbGameData::GetKillRank( long Rank ) const
{
	if( Rank <= 0 || Rank > MemberList.size() )return String();

	std::list<Ranking> RankList;

	std::map<long, MemberStatus>::const_iterator itr = MemberList.begin();
	while( itr != MemberList.end() )
	{
		const MemberStatus& Ref = (itr++)->second;
		std::list<Ranking>::iterator ins = RankList.begin();
		while( ins != RankList.end() )
		{
			if( Ref.Kill > ins->Kill )break;
			++ins;
		}
		if( ins == RankList.end() )if( RankList.size() > Rank )continue;
		Ranking Add;
		Add.Kill = Ref.Kill;
		Add.Name = Ref.PasonName;
		RankList.insert( ins, Add );
	}
	if( RankList.size() < Rank )return String();
	std::list<Ranking>::const_iterator ret = RankList.begin();
	while( --Rank > 0 )++ret;
	return ret->Name;
}

bool RbGameData::ClearJob()
{
	ShopSystem& Shop = GetShop();
	std::map<long, MemberStatus>::iterator itr = MemberList.begin();
	while( itr != MemberList.end() )
		Shop.UnEquipAll( (itr++)->second );
	std::map<long,HaveItems> StockItemList = HaveItemList;
	AllClear();
	HaveItemList = StockItemList;
	return true;
}
