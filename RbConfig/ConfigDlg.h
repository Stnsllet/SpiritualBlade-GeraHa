// ConfigDlg.h: ConfigDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGDLG_H__CE1C157E_DDE3_42D1_AFD5_7DEAEFC923C2__INCLUDED_)
#define AFX_CONFIGDLG_H__CE1C157E_DDE3_42D1_AFD5_7DEAEFC923C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Ribe2\RbConfig.h"
#include "..\Ribe2\GlobalWindow.h"
class XMLPaser;

class ConfigDlg  
{
public:
	ConfigDlg();
	virtual ~ConfigDlg();

	BOOL DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
	static BOOL CALLBACK ConfigDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int DoModal( HINSTANCE hInstance );
	void ReadDefault();
	bool SetSystemXML( const XMLPaser& XML );
	HRESULT SaveConfig(){return Config.SaveConfig(); }
protected:
	BOOL InitDialog( HWND hWnd );
	BOOL Command( int ID, HWND hWnd );
	void SetTrackBarRange(int ID, int Min, int Max);
	void SetTrackBarPos(int ID, int Pos);
	int GetTrackBarPos(int ID );
	BOOL TrackBarMsg( int ID, int Msg, int Value );

	void SetConfigData( bool bValuOnly=false );
	long ScaleToSlide( float Scale ) const;
	float SlideToScale( long Slide ) const;

	GlobalWindow GWnd;
	RbConfig Config;
	HWND m_hWnd;
	CComBSTR WindowName;
	long WindowTexCount;
	float MinScale;
	float MaxScale;
};

#endif // !defined(AFX_CONFIGDLG_H__CE1C157E_DDE3_42D1_AFD5_7DEAEFC923C2__INCLUDED_)
