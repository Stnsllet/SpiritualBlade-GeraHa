// BattleSystemStatus.cpp: BattleSystemStatus クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "DispStocker.h"
#include "BattleSystem.h"
#include "RbGameData.h"
#include "RbCache.h"
#include "MoveLiner.h"
#include "Draws.h"
#include "RbSound.h"
#include "XMLList.h"
#include "BattleSystemStatus.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const float BattleSystemStatus::ClickNiar2 = 5*5;

BattleSystemStatus::BattleSystemStatus(BattleSystem& Ref)
	:System(Ref)
{
	bPatyMove = true;
	Clear();
}

bool BattleSystemStatus::Clear()
{
	LastCheck = 0;
	AllZero( LastCheckPos );
	Com.bMove = Com.bSpeed = false;
	Com.WaitLocker.UnLock();
	Com.MoveLocker.UnLock();
	Result.Clear();

	AllZero(SysState);
	SysState.bDrawCursor = true;
	return true;
}

bool BattleSystemStatus::SetDrawCursor( bool bNew )
{
	return SysState.bDrawCursor = bNew;
}

BattleSystemStatus::~BattleSystemStatus()
{
}

bool BattleSystemStatus::MoveMouseJob( Display& Disp, GameMessage& State )
{
	if( !IsMoveMode() )return false;
	RefPtr<RbActStatus> pSelChara = System.GetSelectCharactor();
	if( pSelChara == NULL )return false;

	bool bDbl = false;
	switch( State.Msg() )
	{
	case eRDblClick:
		GetSoundSystem().Play(_T("cancel"));
		MoveChange();
		return true;
	case eLDblClick:
		bDbl = true;
	case eLDown:
		GetSoundSystem().Play(_T("click"));
		{
			bool Nier = GetStartPos().NiarXY2( State.ViewPos, ClickNiar2);
			bDbl |= Nier;
			if( !Nier )GetMoveList( State.ViewPos, MoveList );
			if( bDbl )
			{
				pSelChara->AlgoMove.SetMoveList( MoveList,true );
				MoveChange();
			}
			return true;
		}
	}
	return false;
}

RbPosition BattleSystemStatus::GetStartPos() const
{
	if( !!MoveList )return MoveList.last();

	RefPtr<RbActStatus> pSelChara = System.GetSelectCharactor();
	if( pSelChara == NULL )return RbPosition();

	return pSelChara->GetPosition();
}

float BattleSystemStatus::GetMoveSize( const RbPosition& StartPos, const RbPosition& TagetPos, bool bMin ) const
{
	RefPtr<RbActStatus> pSelChara = System.GetSelectCharactor();
	if( pSelChara == NULL )return 32;

	float Size = pSelChara->Size();
	if( bMin )return Size;
	RefPtr<RbBattlePaty> pPaty = pSelChara->GetPaty();
	if( pPaty == NULL )return Size;

	float FormSize = pPaty->GetFormationRange()*1.1f;
	if( !System.Land.MoveOK( TagetPos, FormSize ) )return Size;
	if( !System.Land.MoveOK( StartPos, FormSize ) )return Size;
	return FormSize;
}

bool BattleSystemStatus::GetMoveList( const RbPosition& TagetPos, Move::List& OutList ) const
{
	RbPosition StartPos = GetStartPos();
	if( StartPos.NiarXY2( TagetPos, ClickNiar2 ) )return true;
	float Size = GetMoveSize( StartPos, TagetPos );

	Move::List GetList;
	MoveLiner Liner( System.Land, Size );
	bool bRet = Liner.GetMoveList( GetList, TagetPos, StartPos );
	if( !bRet )
	{
		float Size = GetMoveSize( StartPos, TagetPos, true );
		MoveLiner Liner( System.Land, Size );
		bRet = Liner.GetMoveList( GetList, TagetPos, StartPos );
	}
	OutList.Append( GetList );
	return bRet;
}

// 描写
HRESULT BattleSystemStatus::Draw( Display& Disp, const GameMessage& MouseState )
{
	if( !SysState.bDrawCursor )return S_FALSE;
	RefPtr<RbActStatus> pSelChara = System.GetSelectCharactor();

	RbTimer NewTime;
	if( pSelChara != NULL && NewTime.Passed(LastCheck)>200 )
	{
		LastCheck = NewTime;
		if( !LastCheckPos.NiarXY2(MouseState.ViewPos,9) )
		{
			LastCheckPos = MouseState.ViewPos;
			Move::List PList = MoveList;
			if( GetMoveList( MouseState.ViewPos, PList ) )
			{
				LinePoints.clear();
				while( !!PList )AddPoint( PList.pick() );
			}
		}
	}
	DrawCursor( Disp, MouseState );

	return S_OK;
}

