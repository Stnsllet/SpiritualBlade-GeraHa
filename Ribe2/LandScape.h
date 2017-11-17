// LandScape.h: LandScape クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDSCAPE_H__17D69A64_88EF_4FE1_9E41_E6DC0B16ECE7__INCLUDED_)
#define AFX_LANDSCAPE_H__17D69A64_88EF_4FE1_9E41_E6DC0B16ECE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LandBase.h"
#include "PolyMaster.h"

class LandScape : public LandBase
{
public:
	LandScape();
	virtual ~LandScape();
	virtual void InitLand( DISPDEVICE* pDevice );
	virtual HRESULT LoadLandScape( LPCTSTR FileName );
	virtual bool MouseJob( Display& Disp, GameMessage& State );
	bool IsUpDate( double CheckTime ) const;

	virtual void ClearData();
	virtual HRESULT Draw( Display& Disp );
	virtual HRESULT DrawGhost( Display& Disp );
	float GetHight( float x, float y ) const;
	float GetHight( float x, float y, int FaceNum ) const;
	D3DXVECTOR3 GetLandCenterPos() const;
	D3DXVECTOR3 GetClickPos( const Display& Disp, int MouseX, int MouseY ) const;
	RbPosition GetStrPos( const String& PosString ) const;

	const PolyStation* BlockCheck( const RbPosition& Pos, float Size ) const;
	bool MoveOK( const RbPosition& Pos, float Size, float LandSize=48.0f ) const;
	RbPosition MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const;
	bool SetPolygon( const XMLPaser& PolyXML );
	PolyStation* AddPolygon( const String& PolyName, const RbPosition& Pos, float Direction=0, float Scale=1 );
	long DeletePolygon( const RbPosition& Pos );
	long DeletePolygon( const PolyStation* pStation );
	LandMapData Map;
protected:
	RbPosition _MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const;
	int GetFacePosition( float x, float y ) const;
	HRESULT DrawSinplePlane( Display& Disp, const SinplePlane& SinpleData ) const;

	PolyMaster BlockPoly;
	PolyMaster GlassPoly;
	PolyMaster GhostPoly;
	PolyMaster BridgePoly;

	double LastUpdateTime;
};

#endif // !defined(AFX_LANDSCAPE_H__17D69A64_88EF_4FE1_9E41_E6DC0B16ECE7__INCLUDED_)
