// DefaultRenderState.h: DefaultRenderState クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFAULTRENDERSTATE_H__592D5DE8_B6C8_456C_AB32_AFFB40A5AF8C__INCLUDED_)
#define AFX_DEFAULTRENDERSTATE_H__592D5DE8_B6C8_456C_AB32_AFFB40A5AF8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DefaultRenderState  
{
public:
	DefaultRenderState( DISPDEVICE * pDev );
	virtual ~DefaultRenderState();

	HRESULT SetRenderState();
	DWORD SetAlphaType( DWORD Type = D3DTOP_MODULATE, long Stage = 0 );
	bool SetFog( float Min, float Max, D3DCOLOR BackColor );

protected:
	CComPtr<DISPDEVICE> pDevice;
};

#endif // !defined(AFX_DEFAULTRENDERSTATE_H__592D5DE8_B6C8_456C_AB32_AFFB40A5AF8C__INCLUDED_)
