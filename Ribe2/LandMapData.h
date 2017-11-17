// LandMapData.h: LandMapData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDMAPDATA_H__9E72797D_802B_4AD3_9FF5_2A075F35B74C__INCLUDED_)
#define AFX_LANDMAPDATA_H__9E72797D_802B_4AD3_9FF5_2A075F35B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "RbRect.h"
#include "RbBuffer.h"
#include "RbPosition.h"

class LandMapData  
{
public:
	struct MapID
	{
		MapID(){}
		MapID( BYTE M ){Add=0;Move=M;}
		MapID( BYTE M, BYTE A ){Add=A;Move=M;}
		BYTE Move;
		BYTE Add;
		operator long() const{return Move;}
		long View() const{return (long)Add*256 + Move;}
	};

	LandMapData( RbBuffer<MapID>& Ref );
	virtual ~LandMapData();
	bool SetMapData( int x, int y, float ChipX, float ChipY );

	bool MoveOK( float x, float y, float Size ) const;
	const RbRectF& LandArea() const{return MapData.MapArea;}
	MapID GetMapID( float x, float y ) const;
	MapID GetMapID( int FaceNum ) const;
	int GetFacePosition( float x, float y ) const;
protected:
	struct _MapData
	{
		_MapData( RbBuffer<MapID>& Ref ):pData(Ref){}
		RbPoint	Face;
		RbPointF ChipSize;
		RbRectF	 MapArea;
		RbBuffer<MapID>&	pData;
		bool SetMapData( int x, int y, float ChipX, float ChipY );
	}MapData;
};

#endif // !defined(AFX_LANDMAPDATA_H__9E72797D_802B_4AD3_9FF5_2A075F35B74C__INCLUDED_)
