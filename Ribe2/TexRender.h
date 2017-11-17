// TexRender.h: TexRender クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXRENDER_H__60554E7C_DD38_4F46_AD8C_645AAD6142BD__INCLUDED_)
#define AFX_TEXRENDER_H__60554E7C_DD38_4F46_AD8C_645AAD6142BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TexRender  
{
public:
	TexRender();
	~TexRender();

	HRESULT Restore();
	HRESULT Swap( DISPDEVICE* pDevice, MYSURFACE* pTaget, D3DFORMAT DepthStencilFormat=D3DFMT_UNKNOWN );

	bool operator!() const{return pRender==NULL;}
	ID3DXRenderToSurface* operator ->()const{return pRender;}
	const D3DSURFACE_DESC& RefDesc() const{return SurfaceDesc;}
private:
	CComPtr<ID3DXRenderToSurface> pRender;
	bool bChangeScene;
	D3DSURFACE_DESC SurfaceDesc;

};

#endif // !defined(AFX_TEXRENDER_H__60554E7C_DD38_4F46_AD8C_645AAD6142BD__INCLUDED_)
