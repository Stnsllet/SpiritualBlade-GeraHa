// RbBattleStatus.cpp: RbBattleStatus クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PaformanceChecker.h"
#include "StringSplitter.h"
#include "Display.h"
#include "RbCache.h"
#include "LevelUpSystem.h"
#include "RbMember.h"
#include "RbPosition.h"
#include "BillbordPlane.h"
#include "RbWindowSystem.h"
#include "BattleSystem.h"
#include "RbGameData.h"
#include "DispStocker.h"
#include "RbBattleStatus.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

long RbBattleStatus::IDManage = 0;

RbBattleStatus::RbBattleStatus()
: System( GetBattleSystem() )
, PasonalEffect( SkillList.Effect.RefEffect() )
, LeaderEffect( SkillList.Effect.RefLeader() )
{
	AllZero(Status);
	BattleID = ::InterlockedIncrement(&IDManage);
	pDrawAction = NULL;
}

RbBattleStatus::~RbBattleStatus()
{

}

bool RbBattleStatus::DropJob()
{
	RestoreUserData();
	SetPaty( NULL );
	return true;
}

// 位置取得
const RbPosition& RbBattleStatus::GetPosition() const
{
	return Status.Pos;
}

// 頭位置取得
RbPosition RbBattleStatus::GetHeadPosition( Display& Disp ) const
{
	RbPosition Head( 0, -Hight(), 0 );
	Head = Disp.UnViewMat().Trans( Head );
	return Head += Status.Pos;
}

// 表示位置取得
RbRectF RbBattleStatus::GetDisplayRect( Display& Disp ) const
{
	RbPosition LT( -Size()/2, -Hight(), 0 );
	RbPosition RB( Size()/2, 0, 0 );
	LT = Disp.UnViewMat().Trans( LT );
	RB = Disp.UnViewMat().Trans( RB );
	LT += Status.Pos;
	RB += Status.Pos;
	LT = Disp.RefAllMat().Trans( LT );
	RB = Disp.RefAllMat().Trans( RB );

	RbRectF Ret;
	Ret.left = LT.x;
	Ret.top = LT.y;
	Ret.right = RB.x;
	Ret.bottom = RB.y;
	return Ret;
}

bool RbBattleStatus::TexSound( BattleAction Action ) const
{
	if( pTexture == NULL )return false;
	return pTexture->Sound( Action, Status.Pos );
}

bool RbBattleStatus::TexSound( long ActID ) const
{
	if( pTexture == NULL )return false;
	return pTexture->Sound( ActID, Status.Pos );
}

bool RbBattleStatus::UseMp( float Mp )
{
	Mp *= LeaderEffect.MagicForce;
	if(NowStatus.Mp<Mp)return false;
	NowStatus.Mp -= Mp;
	Update();
	return true;
}

bool RbBattleStatus::InRange( const D3DVECTOR& Pos, float Range ) const
{
	RbPosition Length( Status.Pos );
	Length -= Pos;
	if( Pos.z > Status.Pos.z )
	{
		Range += (Pos.z-Status.Pos.z)/1.5f;
	}
	Range += Size()/2;
	Range *= Range;
	return (Range >= Length.Length2());
}

bool RbBattleStatus::SetDelete()
{
	Status.bDelete = true;
	if( !IsDead() )NowStatus.Hp = NowStatus.Mp = 0;
	return true;
}

bool RbBattleStatus::IsDeleteOK() const
{
	if( !Status.bDelete )return false;
	if( !Status.bLeader )return true;
	if( pPaty == NULL )return true;
	return !(pPaty->IsAlivePaty());
}

float RbBattleStatus::GetComboRate() const
{
	return 1.0f+(Status.Combo*System.ComboRate);
}

void RbBattleStatus::SetPaty( RbBattlePaty* pPatyData )
{
	pPaty = pPatyData;
	if( pPaty == NULL )return;
	if( NULL == pPaty->GetLeader()
		|| this == pPaty->GetLeader() )Status.bLeader = true;
	Status.ArmyID = pPaty->GetArmyID();
}

bool RbBattleStatus::SetAngle( float NewAngle )
{
	Status.Direction = AClip( NewAngle );
	return true;
}

float RbBattleStatus::GetLinerAngle( float Range, float Dips, float Width )
{
	BattleSystem::CharaList List;
	System.GetCharaList( List, Status.Pos, Range, GetArmyID() );
	return System.GetDirection( List, Status.Pos, Dips, Width, Status.Direction );
}

