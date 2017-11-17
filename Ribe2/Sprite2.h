// Sprite2.h: Sprite2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE2_H__59E9B2AE_60AA_49A3_A667_64210F67951E__INCLUDED_)
#define AFX_SPRITE2_H__59E9B2AE_60AA_49A3_A667_64210F67951E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TexRender.h"
class Texture;

class Sprite2  
{
public:
	Sprite2();
	virtual ~Sprite2();
	virtual bool Restore();
	static bool Is3D(){return true;}

	HRESULT InitSprite( DISPDEVICE* pDevice );
	HRESULT InitSprite( DISPDEVICE* pDevice, const Texture& BackBuffer );
	HRESULT InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer );

	HRESULT GetDevice( DISPDEVICE** ppDevice ) const;
	virtual HRESULT Clear();
	HRESULT ColorClear( D3DCOLOR ClearColor );

	HRESULT Draw( const Texture& Tex, float x, float y );
	HRESULT Draw( MYTEXTURE* pTexture, float x, float y );
	HRESULT Draw( MYTEXTURE* pTexture, const RECT& Dest );
	HRESULT Draw( MYTEXTURE* pTexture, float x, float y , const RECT& Rect );
	HRESULT Draw( MYTEXTURE* pTexture, const RECT& Rect, const RECT& Dest );
	HRESULT Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans );
	HRESULT Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans, const RECT& Rect );

	virtual ARGBF SetDrawColor( const D3DCOLORVALUE& NewColor );
	const POINT& GetSize() const{return SpriteSize;}
	static float SetAduster( float New );
protected:
	HRESULT _Draw( const Texture& Tex, const D3DMATRIX& Trans );
	HRESULT _Draw( const Texture& Tex, const D3DMATRIX& Trans, const RECT& Rect );
	HRESULT _Draw( const Texture& Tex );
	HRESULT _Draw( const Texture& Tex, const RECT& Rect );
	virtual HRESULT _InitSprite( DISPDEVICE* pTagetDevice, MYSURFACE* pBackBuffer );

	CComPtr<DISPDEVICE> pDevice;
	struct Stoker
	{
		bool Stock( DISPDEVICE * pStockDevice );
		bool Restore();
		CComPtr<DISPDEVICE> pDevice;
		D3DMATRIX ProjTrans, ViewTrans, WorldTrans;
		DWORD ZEnable, CullMode, Lighting, Ambient, SSU, SSV, FogEnable, StencilEnable;
		D3DMATERIAL8 Mate;
	}MyStock;
	D3DMATRIX MyWorld;
	D3DMATERIAL8 MyMaterial;
	TexRender Render;
	POINT SpriteSize;
	float ScaleX, ScaleY;
	static float Adjuster;
};

#endif // !defined(AFX_SPRITE2_H__59E9B2AE_60AA_49A3_A667_64210F67951E__INCLUDED_)
