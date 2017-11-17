// LandScape.cpp: LandScape クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbPosition.h"
#include "RbDisplay.h"
#include "RbSound.h"
#include "XMLPaser.h"
#include "DispStocker.h"
#include "Draws.h"
#include "RbTimer.h"
#include "StringSplitter.h"
#include "LandScape.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandScape::LandScape() : Map( MapData )
{
	LastUpdateTime = 0;
}

LandScape::~LandScape()
{

}

void LandScape::ClearData()
{
	BlockPoly.Clear();
	GlassPoly.Clear();
	GhostPoly.Clear();
	BridgePoly.Clear();
	LandBase::ClearData();
}

bool LandScape::IsUpDate( double CheckTime ) const
{
	if( CheckTime >= LastUpdateTime )return false;
	if( LastUpdateTime+500 > RbTimer() )return false;

	return true;
}

// ランドスケープロード
HRESULT LandScape::LoadLandScape( LPCTSTR FileName )
{
	HRESULT hr = LandBase::LoadLandScape( FileName );
	if( FAILED(hr) )return hr;
	LastUpdateTime = RbTimer();

	Map.SetMapData( GData.Face.x, GData.Face.y,
		GData.ChipSize.y, GData.ChipSize.y );

	return hr;
}

// マウス位置を求める
D3DXVECTOR3 LandScape::GetClickPos( const Display& Disp, int MouseX, int MouseY ) const
{
	float TargetZ = 0;
	D3DXVECTOR3 Pos = Disp.MouseToView( MouseX, MouseY, TargetZ );
	Pos.z = GetHight( Pos.x, Pos.y );

	for( int i = 0; i < 15; i++ )	//最大15回チェック
	{
		D3DXVECTOR3 View = Disp.ViewToDisp( Pos );
		if( View.x < MouseX+2 && View.x > MouseX-2 )
			if( View.y < MouseY+2 && View.y > MouseY-2 )break;
		TargetZ = (Pos.z + TargetZ)/2.0f;
		Pos = Disp.MouseToView( MouseX, MouseY, TargetZ );
		Pos.z = GetHight( Pos.x, Pos.y );
	}
	return Pos;
}

bool LandScape::MouseJob( Display& Disp, GameMessage& State )
{
	if( State == eRDown )
	{
		State.DragStart();
		return true;
	}

	if( State.IsDraging() && State == eMouseMove )
	{
		const RbPosition& NowPos = Disp.GetViewPos();
		RbPosition Old = Disp.MouseToView( State.OldX, State.OldY );
		RbPosition New = Disp.MouseToView( State.x, State.y );
		Disp.ScrollView( Old.x-New.x, Old.y-New.y, 0 );
		const RbPosition& NewPos = Disp.GetViewPos();
		float Hight = GetHight( NewPos.x, NewPos.y );
		Disp.ScrollView( 0, 0, Hight - NowPos.z );
		GetSoundSystem().SetListenerPosition( Disp.GetViewPos() );
		return true;
	}

	if( State == eDrop )
	{
		State.DragEnd();
		return true;
	}
	return false;
}

void LandScape::InitLand( DISPDEVICE* pDevice )
{
	LandBase::InitLand( pDevice );
}

// ランドスケープの中心位置
D3DXVECTOR3 LandScape::GetLandCenterPos() const
{
	return D3DXVECTOR3( (GData.Face.x/2.0f*GData.ChipSize.x)/2.0f,
		(GData.Face.y*GData.ChipSize.y)/2.0f, 0 );
}

// ポリゴン番号を返す
int LandScape::GetFacePosition( float x, float y ) const
{
	const float ChipWidth = GData.ChipSize.x;
	const float ChipHeight = GData.ChipSize.y;
	int ChipY = (int)(y / ChipHeight);

	float OffsetX = ( y-(ChipHeight*ChipY) )/2.0f;
	if( ChipY&1)OffsetX = OffsetX - (ChipWidth/2.0f);
	else OffsetX = - OffsetX;

	int ChipX = (int)((x+OffsetX) / ChipWidth);
	float RectY = y-(ChipHeight * ChipY);
	float RectX = x-(ChipWidth  * ChipX)+OffsetX;

	ChipX *= 2;
	if( ChipY&1 )
	{
		if( RectX < RectY )ChipX += 1;
	}
	else
	{
		if( RectX > ChipHeight-RectY )ChipX += 1;
	}

	if( ChipX < 0 || ChipX >= GData.Face.x )return -1;
	if( ChipY < 0 || ChipY >= GData.Face.y )return -1;

	int RetPos = ChipX + ChipY * GData.Face.x;
	ATLASSERT( RetPos < IndexData.Count() );
	return RetPos;
}

// 文字列で示された位置取得
RbPosition LandScape::GetStrPos( const String& PosString ) const
{
	StringSplitter<> Split(PosString);
	RbPosition Ret(Split[0].ToFloat(), Split[1].ToFloat(), Split[2].ToFloat() );
	Ret.z += GetHight(Ret.x, Ret.y );
	return Ret;
}