bool RbBattleStatus::SetActImage( long ActionID, float Direction, int Time )
{
	pDrawAction = NULL;
	if( ActionID <= 0 )return false;
	if( pTexture == NULL )return false;
	Direction = AClip( Direction );
	pDrawAction = &(pTexture->GetAction( ActionID, Direction, Time ));
	return true;
}

const RbBattleStatus::ActTime* RbBattleStatus::GetNowImage() const
{
	if( pDrawAction != NULL )return pDrawAction;
	if( pTexture == NULL )return NULL;

	return &(pTexture->GetBattleAction( Status.Action, Status.Direction, Status.ActTime ) );
}

HRESULT RbBattleStatus::Draw( Display& Disp, const RbTransform& AllProj )
{
	if( pTexture == NULL )return E_FAIL;
	if( Status.bDelete )return S_FALSE;

	if( Status.FlyHight > 0 )DrawShadow( Disp, Status.Pos );

	if( pDrawAction == NULL )
	{
		pDrawAction = GetNowImage();
	}
	if( pTexture == NULL || pDrawAction == NULL )return E_FAIL;

	return DrawColor( Disp, *pDrawAction, Status.Pos, AllProj );
}

HRESULT RbBattleStatus::DrawShadow( Display& Disp, const D3DVECTOR& Pos )
{
	RefPtr<RefBord> pShadow = System.GetShadowBord();
	if( pShadow == NULL )return S_FALSE;

	DispStocker::Render ZWrite( Disp, D3DRS_ZENABLE);//D3DRS_ZWRITEENABLE );
	ZWrite.Set( FALSE );

	RbPosition DrawPos( Pos );
	DrawPos.z -= Status.FlyHight;

	RbTransform Trans;
	Trans.AddScroll( DrawPos );
	DispStocker::Transform WorldTrans( Disp, D3DTS_WORLD );
	WorldTrans.Set( Trans );
	return pShadow->Draw( Disp );

//	Disp->SetRenderState( D3DRS_ZBIAS ,16 );
//	Draws::Bord Drawer( Disp );
//	return Drawer.Draw( *pShadow, DrawPos );
}

HRESULT RbBattleStatus::DrawColor( Display& Disp, const ActTime& TimeData, const D3DVECTOR& Pos, const RbTransform& AllProj )
{
	static Material OverDriveMate( ARGBF(1,.3f,.3f,0),100 );
	if( IsOverDrive() )
	{
		DispStocker::TexRender TexStock( Disp, D3DTSS_COLOROP );
		TexStock.Set( D3DTOP_ADD );
		DispStocker::Material Stock( Disp );
		Stock.Set( OverDriveMate );
		return DrawPos( Disp, TimeData, Pos, AllProj );
	}

	return DrawPos( Disp, TimeData, Pos, AllProj );
}

HRESULT RbBattleStatus::DrawPos( Display& Disp, const ActTime& TimeData, const D3DVECTOR& Pos, const RbTransform& AllProj )
{
	if( TimeData.PlaneList.size() <= 0 )return E_FAIL;
	static const float InScale = 1.5f;
	static const RbRectF CheckRect( -1.0f*InScale,-1.0f*InScale,
		2.0f*InScale,2.0f*InScale );

	DispStocker::Transform Proj( Disp, D3DTS_PROJECTION );
	RbPosition ProjPos = AllProj.Trans( Pos );
	if( ProjPos.z < Disp.ViewPort().MinZ )return S_FALSE;
	if( !CheckRect.Pin( ProjPos.x, ProjPos.y ) )return S_FALSE;

//	float ZAngle = -::cosf( Disp.RefDispInfo().UpView.Angle * D3DX_PI / 180.0f );
	float ZAngle = (Disp.RefDispInfo().UpView.Angle-90.0f)/90.0f;

	RbTransform ZShift;
	ZShift._23 = ZAngle/3.0f;
	ZShift._43 = -ProjPos.y*ZShift._23;
	RbTransform NewProj = Proj.OldTrans;
	NewProj *= ZShift;
	Proj.Set( NewProj );

	DispStocker::Transform View( Disp, D3DTS_WORLD );
	RbTransform Trans = Disp.UnViewMat();
	Trans.AddScroll( Pos );
	View.Set( Trans );

	long Count = TimeData.PlaneList.size();
	long ZShitUp = 4;
	if( Count > 5 )
		if( Count > 15 )ZShitUp = 1;
		else ZShitUp = 16/Count;

	if( Disp.ZBiasOK() )
		for( int i = 0; i < Count; i++ )
		{
			Disp->SetRenderState( D3DRS_ZBIAS ,i*ZShitUp );
			Draw2( Disp, TimeData.PlaneList[i] );
		}
	else
		for( int i = 0; i < Count; i++ )
		{
			DispStocker::Transform Proj( Disp, D3DTS_PROJECTION );
			RbTransform ZTrans( Proj.OldTrans );
			ZTrans.AddScroll( RbPosition(0,0,-(i*ZShitUp)*Disp.RefZposRate()) );
			Proj.Set( ZTrans );
			Draw2( Disp, TimeData.PlaneList[i] );
		}

	return S_OK;
}

