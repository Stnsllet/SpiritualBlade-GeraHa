// LandMapData.cpp: LandMapData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LandMapData.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandMapData::LandMapData( RbBuffer<MapID>& Ref ) : MapData( Ref )
{

}

LandMapData::~LandMapData()
{

}

bool LandMapData::SetMapData( int x, int y, float ChipX, float ChipY )
{
	return MapData.SetMapData( x, y, ChipX, ChipY );
}

bool LandMapData::_MapData::SetMapData( int x, int y, float ChipX, float ChipY )
{
	Face.x = x;
	Face.y = y;
	ChipSize.x = ChipX;
	ChipSize.y = ChipY;

	MapArea.left = ChipX / 2;
	MapArea.top = ChipY / 2;
	MapArea.right = (ChipSize.x*Face.x)/2.0f;// - MapArea.left;
	MapArea.bottom = (ChipSize.y*Face.y) - MapArea.top;
	return true;
}

bool LandMapData::MoveOK( float x, float y, float Size ) const
{
	return MapData.MapArea.PinSize( x, y, Size );
}

// ポリゴン番号を返す
int LandMapData::GetFacePosition( float x, float y ) const
{
	const float ChipWidth = MapData.ChipSize.x;
	const float ChipHeight = MapData.ChipSize.y;
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

	if( ChipX < 0 || ChipX >= MapData.Face.x )return -1;
	if( ChipY < 0 || ChipY >= MapData.Face.y )return -1;

	int RetPos = ChipX + ChipY * MapData.Face.x;
	ATLASSERT( RetPos < MapData.pData.Count() );
	return RetPos;
}

// マップを返す
LandMapData::MapID LandMapData::GetMapID( float x, float y ) const
{
	return GetMapID( GetFacePosition( x, y ) );
}

// マップを返す
LandMapData::MapID LandMapData::GetMapID( int FaceNum ) const
{
	if( FaceNum < 0 )return AllZero(MapID());
	return MapData.pData[FaceNum];
}