// 高さ(z座標)を取得
float LandScape::GetHight( float x, float y ) const
{
	return GetHight( x, y, GetFacePosition( x, y ) );
}

// 高さ(z座標)を取得
float LandScape::GetHight( float x, float y, int FaceNum ) const
{
	if( FaceNum < 0 )return 0.0f;

	const PanelVector& Normal = FaceNormal[FaceNum];
	float z = -Normal.d - Normal.a*x - Normal.b*y;
	if( Normal.c != 0 )z /= Normal.c;

	return z;
}

HRESULT LandScape::Draw( Display& Disp )
{
	if( !Disp )return E_POINTER;
	Disp->SetRenderState( D3DRS_CULLMODE ,D3DCULL_CCW );
	Disp->SetRenderState(D3DRS_LIGHTING,TRUE);
	Disp->SetTextureStageState(0,D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	Disp->SetTextureStageState(0,D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	Disp->SetVertexShader( LandVertex::Def() );

	Disp->SetStreamSource( 0, LandData, LandData.StructSize() );
	std::map<String,SinplePlane>::const_iterator itr = SinpleList.begin();
	while(itr != SinpleList.end()) DrawSinplePlane( Disp, (itr++)->second );

//	BlockPoly.DrawPoly( Disp );

	return S_OK;
}

HRESULT LandScape::DrawGhost( Display& Disp )
{
	BridgePoly.DrawPoly( Disp );
	BlockPoly.DrawPoly( Disp );
	GhostPoly.DrawPoly( Disp );

	return S_OK;
}

HRESULT LandScape::DrawSinplePlane( Display& Disp, const SinplePlane& SinpleData ) const
{
	// テクスチャ設定
	if( !SinpleData.ChipTexture )return S_FALSE;
	Disp->SetTexture( 0, SinpleData.ChipTexture );

	Disp->SetIndices( SinpleData.IndexData, 0 );
	return Disp->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
		0, LandData.Count(), 0, SinpleData.IndexData.Count() );
}

bool LandScape::MoveOK( const RbPosition& Pos, float Size, float LandSize ) const
{
	if( BlockCheck( Pos, Size ) )return false;
	return Map.MoveOK( Pos.x, Pos.y, LandSize );
}

const PolyStation* LandScape::BlockCheck( const RbPosition& Pos, float Size ) const
{
	const PolyStation* pBlock = BlockPoly.BlockCheck( Pos, Size );
	if( pBlock )return pBlock;
	pBlock = GlassPoly.BlockCheck( Pos, Size );
	if( pBlock )return pBlock;
	return NULL;
}

bool LandScape::SetPolygon( const XMLPaser& PolyXML )
{
	LastUpdateTime = RbTimer();

	String Type = PolyXML.AttributeString(_T("type"));
	if( Type==_T("block") )return (NULL!=BlockPoly.AddXML( PolyXML, *this ));
	if( Type==_T("glass") )return (NULL!=GlassPoly.AddXML( PolyXML, *this ));
	if( Type==_T("bridge") )return (NULL!=BridgePoly.AddXML( PolyXML, *this ));
	if( Type==_T("delete") )
	{
		RbPosition Pos( PolyXML.AttributeFloat(_T("x")),
			PolyXML.AttributeFloat(_T("y")),
			PolyXML.AttributeFloat(_T("z")) );
		return (DeletePolygon(Pos)>0);
	}

	return (NULL!=GhostPoly.AddXML( PolyXML, *this ));
}

PolyStation* LandScape::AddPolygon( const String& PolyName,
	const RbPosition& Pos, float Direction, float Scale )
{
	LastUpdateTime = RbTimer();
	return GhostPoly.AddStation( PolyName, *this, Pos, Direction, Scale );
}

long LandScape::DeletePolygon( const RbPosition& Pos )
{
	long Count = 0;
	Count += GhostPoly.Delete( Pos );
	Count += BlockPoly.Delete( Pos );
	Count += GlassPoly.Delete( Pos );
	Count += BridgePoly.Delete( Pos );
	if( Count > 0 )LastUpdateTime = RbTimer();
	return Count;
}

long LandScape::DeletePolygon( const PolyStation* pStation )
{
	long Count = 0;
	Count += GhostPoly.Delete( pStation );
	Count += BlockPoly.Delete( pStation );
	Count += GlassPoly.Delete( pStation );
	Count += BridgePoly.Delete( pStation );
	if( Count > 0 )LastUpdateTime = RbTimer();
	return Count;
}

RbPosition LandScape::MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const
{
	RbPosition Pos = _MovingPos( DestPos, SrcPos, Size );
	Pos.z = GetHight( Pos.x, Pos.y );
	return Pos;
}

RbPosition LandScape::_MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const
{
	static float CheckLength = 16;

	RbPosition Vector = DestPos - SrcPos;
	float Length = Vector.Length();
	if( Length <= 0 )return DestPos;

	Vector.Normalize( CheckLength );

	RbPosition RetPos = SrcPos;
	do
	{
		if( !MoveOK( RetPos+Vector, Size ) )return RetPos;
		RetPos += Vector;
		Length -= CheckLength;
	}while( Length > 0 );

	return DestPos;
}
