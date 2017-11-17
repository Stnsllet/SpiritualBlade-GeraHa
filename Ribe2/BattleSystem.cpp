// BattleSystem.cpp: BattleSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "PaformanceChecker.h"
#include "GameSystem.h"
#include "RbCache.h"
#include "RbSprite.h"
#include "RbDisplay.h"
#include "DispStocker.h"
#include "RbWindowSystem.h"
#include "RbEventSystem.h"
#include "LevelUpSystem.h"
#include "DamegeSystem.h"
#include "ExpSystem.h"
#include "StrCommander.h"
#include "BattleSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float BattleSystem::BatIn = 256;
float BattleSystem::BatKeep = 308;
float BattleSystem::FormSize = 48;
float BattleSystem::EscapeRange = 128;
float BattleSystem::EscapeRun = 256;
float BattleSystem::MinScale = 0.5f;
float BattleSystem::MaxScale = 2.0f;
float BattleSystem::ComboRate = 0.01f;
float BattleSystem::SoulRate = 1.0f;

BattleSystem::BattleSystem() : BStatus( *this ), Formation(*this)
{
	BattleTime = OldTime = OverCount = 0;
}

BattleSystem::~BattleSystem()
{
	ClearData();
}

// 最初に表示する位置
D3DXVECTOR3 BattleSystem::GetFirstPos() const
{
	return Land.GetLandCenterPos();
}

// パーティ追加
RbBattlePaty* BattleSystem::AddPaty( const XMLPaser& PatyXML )
{
	String FormName = PatyXML.AttributeString(_T("user"));
	if( FormName.empty() )return BattleTagetSystem::AddPaty( PatyXML );

	return Formation.AddPatys( FormName, PatyXML );
}

// データクリア
void BattleSystem::ClearData()
{
	TobiSys.ClearData();
	TobiPlaneSys.ClearData();
	BStatus.Clear();
	BattleTagetSystem::ClearData();
	BattleTime = OldTime = OverCount = 0;
	Land.ClearData();
}

// 軍団逃走
long BattleSystem::EscapeArmy( long ArmyID )
{
	long Count = 0;
	BPatyMap::const_iterator itr = BattlePatys.begin();
	while( itr != BattlePatys.end() )
	{
		RefPtr<RbBattlePaty> pPaty = (itr++)->second;
		if( pPaty->GetArmyID() != ArmyID )continue;
	//	if( !pPaty->IsAlivePaty() )continue;
		if( pPaty->EscapePaty() )++Count;
	}
	return Count;
}

// モード変更処理
bool BattleSystem::ChangeGameMode( GameMode NewMode )
{
	BStatus.SetDrawCursor(NewMode == eBattleMode);
	if( NewMode != eBattleMode && NewMode != eEventMode )return false;
	RbDisplay& Disp = GetRbDisplay();
	Disp.SetZoom( 0, 0 );
	Disp.SetAngle( 0, 0 );
	return true;
}

// 初期化を行う
HRESULT BattleSystem::InitSystem( Display& Disp )
{
	HRESULT ret = BattleTagetSystem::InitSystem( Disp );
	if( FAILED(ret) )return ret;

	ret = Formation.LoadFile();
	if( FAILED(ret) )return ret;

	Land.InitLand( Disp );
	OldTime = RbTimer();
	BattleTime = OldTime = OverCount = 0;

	TobiSys.Init();

	return S_OK;
}

