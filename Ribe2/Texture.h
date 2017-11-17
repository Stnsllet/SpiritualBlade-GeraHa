// Texture.h: Texture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__F31F2234_2213_499F_95DC_7CC59E733A6A__INCLUDED_)
#define AFX_TEXTURE_H__F31F2234_2213_499F_95DC_7CC59E733A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Display;
struct DISPDEVICE;

class Texture
{
public:
	Texture();
	Texture( MYTEXTURE* pNewTexture ){SetTexture(pNewTexture);}
	virtual ~Texture();
	operator MYTEXTURE*() const{return pTexture;}
	HRESULT GetSurface( MYSURFACE** ppSurface ) const;
	void Release();
	static bool Algin( DISPDEVICE* pDevice, UINT* pWidth, UINT* pHeight );
	static bool Algin( DISPDEVICE* pDevice, UINT* pWidth, UINT* pHeight, DWORD Usage, D3DFORMAT* pFormat, D3DPOOL Pool );

	HRESULT SetTexture( MYTEXTURE* pNewTexture );
	HRESULT LoadTexture( DISPDEVICE* pDevice, LPCTSTR pFileName );
	HRESULT Create( DISPDEVICE* pDevice, int Width, int Height );
	HRESULT CreateTexture( DISPDEVICE* pDevice, int Width, int Height, D3DFORMAT Format, DWORD Usage, D3DPOOL Memory, long MipLevel=1 );
	HRESULT CreateRenderTaget( DISPDEVICE* pDevice, int Width, int Height );
	HRESULT CreateDefault( DISPDEVICE* pDevice, int Width, int Height );
	const D3DXIMAGE_INFO& ImageInfo() const{return TexInfo;}
	const D3DSURFACE_DESC& RefDesc() const{return DescInfo;}

	bool operator!() const{return pTexture==NULL;}
	static void SetUseFormat( D3DFORMAT Default, D3DFORMAT Render=D3DFMT_UNKNOWN );
	static bool SetUseManaged( bool bManaged );
	static bool CheckAlgin( DISPDEVICE* pDevice );
	static bool SetAlgin( bool bAlgin );
protected:
	static D3DFORMAT DefaultFormat;
	static D3DFORMAT RenderFormat;
	static bool bUseManaged;
	static bool bMaxAlgin;
	HRESULT GetDesc( D3DSURFACE_DESC* pDesc ) const;
	CComPtr<MYTEXTURE> pTexture;
	D3DXIMAGE_INFO TexInfo;
	D3DSURFACE_DESC DescInfo;
};

#endif // !defined(AFX_TEXTURE_H__F31F2234_2213_499F_95DC_7CC59E733A6A__INCLUDED_)
