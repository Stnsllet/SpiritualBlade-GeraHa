// StrCommander.cpp: StrCommander クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"
#include "AllWindow.h"
#include "RbMember.h"
#include "DataUpdater.h"
#include "NewsChecker.h"
#include "StrParamCom.h"
#include "StrCommander.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

StrCommander::StrCommander()
{
}

StrCommander::~StrCommander()
{
}

BaseWindow* StrCommander::CreateWnd( LPCTSTR Name ) const
{
	String name = Name;
	if( name==_T("Money") )return new MoneyWindow;
	if( name==_T("SelectCharactor") )return new SelCharaWindow;
	if( name==_T("SpecialAttack") )return new SPWindow;
	if( name==_T("BattleCommand") )return new BatComWindow;
	if( name==_T("PatyHp") )return new PatyHpWindow;
	if( name==_T("LandMap") )return new LandWindow;

	if( name==_T("StockMember") )return new StockMemberWindow;
	if( name==_T("Division") )return new DivisionWindow;
	if( name==_T("PatyEdit") )return new PatyEditWindow;
	if( name==_T("ExPatyEdit") )return new ExPatyEditWindow;
	if( name==_T("Status") )return new StatusWindow;
//	if( name==_T("Status") )return new MiniStatusWindow;
	if( name==_T("MiniStatus") )return new MiniStatusWindow;

	if( name==_T("News") )return new NewsWindow;
	if( name==_T("NewsList") )return new NewsListWindow;
	if( name==_T("ItemList") )return new ItemListWindow;
	if( name==_T("ShopList") )return new ShopListWindow;
	if( name==_T("Load") )return new LoadWindow;
	if( name==_T("Save") )return new SaveWindow;
	if( name==_T("Buy") )return new BuyWindow;
	if( name==_T("Sell") )return new SellWindow;
	if( name==_T("AutoBuy") )return new AutoBuyWindow;
	if( name==_T("EqStatus") )return new EquipStatusWindow;
	if( name==_T("Employ") )return new EmployWindow;
	if( name==_T("Discharge") )return new DischargeWindow;
	if( name==_T("Timer") )return new TimerWindow;

	if( name==_T("Infomation") )return new InfomationWindow;
	return NULL;
}

bool StrCommander::Command( LPCTSTR Command )
{
	if( Commands.SetString( Command ) <= 0 )return false;
	long Count = Commands.Count();

	for( int i = 0; i < Count; i++ )UserOrStr(Commands[i]);

	return _Command();
}

bool StrCommander::_Command() const
{
	const String& Com = Commands[0];
	if( Com == _T("window") )return Window();
	if( Com == _T("world") )return World();
	if( Com == _T("battle") )return Battle();
	if( Com == _T("tutorial") )return Tutorial();
	if( Com == _T("data") )return GameData();
	if( Com == _T("ex") )return Ex();
	if( Com == _T("param") )return Param();
	if( Com == _T("landedit") )return CLandEdit();

	return false;
}

bool StrCommander::Tutorial() const
{
	if( Commands.Count() <= 1 )return false;
	return GetWindowSystem().Tutorial( Commands[1] );
}

bool StrCommander::CLandEdit() const
{
	LandEdit& Edit = GetBattleSystem().Land;
	const String& Com = Commands[1];
	if( Com==_T("edit") )return Edit.Edit.EditMode();
	if( Com==_T("paint") )return Edit.Edit.SetPaintMode();
	if( Com==_T("hight") )return Edit.Edit.SetHightMode();

	if( Com==_T("save") )return !FAILED(Edit.SaveLandData());
	if( Com==_T("load") )return !FAILED(Edit.LoadLandData());
	if( Com==_T("chip") )return Edit.Edit.SetChip(Commands[2].ToLong(), Commands[3]);
	if( Com==_T("rndhight") )return Edit.RandomHight(Commands[2].ToLong(), Commands[3].ToLong());
	return false;
}