bool BattleSystem::LoadSystemXML( const XMLPaser& SystemXML )
{
	LevelUpSystem::SetMaxLevel( SystemXML.AttributeFloat(_T("maxlev")) );
	long NextExp = SystemXML.AttributeFloat(_T("nextexp"));
	if( NextExp > 0 )ExpSystem::NextCount = NextExp;
	ComboRate = SystemXML.AttributeFloat(_T("combo"));
	SoulRate = SystemXML.AttributeFloat(_T("soul"));
	if( SoulRate <= 0 )SoulRate = 1.0f;

	long SkillLevel = SystemXML.AttributeLong(_T("skilllev"));
	if( SkillLevel > 0 )SkillCache::MaxLevel = SkillLevel;

	float Scale = SystemXML.AttributeFloat(_T("minscale"));
	if( Scale > 0 )MinScale = Scale;
	Scale = SystemXML.AttributeFloat(_T("maxscale"));
	if( Scale > 0 )MaxScale = Scale;

	BStatus.SetPayMove( SystemXML.AttributeBool(_T("patymove")) );

	XMLPaser FindXML;
	if( !FAILED(SystemXML.Find(_T("Range"),FindXML)) )
	{
		float Value = FindXML.AttributeFloat(_T("batin"));
		if( Value > 0 )BatIn = Value;
		Value = FindXML.AttributeFloat(_T("batkeep"));
		if( Value > 0 )BatKeep = Value;
		Value = FindXML.AttributeFloat(_T("formation"));
		if( Value > 0 )FormSize = Value;
		Value = FindXML.AttributeFloat(_T("tactics"));
		if( Value > 0 )TacticsAlgo::TacticsRange = Value;
		Value = FindXML.AttributeFloat(_T("escape"));
		if( Value > 0 )EscapeRange = Value;
		Value = FindXML.AttributeFloat(_T("runmax"));
		if( Value > 0 )EscapeRun = Value;
	}
	if( !FAILED(SystemXML.Find(_T("BraveRate"),FindXML)) )
	{
		float Up = FindXML.AttributeFloat(_T("up"));
		float Down = FindXML.AttributeFloat(_T("down"));
		DamegeSystem::SetBraveRate( Up, Down );
	}

	ArmyStatus::LoadSystemXML( SystemXML );
	return true;
}

// クリックした位置を求める
D3DXVECTOR3 BattleSystem::GetClickPos( Display& DisplaySystem, int MouseX, int MouseY ) const
{
	return Land.GetClickPos( DisplaySystem, MouseX, MouseY );
}

// 表示中心にする
bool BattleSystem::SetViewOn( long BattleID ) const
{
	return SetViewOn( GetCharactor( BattleID ) );
}

// 表示中心にする
bool BattleSystem::SetViewOn( LPCTSTR Name ) const
{
	return SetViewOn( GetCharactor( Name ) );
}

// 表示中心にする
bool BattleSystem::SetViewOn( RbBattleStatus* pStatus ) const
{
	if( pStatus == NULL ) return false;
	const RbPosition& Pos = pStatus->GetPosition();
	GetRbDisplay().SetViewPos( Pos.x, Pos.y, Pos.z );
	return true;
}

bool BattleSystem::LoackView( LPCTSTR Name, double Life ) const
{
	return LoackView( GetCharactor( Name ), Life );
}

bool BattleSystem::LoackView( RbBattleStatus* pStatus, double Life ) const
{
	if( pStatus == NULL )return false;
	RbPosition Pos = pStatus->GetPosition();
	Pos.y -= pStatus->Hight();
	GetRbDisplay().LockView( Pos, Life );
	return true;
}

RefBord* BattleSystem::GetShadowBord()
{
	if( pShadowBill != NULL )return pShadowBill;

	Plane<> Ref = GetCache().SystemPlane.GetPlane(_T("shadow"));
	pShadowBill = new RefBord( Ref, Ref );

	return pShadowBill;
}

// 処理を行う
HRESULT BattleSystem::Job()
{
	PEFORMANCE("Battle Job");
	RbTimer NewTime;
	double Passed = NewTime.Passed( OldTime );
	DWORD CheckTime = JobTime();
	if( BStatus.IsSpeedMode() )CheckTime /= 4;

	if( Passed > 500 )
	{
		ATLTRACE(_T("Battle System Over Time %.1f(ms)\n"), Passed );
		double SkipTime = CheckTime;
		if( OverCount++ >= 3 )SkipTime = 100;
		Passed = CheckTime;
		OldTime = NewTime - CheckTime;
	}
	else OverCount = 0;

	RbEventSystem& EventSys = GetEventSystem();

	while( Passed >= CheckTime )
	{
		Passed -= CheckTime;
		OldTime += CheckTime;
		if( !IsBattleLock() )
		{
			BattleTime += JobTime();

			BCharaMap::const_iterator itr = BattleCharactors.begin();
			while( itr != BattleCharactors.end() )
			{
				(*itr)->Job( *this );
				itr++;
			}
			DeleteDead();
			TobiSys.Job( *this );
		}
		EventSys.Job();
	}
	DeleteUnUsePaty();
	return S_OK;
}

