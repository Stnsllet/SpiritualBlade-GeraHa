// Sprite.h: Sprite クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__645778EC_3D72_4723_9D2C_315C3E8F5164__INCLUDED_)
#define AFX_SPRITE_H__645778EC_3D72_4723_9D2C_315C3E8F5164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Texture;
#include "TexRender.h"

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();
	virtual bool Restore();
	static bool Is3D(){return false;}
	static float SetAduster( float New );

	HRESULT InitSprite( DISPDEVICE* pDevice );
	HRESULT InitSprite( DISPDEVICE* pDevice, const Texture& BackBuffer );
	HRESULT InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer );

	HRESULT Draw( const Texture& Tex, float x, float y );
	HRESULT Draw( MYTEXTURE* pTexture, float x, float y );
	HRESULT Draw( MYTEXTURE* pTexture, const RECT& Dest );
	HRESULT Draw( MYTEXTURE* pTexture, float x, float y , const RECT& Rect );
	HRESULT Draw( MYTEXTURE* pTexture, const RECT& Rect, const RECT& Dest );
	HRESULT Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans );
	HRESULT Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans, const RECT& Rect );

	HRESULT GetDevice( DISPDEVICE** ppDevice ) const;
	virtual HRESULT Clear();
	HRESULT ColorClear( D3DCOLOR ClearColor );

	virtual ARGBF SetDrawColor( const D3DCOLORVALUE& NewColor );
	const POINT& GetSize() const{return SpriteSize;}
protected:
	virtual HRESULT _InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer );
	D3DCOLOR DrawColor;
	POINT SpriteSize;
	CComPtr<MYSPRITE> pSprite;
	TexRender Render;
	struct Stoker
	{
		bool Stock( DISPDEVICE * pStockDevice );
		bool Restore();
		CComPtr<DISPDEVICE> pDevice;
		DWORD FogEnable, AlphaTest;
	}MyStock;
};

#endif // !defined(AFX_SPRITE_H__645778EC_3D72_4723_9D2C_315C3E8F5164__INCLUDED_)