bool StrCommander::Battle() const
{
	BattleSystem& Battle = GetBattleSystem();
	const String& Com = Commands[1];
	if( Com == _T("blackout") )return Battle.BStatus.SetBlackOut(true);
	if( Com == _T("setbord") )return SetBord(Battle);
	return false;
}

bool StrCommander::World() const
{
	AllWorld& World = GetWorldSystem();
	const String& Com = Commands[1];
	if( Com == _T("stop") )return World.StopMove();
	if( Com == _T("locale") )return World.SetPosition( Commands[2] );
	if( Com == _T("move") )return World.MoveStart( Commands[2] );
	if( Com == _T("stay") )return World.IsMoveStop();
	if( Com == _T("zoom") )return World.SetBaseZoom( Commands[2].ToFloat() );
	if( Com == _T("main") )return World.ChangeMainChara( Commands[2] );
	return false;
}

bool StrCommander::Window() const
{
	RbWindowSystem& System = GetWindowSystem();
	const String& Com = Commands[1];
	if( Com == _T("show") )return Show( System );
	if( Com == _T("new") )return New( System );
	if( Com == _T("comwnd") )return System.ComWindow( Commands[2] );
	if( Com == _T("close") )
	{
		if( Commands[2].empty() ){System.AllClose();return true;}
		else return System.CloseWindow( Commands[2] );
	}
	if( Com == _T("nextmsg") )return g_pSystem->NextMsg();
	if( Com == _T("pos") )return WindowPosition( System );
	if( Com == _T("restore") )return WindowRestore( System );
	if( Com == _T("shopitem") )return ShowShopItem( System );
	if( Com == _T("inmsg") )return System.AddMsgTrap( System.GetNameWindow(Commands[2]) );
	if( Com == _T("frame") )return System.ChangeWindowTexture( Commands[2].ToLong() );
	if( Com == _T("nomsg") )return !System.OnMsg();
	return false;
}

bool StrCommander::GameData() const
{
	RbGameData& UserData = GetGameData();
	const String& Com = Commands[1];
	if( Com == _T("member") )return UserData.InMember( Commands[2] );
	if( Com == _T("have") )return (UserData.GetItemCount(Commands[2].ToLong())>=Commands[3].ToLong());
	if( Com == _T("delete") )return UserData.DischargeMember( Commands[2] );
	if( Com == _T("unview") )return UserData.UnViewMember( Commands[2] );
	if( Com == _T("review") )return UserData.ReViewMember( Commands[2] );
	if( Com == _T("changepaty") )
		if( Commands[3]==_T("ex") )return UserData.ChangePaty( Commands[2], UserData.RefExPatyNum() );
		else return UserData.ChangePaty( Commands[2], Commands[3].ToLong() );
	if( Com == _T("class") )return DataUpdater(Commands[2]).ClassChange( Commands[3] );
	if( Com == _T("skill") )return DataUpdater(Commands[2]).AddSkill( Commands[3],Commands[4].ToLong() );
	if( Com == _T("alldelete") )return (UserData.AllClear(),true);
	if( Com == _T("cleardata") )return UserData.ClearJob();
	if( Com == _T("deleteitem") )return DeleteItems( UserData );
	if( Com == _T("deleteitemtype") )return DeleteItemType( UserData, Commands[2] );

	return false;
}

bool StrCommander::DeleteItems( RbGameData& UserData ) const
{
	if( Commands.Count() < 3 )return false;
	for( int i = 2; i < Commands.Count(); i++ )
		UserData.AddItem(Commands[i].ToLong(),-100);
	return true;
}

