// StrParamCom.cpp: StrParamCom クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "Rbtimer.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "BattleSystem.h"
#include "StrParamCom.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

StrParamCom::StrParamCom( const String& Name )
: ParamName( Name ), UserData(GetGameData())
{
}

StrParamCom::StrParamCom( const StringSplitter<>& Commands )
: ParamName( Commands[2] ), UserData(GetGameData())
{
}

StrParamCom::~StrParamCom()
{
}

bool StrParamCom::Param( const StringSplitter<>& Commands )
{
	if( ParamName.empty() )return false;

	const String& Com = Commands[1];

	if( Com==_T("rnd") )return RndParam( Commands );
	if( Com==_T("selchar") )return SelectCharactorParam();
	if( Com==_T("batpos") )return BatPosParam( Commands[3] );

	if( Com==_T("itemname") )return ItemNameParam( Commands[3].ToLong() );
	if( Com==_T("itemtype") )return ItemTypeParam( Commands[3].ToLong() );
	if( Com==_T("member") )return MemberParam( Commands[3], Commands[4] );
	if( Com==_T("leader") )return LeaderParam( Commands[3] );

	if( Com==_T("korank") )return KillRankParam( Commands[3].ToLong() );
	if( Com==_T("batresult") )return BatResultParam( Commands[3] );
	if( Com==_T("batcount") )return BatCountParam( Commands[3], Commands[4] );
	if( Com==_T("nowtime") )return SetParam( CComVariant(RbTimer()) );

	return false;
}

bool StrParamCom::SetParam( const String& Value )
{
	CComBSTR Stock = Value;
	UserOrStr(Stock);
	return UserData.ExecUserParam( ParamName, String(), Stock );
}

bool StrParamCom::SetParam( const CComVariant& Value )
{
	return SetParam( String().SetVariant(Value) );
}

bool StrParamCom::RndParam( const StringSplitter<>& Commands )
{
	static long RndMin = 3;
	long RndMax = Commands.Count()-1;
	if( RndMax < RndMin )return false;

	return SetParam( Commands[Rnd2(RndMin,RndMax)] );
}

bool StrParamCom::ItemNameParam( long ItemNum )
{
	const ItemCache& Cache = GetCache().Item;
	return SetParam( Cache.Get(ItemNum).Name );
}

bool StrParamCom::ItemTypeParam( long ItemNum )
{
	const ItemCache& Cache = GetCache().Item;
	return SetParam( Cache.GetTypeData(Cache.Get(ItemNum).Type).Name );
}

bool StrParamCom::MemberParam( const String& MemName, const String& DataName )
{
	MemberStatus Mem;
	if( !UserData.GetMember(MemName,Mem) )return false;

	RbMember Data(Mem);
	const CComVariant& GetParam = Data.GetParam( DataName );
	return SetParam( String().SetVariant( GetParam ) );
}

bool StrParamCom::KillRankParam( long KillRank )
{
	const String& Name = UserData.GetKillRank( KillRank );
	if( Name.empty() )return false;

	return SetParam( Name );
}

bool StrParamCom::BatCountParam( const String& Army, const String& Type )
{
	const ArmyStatus& Status = GetBattleSystem().ArmyData(Army);

	if( Type == _T("dead") ) return SetParam( Status.RefCounter().Dead );
	if( Type == _T("entry") ) return SetParam( Status.RefCounter().Enter );
	if( Type == _T("live") ) return SetParam( Status.RefCounter().Live );

	return false;
}

bool StrParamCom::LeaderParam( const String& PatyNo )
{
	long Paty = -1;
	if( PatyNo == _T("ex") )Paty = UserData.RefExPatyNum();
	else Paty = PatyNo.ToLong();
	MemberStatus Mem;
	if( !UserData.GetMember( Paty, 0, Mem ) )return false;

	return SetParam( Mem.PasonName );
}

bool StrParamCom::BatResultParam( const String& Type )
{
	const BattleSystem& System = GetBattleSystem();
	const BattleSystemStatus::ResultData& Result = System.BStatus.RefResult();
	if( Type == _T("combo") )return SetParam( Result.MaxCombo );
	if( Type == _T("time") )return SetParam( System.GetBattleTime() );
	if( Type == _T("ko") )return SetParam( Result.GetKill(System) );
	if( Type == _T("medical") )return SetParam( Result.GetMedical(System) );
	if( Type == _T("bonus") )return SetParam( Result.Bonus );

	return false;
}

bool StrParamCom::BatPosParam( const String& Name )
{
	const BattleSystem& Battle = GetBattleSystem();
	RefPtr<RbActStatus> pChara = Battle.GetCharactor( Name );
	if( pChara == NULL )return false;
	const BattleStatus& Ref = pChara->RefBattleStatus();
	String Pos;
	Pos.Format( "%.3f/%.3f/%.3f", Ref.Pos.x, Ref.Pos.y, Ref.Direction );
	return SetParam( Pos );
}

bool StrParamCom::SelectCharactorParam()
{
	const BattleSystem& Battle = GetBattleSystem();
	RefPtr<RbActStatus> pChara = Battle.GetSelectCharactor();
	if( pChara == NULL )return false;
	return SetParam( pChara->MemberState().PasonName );
}
