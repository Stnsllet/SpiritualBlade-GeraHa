// TextFont.h: TextFont クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTFONT_H__9AA200C2_82B9_485F_BE50_D28891590200__INCLUDED_)
#define AFX_TEXTFONT_H__9AA200C2_82B9_485F_BE50_D28891590200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"

class XMLPaser;

class TextFont  
{
public:
	TextFont();
	virtual ~TextFont();

	HRESULT InitFonts( DISPDEVICE* pDevice );
	HRESULT InitFonts( DISPDEVICE* pDevice, LPCTSTR Filename );

	RECT GetTextRect( LPCTSTR Type, LPCTSTR Message ) const;

	HRESULT Draw( LPCTSTR Type, LPCTSTR Message, int x, int y, DWORD Color=MYWHITE ) const;
	HRESULT Draw( LPCTSTR Type, LPCTSTR Message, const RECT& Rect, DWORD Format=DT_LEFT, DWORD Color=MYWHITE ) const;
	HRESULT Draw( LPCTSTR Message, const RECT& Rect, DWORD Format=DT_LEFT, DWORD Color=MYWHITE ) const;
	HRESULT Release();

protected:
	ID3DXFont* GetFont( LPCTSTR Type ) const;
	HRESULT Draw( ID3DXFont* pFont, LPCTSTR Message, const RECT& Rect, DWORD Format, DWORD Color, bool fuchi ) const;
	HRESULT _Draw( ID3DXFont* pFont, LPCTSTR Message, RECT Rect, DWORD Format, DWORD Color ) const;
	HRESULT CreateFont( DISPDEVICE* pDevice, const XMLPaser& FontXML );
	HRESULT CreateFont( DISPDEVICE* pDevice, const LOGFONT& FontStyle, ID3DXFont** ppOutput ) const;

	CComPtr<DISPDEVICE> pUseDevice;
	typedef std::map<String,CComPtr<ID3DXFont> > FontMap;
	mutable FontMap FontList;
	typedef std::map<String,LOGFONT > FontDefMap;
	FontDefMap FontDefineList;
	bool bRestore;
};

#endif // !defined(AFX_TEXTFONT_H__9AA200C2_82B9_485F_BE50_D28891590200__INCLUDED_)