void BattleSystem::DeleteDead()
{
	BCharaMap::iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( (*itr)->IsDeleteOK() )itr = BattleCharactors.erase(itr);
		else itr++;
	}
}

bool BattleSystem::KeyJob( int KeyCode, bool bPress )
{
	if( bPress )return false;
	switch( KeyCode )
	{
	case VK_SHIFT: case VK_TAB:
		BStatus.SpeedChange();
		return true;
	case VK_RETURN: case VK_CONTROL:
		BStatus.WaitChange();
		return true;
	}
	return false;
}

bool BattleSystem::MouseJob( RbDisplay& Disp, GameMessage& State )
{
	if( Land.Edit.IsEditMode() )
		if( Land.MouseJob( Disp, State ) )return true;

	if( BStatus.IsMoveMode() )if( BStatus.MoveMouseJob( Disp, State ) )return true;

	float BaseZoom = Disp.DestZoom();
	RefPtr<RbActStatus> pChara = GetPosCharactor( Disp, State.x, State.y );
	switch( State.Msg() )
	{
	case eLDblClick:
		if( pChara == NULL )break;
		if( pChara->IsUserData() && !BStatus.IsMoveMode() )
		{
			pSelectCharactor = pChara;
			BStatus.MoveChange();
			return true;
		}
	case eRDblClick:
		if( pChara == NULL )break;
		if( pChara->AlgoMove.IsUnRadar() )break;
		StrCommander().ShowBattleStatus( pChara );
		return true;
	case eLDown:
		if( pChara != NULL && pChara->IsUserData() )
		{
			pSelectCharactor = pChara;
		}
		return true;
	case eScrollUp:
		BaseZoom = Trim( BaseZoom+(MaxScale-MinScale)/5, MaxScale, MinScale );
	//	Disp.ChangeDisplayBaseScale( BaseZoom );
		Disp.SetZoom( BaseZoom );
		return true;
	case eScrollDown:
		BaseZoom = Trim( BaseZoom-(MaxScale-MinScale)/5, MaxScale, MinScale );
	//	Disp.ChangeDisplayBaseScale( BaseZoom );
		Disp.SetZoom( BaseZoom );
		return true;
	}
	return Land.MouseJob( Disp, State );
}

// 描写
HRESULT BattleSystem::DrawMain( Display& Disp, const GameMessage& MouseState )
{
	if( !Disp )return E_POINTER;
	if( BStatus.IsBlackOut() )
	{
		Disp->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ARGB(0), 1.0f,0);
		return S_FALSE;
	}
	PEFORMANCE("Battle Draw");

	HRESULT ret = S_OK;
	{
		PEFORMANCE("BatLand Draw");
		ret = Land.Draw( Disp );
	}
	{
		DispStocker::TexUV UV( Disp );
		UV.Set( D3DTADDRESS_BORDER );

		ret = BattleDataManager::DrawMain( Disp );
	}
	ret = Land.DrawGhost( Disp );

	if( GetWindowSystem().OnMsg() )
	{
		TobiPlaneSys.Draw( Disp );
		return S_OK;
	}

	DispStocker::TexUV UV( Disp );
	UV.Set( D3DTADDRESS_CLAMP );
	TobiSys.Draw( Disp );

	BStatus.Draw( Disp, MouseState );

	RbSprite Surface;
	ret = Surface.InitSprite( Disp );
	if( FAILED(ret) )return ret;

	TobiPlaneSys.Draw( Disp, Surface );
	DrawHpBar( Disp, Surface );

	return S_OK;
}

