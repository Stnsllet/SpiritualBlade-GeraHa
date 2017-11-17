// BackSwapper.h: BackSwapper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKSWAPPER_H__65EA8A12_2D16_4E39_80EB_611A08987093__INCLUDED_)
#define AFX_BACKSWAPPER_H__65EA8A12_2D16_4E39_80EB_611A08987093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BackSwapper  
{
public:
	BackSwapper();
	~BackSwapper();
	HRESULT Restore();
	HRESULT Swap( const BackSwapper& OldSwapper );
	HRESULT Swap( DISPDEVICE* pSetDevice, MYSURFACE* pNewBackBuffer, MYSURFACE* pNewZBuffer=NULL );
private:
	CComPtr<DISPDEVICE> pDevice;
	CComPtr<MYSURFACE> pOldBackBuffer;
	CComPtr<MYSURFACE> pOldZBuffer;
};

#endif // !defined(AFX_BACKSWAPPER_H__65EA8A12_2D16_4E39_80EB_611A08987093__INCLUDED_)