HRESULT BattleSystemStatus::DrawCursor( Display& Disp, const GameMessage& MouseState )
{
	if( pCursor == NULL )pCursor = GetCache().Poly.Get("cursor");
	if( pCursor == NULL )return E_FAIL;

	DispStocker::Render StockZ( Disp, D3DRS_ZENABLE );
	StockZ.Set( FALSE );

	if( System.Land.MoveOK( MouseState.ViewPos, System.FormSize ) )
	{
		pCursor->DrawPos( Disp, MouseState.ViewPos, RbTimer()/1000 );
	}

	if( !IsMoveMode() )return S_OK;

	RefPtr<RbActStatus> pSelChara = System.GetSelectCharactor();
	if( pSelChara == NULL )return S_FALSE;
	Draws::Liner Liner;
	Liner.InitLine( _T("line") );

	DispStocker::Material MaterialStock( Disp );
	MaterialStock.Set( Disp.RefPlaneMaterial() );

	RbPosition PrevPos = pSelChara->GetPosition();
	std::list<LinePos>::const_iterator itr = LinePoints.begin();
	while( itr != LinePoints.end() )
	{
		const LinePos& Ref = *itr++;
		Liner.Draw( Disp, PrevPos, Ref.Pos );
		PrevPos = Ref.Pos;
	//	pCursor->DrawPos( Disp, Ref.Pos, RbTimer()/1000 );
	}

	return S_OK;
}

bool BattleSystemStatus::AddPoint( const RbPosition& Pos, bool bCheck )
{
	if( bCheck )
	{
		std::list<LinePos>::const_iterator itr = LinePoints.begin();
		while( itr != LinePoints.end() )
		{
			const LinePos& Ref = *itr++;
			if( Ref.Pos.NiarXY2( Pos,256 ) )return false;
		}
	}
	LinePos Add;
	Add.Pos = Pos;
	LinePoints.push_back( Add );
	return true;
}

BattleSystemStatus::BComMode::BComMode()
{
	bMove = bSpeed = false;
}

bool BattleSystemStatus::WaitChange()
{
	bool bLock = Com.WaitLocker.IsLock();
	if( bLock )Com.WaitLocker.UnLock();
	else Com.WaitLocker.Lock( System.BattleLock );
	return !bLock;
}

bool BattleSystemStatus::MoveChange()
{
	bool NewMode = !Com.bMove;
	if( NewMode )
	{
		RefPtr<RbActStatus> pChara = System.GetSelectCharactor();
		if( pChara != NULL )if( bPatyMove || !pChara->IsPatyBattle() )pChara = pChara->GetLeader();
		System.SetSelectCharactor( pChara );
		Com.MoveLocker.Lock( System.BattleLock );
	}
	else Com.MoveLocker.UnLock();
	Com.bMove = NewMode;

	MoveList.Clear();
	LinePoints.clear();
	return Com.bMove;
}

bool BattleSystemStatus::ResultData::SetXML( const XMLPaser& ResultXML )
{
	const RbGameData::SystemData& Ref = GetGameData().RefSystemData();
	LPCTSTR KO = _T("ko");
	LPCTSTR MEDICAL = _T("medical");

	if( ResultXML.HasAttribute(KO) )
		Kill = ResultXML.AttributeFloat(KO) * Ref.MonsterLevel;
	if( ResultXML.HasAttribute(MEDICAL) )
		Medical = ResultXML.AttributeFloat(MEDICAL) * Ref.MonsterLevel;
	Bonus += ResultXML.AttributeLong(_T("bonus"));

	long ItemID = ResultXML.AttributeLong(_T("item"));
	if( ItemID > 0 )ItemList.Add( ItemID );

	return true;
}

void BattleSystemStatus::ResultData::Clear()
{
	if( g_pSystem == NULL )
	{
		Kill = 1;
		Medical = -1;
	}
	else
	{
		const RbGameData::SystemData& Ref = GetGameData().RefSystemData();
		Kill = Ref.MonsterLevel;
		Medical = -Ref.MonsterLevel;
	}
	Bonus = 0;
	MaxCombo = 0;
	ItemList.clear();
	bSetDone = false;
}


long BattleSystemStatus::ResultData::GetKill( const BattleSystem& System ) const
{
	long Money = System.GetMedical( 1 );
	Money += System.GetMedical( 2 );
	Money *= Kill;
	return Money;
}

long BattleSystemStatus::ResultData::GetMedical( const BattleSystem& System ) const
{
	long Money = System.GetMedical( 0 );
	Money *= Medical;
	return Money;
}

void BattleSystemStatus::ResultData::SetMaxCombo( long Combo )
{
	if( MaxCombo >= Combo )return;
	MaxCombo = Combo;
}

bool BattleSystemStatus::ResultData::SetResult( RbGameData& UserData )
{
	if( bSetDone )return false;
	const BattleSystem& System = GetBattleSystem();

	long Money = Bonus;
	Money += GetKill(System);
	Money += GetMedical(System);
//	if( Money < 0 )Money = 0;
	UserData.Money( Money );
	UserData.SetMaxCombo( MaxCombo );

	UserData.AddBattleTime( System.GetBattleTime() );

	for( int i = 0; i < ItemList.Count(); i++ )
	{
		UserData.AddItem( ItemList[i] );
	}
	Clear();
	bSetDone = true;
	return true;
}