void BattleSystem::DrawHpBar( Display& Disp, RbSprite& Surface ) const
{
	POINT DisplaySize = Surface.GetSize();

	DefaultPlanes& Ref = GetCache().SystemPlane;

	Plane<> BarPlane = Ref.GetPlane( _T("hpbar") );
	Plane<> MeterPlane[4];
	MeterPlane[0] = Ref.GetPlane( _T("plybar") );
	MeterPlane[1] = Ref.GetPlane( _T("enebar") );
	MeterPlane[2] = Ref.GetPlane( _T("fribar") );
	MeterPlane[3] = Ref.GetPlane( _T("dwnbar") );

	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		const RbActStatus& State = *(*itr);
		if( !(State.IsLifeBar()) )continue;
		RbPosition Pos = Disp.ViewToDisp( State.GetPosition() );
		Pos.x -= BarPlane.w/2;
		if( Pos.y+BarPlane.h < 0 || Pos.y > DisplaySize.y )continue;
		if( Pos.x+BarPlane.w < 0 || Pos.x > DisplaySize.x )continue;

		float MaxHp = State.AllState().Hp;
		if( MaxHp <= 0 )continue;
		float Hp = State.NowState().Hp;
		long BarID = State.GetArmyID();

		if( BarID >= count_of(MeterPlane) )BarID = 0;
		if( BarID == 0 && State.IsEscapeHp() )BarID = 3;
		Plane<> Meter = MeterPlane[BarID];

		Meter.w *= Hp/MaxHp;
		long x = Pos.x;
		long y = Pos.y;
		Surface.DrawPlane( BarPlane, x, y );
		Surface.DrawPlane( Meter, x+1, y+1 );
	}
}

RbActStatus* BattleSystem::GetPosCharactor( Display& Disp, int x, int y ) const
{
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	for( ;itr != BattleCharactors.end(); itr++ )
	{
		RbActStatus& Ref = *(*itr);
		if( Ref.IsDead() )continue;
	//	if( Ref.IsDeleteOK() )continue;
		if( Ref.GetDisplayRect( Disp ).Pin(x,y) )return *itr;
	}
	return NULL;
}

bool BattleSystem::SetSelectCharactor( RbActStatus* pCharactor )
{
	if( pSelectCharactor != pCharactor)
	{
		if( BStatus.IsMoveMode() )BStatus.MoveChange();
	}
	return BattleTagetSystem::SetSelectCharactor( pCharactor );
}

// 周りに何もない位置取得
RbPosition BattleSystem::GetRndPosition( long ArmyID, float Size ) const
{
	const RbRectF& Area = Land.Map.LandArea();
	long MaxCheck = 512;
	while( --MaxCheck > 0 )
	{
		RbPosition Pos( Rndf2( Area.left+Size, Area.right-Size ), Rndf2( Area.top+Size, Area.bottom-Size ), 0 );
		if( !Land.MoveOK( Pos, Size ) )continue;
		RefPtr<RbActStatus> pEnemy = GetEnemy(Pos,ArmyID);
		if( pEnemy != NULL )if( pEnemy->GetPosition().Length(Pos)<= BatIn )continue;

		Pos.z = Land.GetHight(Pos.x,Pos.z);
		return Pos;
	}
	RbPointF Center = Area.Center();
	return RbPosition(Center.x,Center.y,Land.GetHight(Center.x,Center.y));
}

// 周りに何もない位置取得
RbPosition BattleSystem::GetRndPosition( float Size ) const
{
	const RbRectF& Area = Land.Map.LandArea();
	long MaxCheck = 512;
	while( --MaxCheck > 0 )
	{
		RbPosition Pos( Rndf2( Area.left, Area.right ), Rndf2( Area.top, Area.bottom ), 0 );
		if( !Land.MoveOK( Pos, Size ) )continue;

		Pos.z = Land.GetHight(Pos.x,Pos.z);
		return Pos;
	}
	RbPointF Center = Area.Center();
	return RbPosition(Center.x,Center.y,Land.GetHight(Center.x,Center.y));
}
