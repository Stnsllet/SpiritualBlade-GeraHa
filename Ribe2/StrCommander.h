// StrCommander.h: StrCommander クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRCOMMANDER_H__34B3DBE7_6F7E_48AB_BE5D_D659D57BB7B8__INCLUDED_)
#define AFX_STRCOMMANDER_H__34B3DBE7_6F7E_48AB_BE5D_D659D57BB7B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringSplitter.h"
class BaseWindow;
class RbWindowSystem;
class RbStatus;
class BattleSystem;
class RbGameData;

class StrCommander  
{
public:
	StrCommander();
	~StrCommander();

	bool Command( LPCTSTR Command );
	BaseWindow* ShowBattleStatus( RbStatus* pStatus ) const;
	BaseWindow* NewWindow( const String& Name, const CComVariant& Param=CComVariant() ) const;
	BaseWindow* ShowWindow( const String& Name, const CComVariant& Param=CComVariant() ) const;
	bool SetParameter( const String& Name, const CComVariant& Param ) const;
	bool ShowMemberState( long PasonID ) const;
	bool ShowMemberState( LPCTSTR PasonName ) const;

private:
	bool _Command() const;
	bool Ex() const;
	bool Param() const;
	bool Tutorial() const;
	bool Battle() const;
	bool World() const;
	bool Window() const;
	bool GameData() const;
	bool WindowRestore( RbWindowSystem& System ) const;
	bool CLandEdit() const;
	bool NewsCheck() const;
	bool LoadCheck() const;
	bool MemberCheck() const;
	bool ShowShopItem( RbWindowSystem& System ) const;
	bool New( RbWindowSystem& System ) const;
	bool Show( RbWindowSystem& System ) const;
	bool WindowPosition( RbWindowSystem& System ) const;
	bool SetBord( BattleSystem& System ) const;
	bool DeleteItems( RbGameData& UserData ) const;
	bool DeleteItemType( RbGameData& UserData, const String& TypeName ) const;

	BaseWindow* CreateWnd( LPCTSTR Name ) const;

	StringSplitter<> Commands;
};

#endif // !defined(AFX_STRCOMMANDER_H__34B3DBE7_6F7E_48AB_BE5D_D659D57BB7B8__INCLUDED_)
