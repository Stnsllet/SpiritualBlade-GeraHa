// TextFont.cpp: TextFont クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "TextFont.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TextFont::TextFont()
{
	bRestore = false;
}

TextFont::~TextFont()
{

}

// 開放
HRESULT TextFont::Release()
{
	FontList.clear();
	bRestore = false;
	return S_OK;
}

// フォント取得
ID3DXFont* TextFont::GetFont( LPCTSTR Type ) const
{
	FontMap::const_iterator itr = FontList.find(Type);
	if( itr != FontList.end() )return itr->second;

	FontDefMap::const_iterator defitr = FontDefineList.find(Type);
	if( defitr == FontDefineList.end() )defitr = FontDefineList.begin();
	if( defitr == FontDefineList.end() )return NULL;
	CComPtr<ID3DXFont> NewFont;
	CreateFont( pUseDevice, defitr->second, &NewFont );
	if( NewFont != NULL )FontList[Type] = NewFont;
	return NewFont;
}

// フォント初期化
HRESULT TextFont::InitFonts( DISPDEVICE* pDevice )
{
	return InitFonts( pDevice, _T("system/Fonts.xml") );
}

// フォント初期化
HRESULT TextFont::InitFonts( DISPDEVICE* pDevice, LPCTSTR Filename )
{
	if( pDevice == NULL )return E_POINTER;
	pUseDevice = pDevice;
	FontList.clear();

	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( Filename );
	if( FAILED(ret) )return ret;

	XMLList FontList;
	ret = FontList.Query( XMLFile, _T("Font") );
	if( FAILED(ret) )return ret;

	int Count = FontList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		CreateFont( pDevice, FontList.GetItem(i) );
	}
	return S_OK;
}

// フォント作成
HRESULT TextFont::CreateFont( DISPDEVICE* pDevice, const XMLPaser& FontXML )
{
	String Name = FontXML.AttributeString( TAG::NAME );
	if( Name.length() <= 0 )return E_FAIL;

	String FontName = FontXML.AttributeString( _T("font") );
	if( FontName.length() <= 0 )return E_FAIL;

	long Size = FontXML.AttributeLong( _T("size") );
	if( Size <= 0 )Size = 16;

	String Style = FontXML.AttributeString( _T("style") );

	long FontStyle = FW_NORMAL;
	if( (long)Style.find(_T('t'))>=0 )FontStyle = FW_THIN;
	if( (long)Style.find(_T('n'))>=0 )FontStyle = FW_NORMAL;
	if( (long)Style.find(_T('b'))>=0 )FontStyle = FW_BOLD;
	if( (long)Style.find(_T('h'))>=0 )FontStyle = FW_HEAVY;

	bool bItalic = false;
	if( (long)Style.find(_T('i'))>=0 )bItalic = true;

	LOGFONT LogFont;
	AllZero(LogFont);
	LogFont.lfHeight = Size;
	LogFont.lfWeight = FontStyle;
	LogFont.lfItalic = bItalic;
	LogFont.lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
	LogFont.lfOutPrecision = OUT_TT_ONLY_PRECIS; //OUT_DEFAULT_PRECIS;
	LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	LogFont.lfQuality = DEFAULT_QUALITY;
//	LogFont.lfPitchAndFamily = FF_MODERN | FIXED_PITCH;
	LogFont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	lstrcpy( LogFont.lfFaceName, FontName );

//	CComPtr<ID3DXFont> pFont;
//	ret = D3DXCreateFontIndirect( pDevice, &LogFont, &pFont );
//	if( FAILED(ret) )return ret;

	FontDefineList[ Name ] = LogFont;
//	FontList[ Name ] = pFont;
	return S_OK;
}

// フォント作成
HRESULT TextFont::CreateFont( DISPDEVICE* pDevice, const LOGFONT& FontStyle, ID3DXFont** ppOutput ) const
{
	if( ppOutput == NULL )return E_POINTER;

	CComPtr<ID3DXFont> pFont;
	HRESULT ret = D3DXCreateFontIndirect( pDevice, &FontStyle, &pFont );
	if( FAILED(ret) )return ret;

	*ppOutput = pFont.Detach();
	return ret;
}

// テキストの大きさ取得
RECT TextFont::GetTextRect( LPCTSTR Type, LPCTSTR Message ) const
{
	RECT TextRect;
	AllZero( TextRect );

	if( Message == NULL )return TextRect;

	CComPtr<ID3DXFont> pFont = GetFont( Type );
	if( pFont == NULL )return TextRect;

	pFont->DrawText( Message, -1, &TextRect, DT_CALCRECT, MYWHITE );

//	TextRect.bottom += 2;
//	TextRect.right += 2;
	return TextRect;
}

// 文字表示
HRESULT TextFont::Draw( LPCTSTR Message, const RECT& Rect, DWORD Format, DWORD Color ) const
{
	return Draw( _T("default"), Message, Rect, Format, Color );
}

// 文字表示
HRESULT TextFont::Draw( LPCTSTR Type, LPCTSTR Message, int x, int y, DWORD Color ) const
{
	RECT Rect;
	Rect.left = x;
	Rect.top = y;
	Rect.right = 32767;
	Rect.bottom = 32767;
	return Draw( Type, Message, Rect, DT_LEFT, Color );
}

// 文字表示
HRESULT TextFont::Draw( LPCTSTR Type, LPCTSTR Message, const RECT& Rect, DWORD Format, DWORD Color ) const
{
	if( Message == NULL )return E_POINTER;

	CComPtr<ID3DXFont> pFont = GetFont( Type );
	if( pFont == NULL )return E_FAIL;

	RECT Area = Rect;
	HRESULT ret = Draw( pFont, Message, Area, Format, Color, Color!=D3DCOLOR_XRGB(0,0,0) );
	if( FAILED(ret) )return ret;

	return ret;
}

HRESULT TextFont::Draw( ID3DXFont* pFont, LPCTSTR Message, const RECT& Rect, DWORD Format, DWORD Color, bool fuchi ) const
{
	if( fuchi )
	{
		RECT Area = Rect;	Area.left -=1;	Area.right -= 1;
		_Draw( pFont, Message, Area, Format, D3DCOLOR_XRGB(0,0,0) );
		Area = Rect;	Area.left +=1;	Area.right += 1;
		_Draw( pFont, Message, Area, Format, D3DCOLOR_XRGB(0,0,0) );
		Area = Rect;	Area.top -=1;	Area.bottom -= 1;
		_Draw( pFont, Message, Area, Format, D3DCOLOR_XRGB(0,0,0) );
		Area = Rect;	Area.top +=1;	Area.bottom += 1;
		_Draw( pFont, Message, Area, Format, D3DCOLOR_XRGB(0,0,0) );
	}
	return _Draw( pFont, Message, Rect, Format, Color );
}

HRESULT TextFont::_Draw( ID3DXFont* pFont, LPCTSTR Message, RECT Rect, DWORD Format, DWORD Color ) const
{
	pFont->Begin();
	HRESULT ret = pFont->DrawText( Message, -1, &Rect, Format, Color );
	pFont->End();
	if( FAILED(ret) )return ret;

	return S_OK;
}
