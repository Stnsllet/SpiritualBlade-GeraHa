// RbConfig.h: RbConfig クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBCONFIG_H__7DA7B48A_455D_4DD0_B6AC_D0E7C4EE3CE1__INCLUDED_)
#define AFX_RBCONFIG_H__7DA7B48A_455D_4DD0_B6AC_D0E7C4EE3CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XMLPaser;

class RbConfig  
{
public:
	RbConfig();
	virtual ~RbConfig();

	HRESULT ReadConfig();
	HRESULT ReadConfig( LPCTSTR FileName );
	HRESULT SaveConfig();
	bool IsFullScreen( HWND hWnd ) const;

	void SetDisplayConfig( HWND hWnd );

	struct _Display
	{
		int x, y;
		int Width;
		int Height;
		float Scale;
		bool bDrawSoft, bVertexSoft;
		float Adjuster;
	}Display;

	struct _Volume
	{
		long BGMVolume;
		long SoundVolume;
	}Volume;
	struct _Window
	{
		long WindowTexNo;
		bool bMiniStateMode;
	}Window;
	struct _System
	{
		bool bDontCache;
		bool bUseManaged;
	}System;

private:
	HRESULT ReadDisplay( const XMLPaser& ConfigFile );
	HRESULT ReadBGM( const XMLPaser& ConfigFile );
	HRESULT ReadSound( const XMLPaser& ConfigFile );
	HRESULT ReadWindow( const XMLPaser& ConfigFile );
	HRESULT ReadSystem( const XMLPaser& ConfigFile );
};

#endif // !defined(AFX_RBCONFIG_H__7DA7B48A_455D_4DD0_B6AC_D0E7C4EE3CE1__INCLUDED_)