bool StrCommander::DeleteItemType( RbGameData& UserData, const String& TypeName ) const
{
	const std::map<long,HaveItems>& RefItem = UserData.RefHaveItems();
	short Type = ItemCache::GetBaseType( TypeName );
	if( Type < 0 )return false;
	std::list<long> DropItems;
	std::map<long,HaveItems>::const_iterator itr = RefItem.begin();
	const ItemCache& Cache = GetCache().Item;
	while( itr != RefItem.end() )
	{
		const ItemStatus& Ref = Cache.Get((itr++)->second.id);
		if( Type == 0 || Ref.id.EquipType == Type )DropItems.push_back( Ref.id.Number );
	}

	if( DropItems.size() <= 0 )return false;
	std::list<long>::const_iterator ditr = DropItems.begin();
	while( ditr != DropItems.end() )
		UserData.AddItem(*(ditr++),-100);
	return true;
}

bool StrCommander::WindowPosition( RbWindowSystem& System ) const
{
	RefPtr<BaseWindow> pWnd = System.GetNameWindow( Commands[2] );
	if( pWnd == NULL )return false;
	float x = Commands[3].ToFloat()/100;
	float y = Commands[4].ToFloat()/100;
	pWnd->SetFloatPosition( System, x, y );
	return true;
}

bool StrCommander::WindowRestore( RbWindowSystem& System ) const
{
	RefPtr<BaseWindow> pWnd = System.GetNameWindow( Commands[2] );
	if( pWnd == NULL )return false;
	RbDisplay& Disp = GetRbDisplay();
	pWnd->Command( Disp, System, BaseWindow::eReleaseSurface );
	pWnd->Command( Disp, System, BaseWindow::eRestore );
	return true;
}

bool StrCommander::ShowShopItem( RbWindowSystem& System ) const
{
	CComVariant Param;
	ShowWindow( String(_T("ShopList")), Param );
	ShowWindow( String(_T("ItemList")), Param );
	return true;
}

BaseWindow* StrCommander::NewWindow( const String& Name, const CComVariant& Param ) const
{
	RbWindowSystem& System = GetWindowSystem();
	RefPtr<BaseWindow> pWnd = System.GetNameWindow( Name, Param );
	if( pWnd != NULL )
	{
		System.MoveTop( pWnd );
		return pWnd;
	}
	pWnd = CreateWnd( Name );
	if( pWnd == NULL )return NULL;

	if( Param.vt != VT_EMPTY )pWnd->SetParameter( Param );
	pWnd->SetDefaultPosition( System );
	System.AddWindow( pWnd );
	return pWnd;
}

BaseWindow* StrCommander::ShowWindow( const String& Name, const CComVariant& Param ) const
{
	RbWindowSystem& System = GetWindowSystem();
	RefPtr<BaseWindow> pWnd = System.GetNameWindow( Name );
	bool bCreated = false;
	if( pWnd == NULL )
	{
		pWnd = CreateWnd( Name );
		if( pWnd == NULL )return NULL;
		bCreated = true;
	}
	if( Param.vt != VT_EMPTY )pWnd->SetParameter( Param );
	if( bCreated )pWnd->SetDefaultPosition( System );
	System.AddWindow( pWnd );

	return pWnd;
}

bool StrCommander::SetParameter( const String& Name, const CComVariant& Param ) const
{
	RefPtr<BaseWindow> pWnd = GetWindowSystem().GetNameWindow( Name );
	if( pWnd == NULL )return false;
	return pWnd->SetParameter( Param );
}

bool StrCommander::New( RbWindowSystem& System ) const
{
	CComVariant Param;
	if( !Commands[3].empty() )Param = Commands[3];
	return (NewWindow( Commands[2], Param )!=NULL);

}

bool StrCommander::Show( RbWindowSystem& System ) const
{
	CComVariant Param;
	if( !Commands[3].empty() )Param = Commands[3];

	return (ShowWindow( Commands[2], Param )!=NULL);
}

bool StrCommander::ShowMemberState( LPCTSTR PasonName ) const
{
	return ShowMemberState( GetCache().Pason.GetPasonID(PasonName) );
}

