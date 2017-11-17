// D3DObject.h: D3DObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DOBJECT_H__9A55EF8C_BD19_4DBB_89CF_AA4C7CEA3EBA__INCLUDED_)
#define AFX_D3DOBJECT_H__9A55EF8C_BD19_4DBB_89CF_AA4C7CEA3EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Display.h"

class D3DObject  
{
public:
	D3DObject();
	virtual ~D3DObject();

	virtual HRESULT Init();

	bool SetObject( USEDIRECT3D* pNewObject );
	const D3DDISPLAYMODE& DisplayMode() const{return NowDispMode;}
	HRESULT CreateDevice( DISPDEVICE** ppDevice, HWND hWnd, bool FullScreen = false );
	HRESULT CreateDevice( HWND hWnd, D3DPRESENT_PARAMETERS& InitParams, DISPDEVICE** ppDevice );
	D3DCAPS8 GetDeviceCaps( D3DDEVTYPE Type=D3DDEVTYPE_HAL ) const;

	USEDIRECT3D* GetD3D() const;
	operator USEDIRECT3D*() const{return pDirect3D;}
	operator const D3DPRESENT_PARAMETERS&() {return InitParams;}

	D3DFORMAT GetRenderFormat( DISPDEVICE* pDevice ) const;
	D3DFORMAT GetTextureFormat( DISPDEVICE* pDevice ) const;
	bool CheckRenderFormat( DISPDEVICE* pDevice, D3DFORMAT CheckFormat ) const;
	bool CheckTextureFormat( DISPDEVICE* pDevice, D3DFORMAT CheckFormat ) const;
	D3DFORMAT GetDepthStencilFormat() const;
	bool CheckDepthStencilFormat( D3DFORMAT CheckFormat ) const;
	bool IsAlphaFormat( D3DFORMAT CheckFormat ) const;
	void SetEmurationMode( bool Draw, bool Vertex );
protected:
	CComPtr<USEDIRECT3D> pDirect3D;
	D3DDISPLAYMODE NowDispMode;
	D3DPRESENT_PARAMETERS InitParams;
	bool SoftVertex, SoftDraw;

	static D3DFORMAT AlphaFormats[];
};

#endif // !defined(AFX_D3DOBJECT_H__9A55EF8C_BD19_4DBB_89CF_AA4C7CEA3EBA__INCLUDED_)
