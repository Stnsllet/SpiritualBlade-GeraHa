// ExSprite.h: ExSprite クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXSPRITE_H__0F58A6FB_F478_47F8_A083_C1DA2D2DBE01__INCLUDED_)
#define AFX_EXSPRITE_H__0F58A6FB_F478_47F8_A083_C1DA2D2DBE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbSprite.h"

class ExSprite : public RbSprite  
{
public:
	ExSprite();
	virtual ~ExSprite();
	virtual bool Restore();

	static bool SetEx( D3DFORMAT Format );

	virtual HRESULT Clear();
	virtual ARGBF SetDrawColor( const D3DCOLORVALUE& NewColor );
protected:
	virtual HRESULT _InitSprite( DISPDEVICE* pTagetDevice, MYSURFACE* pBackBuffer );
	HRESULT CreateRenderTaget( DISPDEVICE* pDevice, MYSURFACE* pBaseSurface );
	HRESULT ClearFromTex( MYSURFACE* pTagetSurface ) const;
	HRESULT Flip();

	static bool bUseExMode;
	static D3DFORMAT RenderFormat;
	static const ARGBF BaseColor;
	CComPtr<MYTEXTURE> pRenderTaget;
	CComPtr<MYSURFACE> pBaseSurface;
};

#endif // !defined(AFX_EXSPRITE_H__0F58A6FB_F478_47F8_A083_C1DA2D2DBE01__INCLUDED_)
