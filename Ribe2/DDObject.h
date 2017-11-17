// DDObject.h: DDObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDOBJECT_H__35DA016A_E443_4B14_8536_6FDD36041F94__INCLUDED_)
#define AFX_DDOBJECT_H__35DA016A_E443_4B14_8536_6FDD36041F94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct IDirectDraw2;

class DDObject  
{
public:
	DDObject();
	~DDObject();

	HRESULT CreateDirectDraw();

	HRESULT GetVideoMemory( DWORD& dwFree, DWORD& dwAll );
	CComPtr<IDirectDraw2> pDDraw;
};

#endif // !defined(AFX_DDOBJECT_H__35DA016A_E443_4B14_8536_6FDD36041F94__INCLUDED_)
