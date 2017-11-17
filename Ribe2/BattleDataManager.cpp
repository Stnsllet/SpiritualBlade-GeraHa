// BattleDataManager.cpp: BattleDataManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "XMLList.h"
#include "RbWindowSystem.h"
#include "DispStocker.h"
#include "BattleDataManager.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BattleDataManager::BattleDataManager()
{
}

BattleDataManager::~BattleDataManager()
{
}

bool BattleDataManager::SetSelectCharactor( RbActStatus* pCharactor )
{
	if( pCharactor == NULL )return false;
	pSelectCharactor = pCharactor;
	return true;
}

// データクリア
void BattleDataManager::ClearData()
{
	pSelectCharactor = NULL;
	BattleArmys.clear();
	DeleteUnUsePaty( true );
	ATLASSERT( BattlePatys.size() <= 0 );
	BattleCharactors.clear();
}

long BattleDataManager::DeleteUnUsePaty( bool bAll )
{
	long Count = 0;
	BPatyMap::iterator itr = BattlePatys.begin();
	while( itr != BattlePatys.end() )
	{
		RbBattlePaty& Ref = *(itr->second);
		if( bAll || Ref.IsDeleteOK() )
		{
			Ref.DropJob();
			itr = BattlePatys.erase(itr);
			Count++;
		}
		else itr++;
	}
	return Count;
}

// 初期化を行う
HRESULT BattleDataManager::InitSystem( Display& Disp )
{
	ClearData();

	return S_OK;
}

// 軍隊取得
ArmyStatus& BattleDataManager::ArmyData( const String& Name )
{
	BArmyMap::iterator itr = BattleArmys.find(Name);
	if( itr != BattleArmys.end() )return itr->second;

	ArmyStatus& Army = BattleArmys[Name];
	Army.SetArmyName( Name );
	return Army;
}

// 治療費計算
long BattleDataManager::GetMedical( long ArmyID ) const
{
	long Medical = 0;
	BArmyMap::const_iterator itr = BattleArmys.begin();
	while( itr != BattleArmys.end() )
	{
		const ArmyStatus& Ref = (itr++)->second;
		if( Ref.RefArmyID() != ArmyID )continue;
		Medical += Ref.GetMedical();
	}
	return Medical;
}

// 描写
HRESULT BattleDataManager::DrawMain( Display& Disp )
{
	SortCharactorList();
	DispStocker::AllTrans Trans( Disp );

	DispStocker::Render StockBias( Disp, D3DRS_ZBIAS );
	DispStocker::Material MaterialStock( Disp );
	MaterialStock.Set( Disp.RefPlaneMaterial() );

	BCharaMap::reverse_iterator itr = BattleCharactors.rbegin();
	while( itr != BattleCharactors.rend() )
	{
		(*itr)->Draw( Disp, Trans.All );
		itr++;
	}

	return S_OK;
}

// ソートする
void BattleDataManager::SortCharactorList()
{
	BCharaMap::iterator Nextitr = BattleCharactors.begin();
	do
	{
		Nextitr = SortCharactor( Nextitr, BattleCharactors.end() );
	}while( Nextitr != BattleCharactors.end() );
}

// ソートする
BattleDataManager::BCharaMap::iterator BattleDataManager::SortCharactor(BCharaMap::iterator First, BCharaMap::iterator Last)
{
	if( First == Last )return Last;
	BCharaMap::iterator itr = First;
	++itr;
	BCharaMap::iterator Next = itr;

	const RbPosition& FirstPos = (*First)->GetPosition();
	while( itr != Last
		&& FirstPos.y < (*itr)->GetPosition().y )itr++;

	if( Next == itr )return Next;

	itr = SortCharactor( Next, itr );
	itr = BattleCharactors.insert( itr, *First );
	BattleCharactors.erase(First);
	return ++itr;
}

// パーティ追加
RbBattlePaty* BattleDataManager::AddPaty( const XMLPaser& PatyXML )
{
	RefPtr<RbBattlePaty> pPaty( new RbBattlePaty );
	if( FAILED(pPaty->SetPaty( PatyXML )) )return NULL;

	return AddPaty( pPaty );
}

// パーティ追加
RbBattlePaty* BattleDataManager::AddPaty( const PatyStatus& Paty, float x, float y, float angle )
{
	RefPtr<RbBattlePaty> pPaty( new RbBattlePaty );
	if( FAILED(pPaty->SetPaty( Paty, x, y, angle )) )return NULL;

	return AddPaty( pPaty );
}

// パーティ追加
RbBattlePaty* BattleDataManager::AddPaty( RbBattlePaty* pPaty )
{
	if( pPaty == NULL )return NULL;

	BattlePatys[ pPaty->ID() ] = pPaty;
	if( pPaty->IsUserData() )GetWindowSystem().AddPatyHp( pPaty->ID() );
	return pPaty;
}

// キャラ追加
RbActStatus* BattleDataManager::AddCharactor( RbBattlePaty* pPaty, const MemberStatus& Member )
{
	RefPtr<RbActStatus> pChara( new RbActStatus );
	pChara->SetData( pPaty, Member );

	BCharaMap::iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( pChara->GetPosition().y >= (*itr)->GetPosition().y )break;
		itr++;
	}
	BattleCharactors.insert( itr, pChara );

	if( pSelectCharactor == NULL && pChara->IsUserData() )
		SetSelectCharactor( pChara );
	return pChara;
}

// 軍団番号取得
long BattleDataManager::GetArmyID( LPCTSTR name ) const
{
	return ArmyStatus::GetArmyID( name );
}

//パーティーを取得
RbBattlePaty* BattleDataManager::GetPaty( long PatyID ) const
{
	BPatyMap::const_iterator itr = BattlePatys.find( PatyID );
	if( itr == BattlePatys.end() )return NULL;
	return itr->second;
}

// 最初に見つかったキャラクタを取得
RbActStatus* BattleDataManager::GetCharactor( LPCTSTR name ) const
{
	if( name == NULL )return NULL;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == name )return &Ref;
	}
	return NULL;
}

// 同じ名前のキャラクタを取得
long BattleDataManager::GetCharactors( CharaList& Output, LPCTSTR name ) const
{
	if( name == NULL )return NULL;
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == name )
		{
			Count++;
			Output.push_back( &Ref );
		}
	}
	return Count;
}

// 不明のポインタからバトルID取得
long BattleDataManager::GetBattleID( RbStatus* pStatus ) const
{
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( pStatus == (RbStatus*)(*itr) )return (*itr)->ID();
		itr++;
	}
	return 0;
}

// キャラクタを取得
RbActStatus* BattleDataManager::GetCharactor( long MemberID ) const
{
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( MemberID == (*itr)->ID() )return (*itr);
		itr++;
	}
	return NULL;
}

// 名前でコールバック
long BattleDataManager::NameCall( MemCallBack* pCallBack, LPCTSTR Name ) const
{
	if( pCallBack == NULL )return 0;
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == Name )
			if( pCallBack->Call( Ref ) )Count++;
	}
	return Count;
}
