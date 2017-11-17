// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#define _WIN32_WINNT 0x0400

// Windows ヘッダー ファイル:
#include <windows.h>
#include <shellapi.h>
#include <atlbase.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "TAG.h"
#include "MyHead.h"
#include "SystemMessage.h"
#include "VertexDef.h"

class String;
class GameSystem;
extern GameSystem* g_pSystem;
extern D3DCOLOR MYWHITE;

bool InfoPopup( LPCTSTR Msg );
bool SystemPopup( LPCTSTR Msg );
bool UserOrStr( String& InOut );
bool UserOrStr( CComBSTR& InOut );

// ローカル ヘッダー ファイル

// TODO: プログラムで必要なヘッダー参照を追加してください。

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
