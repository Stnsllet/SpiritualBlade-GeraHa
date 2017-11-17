// RbConfig.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "..\Ribe2\XMLPaser.h"
#include "ConfigDlg.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: この位置にコードを記述してください。

	::CoInitialize( NULL );
	ConfigDlg Dlg;
	{
		XMLPaser SystemXML;
		HRESULT hr = SystemXML.Load( _T("System/System.xml") );
		if( FAILED(hr) )
		{
			::MessageBox(NULL,_T("システムファイルが見つかりません"),_T("Error"),MB_OK | MB_ICONSTOP );
			::CoUninitialize();
			return 0;
		}
		Dlg.SetSystemXML( SystemXML );
	}

	Dlg.DoModal( hInstance );
	Dlg.SaveConfig();
	::CoUninitialize();
	return 0;
}
