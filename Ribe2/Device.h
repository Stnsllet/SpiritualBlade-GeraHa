// Dvice.h: Dvice クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVICE_H__E331C5C9_6FAF_49BE_A8FD_AFB69B31FD07__INCLUDED_)
#define AFX_DVICE_H__E331C5C9_6FAF_49BE_A8FD_AFB69B31FD07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class D3DObject;

class Device  
{
public:
	Device();
	virtual ~Device();

	DISPDEVICE* operator ->()const{return pDevice;}
	operator DISPDEVICE*() const{return pDevice;}
	bool operator!() const{return pDevice==NULL;}

	HRESULT Create( D3DObject& D3D, HWND hWnd );
	virtual HRESULT SetRenderState( const D3DCAPS8& HalCaps );
	bool IsLostDevice() const;
	HRESULT FreeMemory();
	HRESULT RestoreDevice( bool bForce = false );

	DWORD SetAlphaType( DWORD Type = D3DTOP_MODULATE, long Stage = 0 );
	bool SetFog( float Min, float Max );

	HRESULT BeginScene(bool bClear=true);
	HRESULT EndScene();
	HRESULT Flip();
	virtual HRESULT WindowSizeChange( UINT Width, UINT Height );
	virtual HRESULT FullScreenWindow( const D3DDISPLAYMODE& DisplayMode, UINT Width, UINT Height );
	const D3DVIEWPORT8& ViewPort() const{return NowViewPort;}

	const D3DCOLOR& RefAbientColor() const{return AbientColor;}
protected:
	virtual HRESULT _RestoreDevice();

	D3DPRESENT_PARAMETERS PresentParams;
	D3DVIEWPORT8 NowViewPort;
	D3DCAPS8 DeviceHalCaps;
	D3DCOLOR BackColor;
	D3DCOLOR AbientColor;

	CComPtr<DISPDEVICE> pDevice;
};

#endif // !defined(AFX_DVICE_H__E331C5C9_6FAF_49BE_A8FD_AFB69B31FD07__INCLUDED_)
