// TobiImage.cpp: TobiImage クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DispStocker.h"
#include "BattleSystem.h"
#include "TobiImage.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiImage::TobiImage()
{
	Life = 1000;
	AllZero( ViewInfo );
	bSetTrans = false;
	ImgType = eScaleUp;

	EffectTime = 0;
}

TobiImage::~TobiImage()
{
}

// 仕事
bool TobiImage::Job( BattleSystem& System )
{
	bSetTrans = false;
	EffectTime += System.JobTime();
	switch( ImgType )
	{
	case eScaleUp:
		return JobErase( EffectTime );
	case eMoveHight:
		return JobHightMove( EffectTime );
	}
	return false;
}

bool TobiImage::JobErase( double Passed )
{
	if( Passed > Life )return false;
	ViewInfo.Scale = Passed / Life;
	return true;
}

bool TobiImage::JobHightMove( double Passed )
{
	if( Passed > Life )return false;
	ViewInfo.Hight = ViewInfo.HightMove * Passed;
	return true;
}

bool TobiImage::ResetTrans()
{
	if( bSetTrans )return false;

	StandTrans = RbTransform();
	StandTrans.Scale( ViewInfo.Scale );
	StandTrans.AddScroll( 0, ViewInfo.Hight, 0 );

	bSetTrans = true;
	return true;
}

bool TobiImage::SetErase( RbActStatus& Source )
{
	if( Source.IsDead() )return false;

	ImgType = eScaleUp;
	pMember = &Source;
	Pos = Source.GetPosition();
	ViewInfo.Hight = -Source.Hight()/2;
	Life = 500;

	static long PolyID = 0;
	if( PolyID == 0 )PolyID = GetPolyID( _T("erase") );
	SetPolygon( PolyID );

	PosTrans.AddScroll( Pos );

	return true;
}

bool TobiImage::SetLightBit( RbActStatus& Source )
{
	if( Source.IsDead() )return false;

	ImgType = eMoveHight;
	pMember = &Source;
	Pos = Source.GetPosition();
	Pos.x += Source.Size()*Rndf2(-0.5f,0.5f);

	Life = Rnd2(500,750);
	ViewInfo.Scale = Rndf2(0.3f,1);
	ViewInfo.HightMove = -0.1f*ViewInfo.Scale;

	static long PolyID = 0;
	if( PolyID == 0 )PolyID = GetPolyID( _T("lightbit") );
	SetPolygon( PolyID );

	PosTrans.AddScroll( Pos );

	return true;
}

// びょうしゃ
void TobiImage::Draw( Display& Disp )
{
	if( pPoly == NULL )return;
	ResetTrans();

	DispStocker::Transform Stock( Disp, D3DTS_WORLD );
	RbTransform Trans( StandTrans );
	Trans *= Disp.UnViewMat();
	Trans *= PosTrans;
	Stock.Set( Trans );

	pPoly->Draw( Disp );
}