bool StrCommander::ShowMemberState( long PasonID ) const
{
	const RbGameData& UserData = GetGameData();
	MemberStatus Mem;
	if( !UserData.GetMember( PasonID, Mem ) )return false;

	const CComVariant EquipType = RbMember(Mem).GetEquip();
	long PatyID = UserData.GetPatyID( Mem.PasonName );

	RbWindowSystem& System = GetWindowSystem();
	RefPtr<BaseWindow> pFindWnd;

	pFindWnd = System.GetNameWindow( _T("PatyEdit") );
	if( pFindWnd != NULL && PatyID >= 0 )pFindWnd->SetParameter( PatyID );

	pFindWnd = System.GetNameWindow( _T("ItemList") );
	if( pFindWnd != NULL )pFindWnd->SetParameter( EquipType );
	pFindWnd = System.GetNameWindow( _T("ShopList") );
	if( pFindWnd != NULL )pFindWnd->SetParameter( EquipType );

	pFindWnd = ShowWindow( _T("EqStatus") );
	if( pFindWnd != NULL )pFindWnd->SetParameter( (LPCTSTR)Mem.PasonName );

	return true;
}

BaseWindow* StrCommander::ShowBattleStatus( RbStatus* pStatus ) const
{
	static LPCTSTR STATUS = _T("Status");
	static LPCTSTR MINISTATUS = _T("MiniStatus");
	CComVariant Param;
	Param.vt = VT_PTR;
	Param.byref = pStatus;
	RbWindowSystem& System = GetWindowSystem();

	RefPtr<BaseWindow> pFindWnd = System.GetNameWindow( STATUS, Param );
	if( pFindWnd != NULL )
	{
		System.MoveTop( pFindWnd );
		return pFindWnd;
	}
	pFindWnd = System.GetNameWindow( MINISTATUS, Param );
	if( pFindWnd != NULL )
	{
		System.MoveTop( pFindWnd );
		return pFindWnd;
	}
	return NewWindow( STATUS, Param );
}

bool StrCommander::SetBord( BattleSystem& System ) const
{
	const String& BordName = Commands[2];
	if( BordName.empty() )return false;

	RbPosition Pos( 0, 0, 0 );
	if( Commands.Count() <= 4 )
	{
		RefPtr<RbActStatus> pStatus = System.GetCharactor( Commands[3] );
		if( pStatus != NULL )Pos = pStatus->GetPosition();
		else Pos = System.Land.GetStrPos(Commands[3]);
	}
	else
	{
		Pos.x = Commands[3].ToFloat();
		Pos.y = Commands[4].ToFloat();
		Pos.z = System.Land.GetHight( Pos.x, Pos.y );
		Pos.z += Commands[5].ToFloat();
	}
	return System.TobiPlaneSys.AddBord( BordName, Pos );
}

bool StrCommander::Ex() const
{
	const String& Com = Commands[1];
	if( Com == _T("gameover") )return g_pSystem->GameOverJob();
	if( Com == _T("shake") )
	{
		GetRbDisplay().SetShake(Commands[2].ToFloat(),Commands[3].ToFloat());
		return true;
	}
	if( Com == _T("systemwait") ){g_pSystem->TimeSystem.Sleep();return true;}
	if( Com == _T("newscheck") )return NewsCheck();
	if( Com == _T("loadcheck") )return LoadCheck();
	return true;
}

bool StrCommander::NewsCheck() const
{
	NewsChecker Checker;
	Checker.SetCheckList( Commands, 2 );
	if( !Checker.CheckALL() )return false;
	return true;
}

bool StrCommander::LoadCheck() const
{
	RefPtr<BaseWindow> pWnd = CreateWnd(_T("Load"));
	if( pWnd == NULL )return false;
	return pWnd->CheckParameter(true);
}

bool StrCommander::MemberCheck() const
{
	const RbGameData& UserData = GetGameData();
	long Count = Commands.Count();
	for( int i = 2; i < Count; i++ )
	{
		if( !UserData.InMember( Commands[i] ) )return false;
	}
	return true;
}

bool StrCommander::Param() const
{
	StrParamCom Com( Commands );
	return Com.Param( Commands );
}