HRESULT RbBattleStatus::Draw2( Display& Disp, const TexturePlane& SourcePlane )
{
	if( !SourcePlane.bItem )
	{
		RefPtr<RefBord> pRef = SourcePlane.GetRefBord();
		BillbordPlane& Ref = *pRef;
		return Ref.Draw( Disp );
	}

	DispStocker::Render ZEnable( Disp, D3DRS_ZFUNC );
	ZEnable.Set( D3DCMP_ALWAYS );
	BillbordPlane DrawPlane;
	GetWeponImage( DrawPlane );
	if( SourcePlane.Rect.w < 0 )
	{
		float px = -(DrawPlane.Ver[0].x+DrawPlane.Ver[1].x);
		if( px != 0 )DrawPlane.Shift( px, 0 );
		DrawPlane.MirrorX();
	}
	if( SourcePlane.Rect.h < 0 )
	{
		float py = -(DrawPlane.Ver[0].y+DrawPlane.Ver[3].y);
		if( py != 0 )DrawPlane.Shift( 0, py );
		DrawPlane.MirrorY();
	}

	DrawPlane.Rotate( SourcePlane.Rect.angle );
	DrawPlane.Shift( SourcePlane.Rect.px, SourcePlane.Rect.py );

	return DrawPlane.Draw( Disp );
}

HRESULT RbBattleStatus::ResetData()
{
	float StockHp = NowStatus.Hp;
	float StockMp = NowStatus.Mp;
	MemberStatus Stock = SourceStatus;
	HRESULT ret = SetData( pPaty, Stock );
	NowStatus.Hp = StockHp;
	NowStatus.Mp = StockMp;
	return ret;
}

HRESULT RbBattleStatus::SetData( RbBattlePaty* pPatyData, const MemberStatus& Member )
{
	SetPaty( pPatyData );

	RefPtr<RbBattleStatus> pLeader;
	if( pPatyData != NULL )
	{
		Status.bUserData = pPatyData->IsUserData();
		pLeader = pPatyData->GetLeader();
	}

	// 基本ステータス設定
	if( pLeader == NULL )SetStatus( Member );
	else SetStatus( Member, pLeader->SourceStatus );

	// スキル設定
//	SkillList.Effect.SetSkillEffect( SkillList );
	if( pLeader == NULL )SkillList.Effect.SetLeaderEffect( SkillList );
	else SkillList.Effect.SetLeaderEffect( pLeader->SkillList );

	RbMember Data( Member );
	const JobStatus& Job = Data.JobData();
	Status.BattleType = Job.BattleType;
	Status.WeponRandom = Job.WeponRandom;

	if( pPatyData != NULL )
	{
		Status.EscapeHp = AllStatus.Hp * pPatyData->GetEscapeLife();
	}
	return S_OK;
}

bool RbBattleStatus::ChangeBatType ( BattleAlgoType NewType )
{
	Status.BattleType = NewType;
	return true;
}

bool RbBattleStatus::IsTexAct( long ActID ) const
{
	if( pTexture == NULL )return false;
	return pTexture->IsRegister( ActID );
}

bool RbBattleStatus::IsOverDrive() const
{
	if( IsDead() )return false;
	if( NowStatus.Hp>(AllStatus.Hp*PasonalEffect.OverDriveHp) )return false;
	if( Status.Action == eActRecover )return false;
	if( Status.Action == eActDamege && Status.OldAction == eActRecover )return false;
	return true;
}

bool RbBattleStatus::RestoreUserData()
{
	if( !Status.bUserData )return false;
	return GetGameData().UpDateMemberStatus( SourceStatus );
}
