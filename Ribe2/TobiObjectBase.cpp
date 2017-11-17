// TobiObjectBase.cpp: TobiObjectBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbCache.h"
#include "RbSound.h"
#include "DispStocker.h"
#include "TobiObjectBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiObjectBase::TobiObjectBase()
: System( GetBattleSystem() )
{
	AttackPow = 1.0f;
	DmgWait = 500;
	Life = 0;
	bPlanePoly = true;

	NEW_CHECK(this);
}

TobiObjectBase::~TobiObjectBase()
{
}

bool TobiObjectBase::SetResource()
{
	return true;
}

// ポリゴン番号
bool TobiObjectBase::SetPolygon( long PolyID )
{
	if( PolyID <= 0 )return false;
	pPoly = GetCache().Poly.Get( PolyID );
	if( pPoly == NULL )return false;

	float Hight = pPoly->Top()-pPoly->Bottom();
	bPlanePoly = (Hight>-0.01f)&&(Hight<0.01f);
	return true;
}

// ポリゴン番号
long TobiObjectBase::GetPolyID( LPCTSTR name ) const
{
	return GetCache().Poly.GetID( name );
}

bool TobiObjectBase::PlayWave( long WaveID, const D3DVECTOR& Pos ) const
{
	return SUCCEEDED(GetSoundSystem().Play( WaveID, Pos ));
}

long TobiObjectBase::GetWaveID( LPCTSTR name ) const
{
	return GetSoundSystem().GetID(name);
}

// 仕事
bool TobiObjectBase::Job( BattleSystem& System )
{
	if( (--Life) > 0 )return true;
	Damege( System );

	return false;
}

// スキル効果設定
bool TobiObjectBase::SetEffect( const SkillEffect& Effect )
{
	if( Effect.Pow > 0 )AttackPow = Effect.Pow;
	return true;
}

// データセット
bool TobiObjectBase::SetSource( RbActStatus& Source )
{
	return SetTagetPos( &Source, Source.GetPosition(), Source.GetPosition() );
}

// データセット
bool TobiObjectBase::SetTaget( RbActStatus* pSource, RbActStatus* pTaget )
{
	if( pTaget == NULL )return false;
	if( pSource == NULL )return false;
	RbPosition BasePos = pSource->GetPosition();
	BasePos.z += pSource->Hight()/2;
	RbPosition TagetPos = pTaget->GetPosition();
	TagetPos.z += pTaget->Hight()/2;
	return SetTagetPos( pSource, BasePos, TagetPos );
}

// データセット
bool TobiObjectBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	return false;
}

// びょうしゃ
void TobiObjectBase::Draw( Display& Disp )
{
}

// びょうしゃ
void TobiObjectBase::DrawPolygon( Display& Disp, const RbTransform& Trans ) const
{
	if( pPoly == NULL )return;
	if( bPlanePoly )
	{
		pPoly->DrawTrans( Disp, Trans );
	}
	else
	{
		// 通常マテリアル
		DispStocker::Material StockMate( Disp );
		StockMate.Set( Disp.RefMaterial() );
		// Z バッファ有効化
		DispStocker::Render StockZ( Disp, D3DRS_ZENABLE );
		StockZ.Set( TRUE );
		// 裏も書く
		DispStocker::Render StockCull( Disp, D3DRS_CULLMODE );
		StockCull.Set( D3DCULL_CCW );
		// 光源はスケーリングしない
		DispStocker::Render Normal( Disp, D3DRS_NORMALIZENORMALS );
		Normal.Set( TRUE );
		pPoly->DrawTrans( Disp, Trans );
	}
}

// ダメージ
void TobiObjectBase::Damege( BattleSystem& System )
{
}

// 新しいヒットか調べる
bool TobiObjectBase::IsNewHit( RbActStatus* pTaget )
{
	if( pTaget == NULL )return false;

	long ID = pTaget->ID();
	if( HitList.find( ID ) != HitList.end() )return false;

	HitList.insert( ID );
	return true;
}
