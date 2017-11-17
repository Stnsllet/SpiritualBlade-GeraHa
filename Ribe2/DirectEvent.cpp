// DirectEvent.cpp: DirectEvent クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "XMLTranser.h"
#include "GameSystem.h"
#include "RbEventSystem.h"
#include "StrCommander.h"
#include "DirectEvent.h"

// CallBacks

DirectEvent::BatCallBacks::ChangeTactics::ChangeTactics( BattleSystem& RefSystem )
: System( RefSystem ), NewType( eTacticsNone )
{
}

long DirectEvent::BatCallBacks::ChangeTactics::Change( const String& RefName, const String& NewTactics )
{
	NewType = TacticsAlgo::GetTactics( NewTactics );
	return System.NameCall( this, RefName );
}

bool DirectEvent::BatCallBacks::ChangeTactics::Call( RbActStatus& State )
{
	RefPtr<RbBattlePaty> pPaty = State.GetPaty();
	if( pPaty == NULL )return false;
	pPaty->ChangeTactics( NewType );
	return true;
}

DirectEvent::BatCallBacks::SpecialAttack::SpecialAttack( GameSystem& RefSystem )
: System( RefSystem )
{
}

long DirectEvent::BatCallBacks::SpecialAttack::Go( const String& RefName, const String& Special )
{
	SkillID = System.Cache.Skill.GetSkillNo( Special );
	if( SkillID < 0 )return 0;
	return System.BattleSys.NameCall( this, RefName );
}

bool DirectEvent::BatCallBacks::SpecialAttack::Call( RbActStatus& State )
{
	return State.SpecialGo( SkillID );
}

DirectEvent::BatCallBacks::Escape::Escape( BattleSystem& RefSystem )
: System( RefSystem ), Type( eEscapeNone )
{
}

long DirectEvent::BatCallBacks::Escape::Go( const String& RefName, const String& EscapeType )
{
	if( EscapeType == _T("paty") )Type = ePaty;
	else if( EscapeType == _T("army") )Type = eArmy;

	if( Type == eEscapeNone )return 0;

	return System.NameCall( this, RefName );
}

bool DirectEvent::BatCallBacks::Escape::Call( RbActStatus& State )
{
	switch( Type )
	{
	case ePaty:
		{
		RefPtr<RbBattlePaty> pPaty = State.GetPaty();
		if( pPaty != NULL )return pPaty->EscapePaty();
		}
		return false;
	case eArmy:
		return (0<System.EscapeArmy( State.GetArmyID() ));
	}
	return false;
}

DirectEvent::BatCallBacks::BraveChange::BraveChange( BattleSystem& RefSystem )
: System( RefSystem )
{
}

long DirectEvent::BatCallBacks::BraveChange::Go( const String& RefName, float Brave )
{
	if( Brave == 0 )return 0;
	System.NameCall( this, RefName );

	std::map<long,RefPtr<RbBattlePaty> >::const_iterator itr = PatyList.begin();
	while( itr != PatyList.end() )
	{
		RefPtr<RbBattlePaty> pPaty = (itr++)->second;
		if( pPaty != NULL )pPaty->BraveUp( Brave );
	}

	return PatyList.size();
}

bool DirectEvent::BatCallBacks::BraveChange::Call( RbActStatus& State )
{
	RefPtr<RbBattlePaty> pPaty = State.GetPaty();
	if( pPaty == NULL )return false;

	PatyList[pPaty->ID()] = pPaty;
	return true;
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

bool DirectEvent::CheckFlags( const XMLPaser& EventXML, GameSystem& System )
{
	const RbUserData& User = System.GameData;
	String OnList = EventXML.AttributeString(TAG::ON);
	if( !User.CheckFlgs( OnList, true ) )return false;
	String OffList = EventXML.AttributeString(TAG::OFF);
	if( !User.CheckFlgs( OffList, false ) )return false;

	return true;
}

bool DirectEvent::EventBase::Event( const XMLPaser& EventXML, GameSystem& System )
{
	RbGameData& User = System.GameData;
	String Flg = EventXML.AttributeString(TAG::FLG);
	if( !Flg.empty() )if( User.GetFlg(Flg) )return false;	//済み
	String UnFlg = EventXML.AttributeString(TAG::UNFLG);
	if( !UnFlg.empty() )if( !User.GetFlg(UnFlg) )return false;	//済み

	if( !DirectEvent::CheckFlags( EventXML, System ) )return false;

	if( !Flg.empty() )User.SetFlg(Flg);
	if( !UnFlg.empty() )User.SetFlg(UnFlg,false);

	System.EventSystem.ChildEvent( EventXML );
	return true;
}

bool DirectEvent::Sound::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String BGM = EventXML.AttributeString(_T("bgm"));
	if( !BGM.empty() )
	{
		System.BGMSystem.Play( BGM );
		System.TimeSystem.Sleep();
	}
	String wave = EventXML.AttributeString(_T("wave"));
	if( !wave.empty() )
	{
		System.SoundSystem.Play( wave );
	}
	return true;
}


bool DirectEvent::Paty::Event( const XMLPaser& EventXML, GameSystem& System )
{
	System.BattleSys.AddPaty( EventXML );
	return true;
}

bool DirectEvent::Land::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Path = EventXML.AttributeString(TAG::PATH);
	LandScape& Land = System.BattleSys.Land;
	Land.LoadLandScape( Path );
	if( &Land == &(System.ActiveLand()) )
		System.DisplaySystem.SetViewArea( Land.Map.LandArea() );

	float x = EventXML.AttributeFloat(TAG::X);
	float y = EventXML.AttributeFloat(TAG::Y);

	if( x == 0 && y == 0 )
	{
		x = Land.GetLandCenterPos().x;
		y = Land.GetLandCenterPos().y;
	}

	System.DisplaySystem.SetViewPos(x, y);
	return true;
}

bool DirectEvent::Move::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String name = EventXML.AttributeString(TAG::NAME);

	BattleSystem& Sys = System.BattleSys;

	BattleSystem::CharaList List;
	if( 0 >= Sys.GetCharactors( List, name ) )return false;

	RbPosition TagetPos;
	String taget = EventXML.AttributeString(_T("taget"));
	if( taget.empty() )
	{
		TagetPos.x = EventXML.AttributeFloat(TAG::X);
		TagetPos.y = EventXML.AttributeFloat(TAG::Y);
	}else
	{
		RefPtr<RbActStatus> pTaget = Sys.GetCharactor( taget );
		if( pTaget == NULL )return false;
		TagetPos = pTaget->GetPosition();
	}
	bool bShift = EventXML.AttributeBool(_T("shift"));

	BattleSystem::CharaItr Itr( List );
	while( !Itr.IsEnd() )
	{
		if( Itr->GetPatyState() != RbActStatus::ePatyBattle )
		{
			if( !bShift )Itr->AlgoMove.SetMovePos( TagetPos.x, TagetPos.y );
			else
			{
				RbPosition Pos = Itr->GetPosition();
				Pos += TagetPos;
				Itr->AlgoMove.SetMovePos( Pos.x, Pos.y );
			}
		}
		Itr.Next();
	}
	return true;
}

bool DirectEvent::Msg::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Name = EventXML.AttributeString(TAG::NAME);

	RefPtr<RbActStatus> pTaget = System.BattleSys.GetCharactor( Name );
/*	if( !Name.empty() )
	{
		if( pTaget == NULL )return false;
		if( pTaget->IsDeleteOK() )return false;
	}
*/
	return System.WindowSys.Message( pTaget, EventXML );
}

bool DirectEvent::Cache::Event( const XMLPaser& EventXML, GameSystem& System )
{
	if( System.Config.System.bDontCache )return false;

	String BGMName = EventXML.AttributeString(_T("bgm"));
	if( !BGMName.empty() )System.BGMSystem.Cache( BGMName );

	RbCache& Cache = System.Cache;
	String Tex = EventXML.AttributeString(_T("tex"));
	if( !Tex.empty() )Cache.RbTex.Cache( Tex );

	String TexPath = EventXML.AttributeString(_T("texpath"));
	if( !TexPath.empty() )Cache.PathTex.Cache( TexPath );

	String PolyPath = EventXML.AttributeString(_T("poly"));
	if( !PolyPath.empty() )Cache.Poly.Cache( PolyPath );

	String WavePath = EventXML.AttributeString(_T("wave"));
	if( !WavePath.empty() )System.SoundSystem.Cache( WavePath );

	String XMLPath = EventXML.AttributeString(_T("xml"));
	if( !XMLPath.empty() )Cache.XML.Cache( XMLPath );

	String Clear = EventXML.AttributeString(_T("clear"));
	if( !Clear.empty() )
	{
		if( Clear==_T("vram") )System.DisplaySystem.FreeMemory();
		if( Clear==_T("bgm") )System.BGMSystem.ReleaseCache();
		if( Clear==_T("tex") )
		{
			Cache.Poly.Clear();
			Cache.RbTex.Clear();
			Cache.Face.Clear();
			Cache.Plane.Clear();
			Cache.PathTex.Clear();
		}
		if( Clear==_T("wave") )System.SoundSystem.ClearCache();
		if( Clear==_T("xml") )Cache.XML.Clear();
		if( Clear==_T("all") )System.ReleaseCache(false);
	}
	return true;
}

bool DirectEvent::LoadEvent::Event( const XMLPaser& EventXML, GameSystem& System )
{
	HRESULT ret = System.EventSystem.LoadEvent( String(EventXML.AttributeString(TAG::PATH)) );
	return !FAILED(ret);
}

bool DirectEvent::List::Event( const XMLPaser& EventXML, GameSystem& System )
{
	XMLList EventList;
	EventList.Child( EventXML );
	return ( System.EventSystem.AddList(EventList) > 0 );
}

bool DirectEvent::ZoomIn::Event( const XMLPaser& EventXML, GameSystem& System )
{
	LPCTSTR ZOOM = _T("zoom");
	LPCTSTR ANGLE = _T("angle");
	LPCTSTR LIFE = _T("life");
	RbDisplay& Disp = System.DisplaySystem;

	double Life = 500;
	if( EventXML.HasAttribute(LIFE) )
		Life = EventXML.AttributeFloat(LIFE);

	if( EventXML.HasAttribute(ZOOM) )
		Disp.SetZoom( EventXML.AttributeFloat(ZOOM), Life );
	if( EventXML.HasAttribute(ANGLE) )
		Disp.SetAngle( EventXML.AttributeFloat(ANGLE), Life );

	String Name = EventXML.AttributeString(TAG::NAME);
	if( !Name.empty() )
	{
		const BattleSystem& Battle = System.BattleSys;
		if( GetWindowSystem().OnMsg() )Battle.LoackView( Name, Life );
		else Battle.SetViewOn( Name );
	}

	String PosStr = EventXML.AttributeString(_T("pos"));
	if( !PosStr.empty() )
	{
		const RbPosition& Pos = System.ActiveLand().GetStrPos(PosStr);
		System.DisplaySystem.SetViewOn( Pos, Life ); 
	}
	return true;
}

bool DirectEvent::Mode::Event( const XMLPaser& EventXML, GameSystem& System )
{
	GameMode NewMode = eNoneMode;
	String Mode = EventXML.AttributeString(_T("mode"));
	if( Mode == _T("battle") )NewMode = eBattleMode;
	if( Mode == _T("world") )NewMode = eWorldMode;
	if( Mode == _T("event") )NewMode = eEventMode;
	if( Mode == _T("none") )NewMode = eNoneMode;

//	if( NewMode == eNoneMode )return false;
	return System.ChangeGameMode( NewMode );
}

bool DirectEvent::AddLine::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Pos1 = EventXML.AttributeString(_T("pos1"));
	String Pos2 = EventXML.AttributeString(_T("pos2"));
	return System.WorldSystem.AddLine( Pos1, Pos2 );
}

bool DirectEvent::Choice::Event( const XMLPaser& EventXML, GameSystem& System )
{
	XMLList EventList;
	EventList.Child( EventXML );
	int Count = EventList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		if( System.EventSystem.Event( EventList.GetItem(i), System ) )return true;
	}
	return false;
}

bool DirectEvent::ChkFlg::Event( const XMLPaser& EventXML, GameSystem& System )
{
	if( !DirectEvent::CheckFlags( EventXML, System ) )return false;

	System.EventSystem.ChildEvent( EventXML );
	return true;
}

bool DirectEvent::Tutorial::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return System.WindowSys.Tutorial( EventXML );
}

bool DirectEvent::Rnd::Event( const XMLPaser& EventXML, GameSystem& System )
{
	long World = EventXML.AttributeLong(_T("monster"));
	if( World != 0 )
	{
		if( !System.WorldSystem.RndCheck( World ) )return false;
	}
	else
	{
		long R = EventXML.AttributeLong(_T("p"));
		if( !Rnd100(R) )return false;
	}

	return (System.EventSystem.ChildEvent( EventXML )>0);
}

bool DirectEvent::Command::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Com = EventXML.AttributeString(_T("com"));
	if( Com.empty() )return false;

	StrCommander StrCom;
	if( !StrCom.Command( Com ) )return false;
	System.EventSystem.ChildEvent( EventXML );
	return true;
}

bool DirectEvent::UserData::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return GetGameData().XMLRead( EventXML );
}

bool DirectEvent::Title::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return System.WindowSys.Title( EventXML );
}

bool DirectEvent::Result::Event( const XMLPaser& EventXML, GameSystem& System )
{
	System.BattleSys.BStatus.RefResult().SetXML( EventXML );
	if( !EventXML.HasAttribute(TAG::MSG) )return true;
	return System.WindowSys.Result( EventXML );
}

bool DirectEvent::Powered::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return System.WindowSys.Powered( EventXML );
}

bool DirectEvent::Poly::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return System.ActiveLand().SetPolygon( EventXML );
}

bool DirectEvent::Shop::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String ShopItems = EventXML.AttributeString(_T("items"));
	System.Shop.SetShopData(ShopItems);
	String EmployJobs = EventXML.AttributeString(_T("employ"));
	System.Shop.SetEmployJob(EmployJobs);
	return true;
}

bool DirectEvent::Popup::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Name = EventXML.AttributeString(TAG::NAME);
	RefPtr<RbActStatus> pTaget = System.BattleSys.GetCharactor( Name );

	return System.WindowSys.PopupMsg( pTaget, EventXML );
}

bool DirectEvent::Param::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Name = EventXML.AttributeString(TAG::NAME);
	String Com = EventXML.AttributeString(_T("com"));
	const CComBSTR& StrValue = EventXML.AttributeString(_T("val"));

	if( !System.GameData.ExecUserParam( Name, Com, StrValue ) )return false;
	System.EventSystem.ChildEvent( EventXML );
	return true;
}

bool DirectEvent::Bat::Event( const XMLPaser& EventXML, GameSystem& System )
{
	const String Name = EventXML.AttributeString(TAG::NAME);
	if( Name.empty() )return false;

	String Tactics = EventXML.AttributeString(_T("tactics"));
	if( !Tactics.empty() )
	{
		BatCallBacks::ChangeTactics Tac( System.BattleSys );
		return ( Tac.Change( Name, Tactics ) > 0 );
	}
	String Special = EventXML.AttributeString(_T("waza"));
	if( !Special.empty() )
	{
		BatCallBacks::SpecialAttack Atc( System );
		return ( Atc.Go( Name, Special ) > 0 );
	}
	const float Brave = EventXML.AttributeFloat(_T("brave"));
	if( Brave != 0 )
	{
		BatCallBacks::BraveChange BraveC( System.BattleSys );
		return ( BraveC.Go( Name, Brave ) > 0 );
	}

	String Escape = EventXML.AttributeString(_T("escape"));
	if( !Escape.empty() )
	{
		BatCallBacks::Escape Esc( System.BattleSys );
		return ( Esc.Go( Name, Escape ) > 0 );
	}

	return false;
}

bool DirectEvent::CallTrans::Event( const XMLPaser& EventXML, GameSystem& System )
{
	String Path = EventXML.AttributeString(TAG::PATH);
	XMLPaser XsltXML = System.Cache.XML.Get( Path );
	if( !XsltXML )return false;

	XMLTranser Xslt;
	if( FAILED(Xslt.SetTemplate( XsltXML )) )return false;
	XMLTranser::Processor Trans( Xslt );
	StringSplitter<> Params = String(EventXML.AttributeString(_T("params")));
	long Count = Params.Count();
	for( int i = 0; i < Count; i++ )
	{
		Trans.AddParam( CComBSTR(Params[i]), System.GameData.GetUserParam(Params[i]) );
	}
	CComPtr<XMLPaser::MyNode> pNode;
	if( FAILED(Trans.Trans( EventXML, &pNode )) )return false;
	ParamXML NewEvent;
	if( FAILED(NewEvent.XMLRead( pNode )) )return false;
	ATLTRACE( "CallTrans\n%s\n", (LPCTSTR)String(NewEvent.AllXML()) );

	System.EventSystem.Event( NewEvent, System );
	return true;
}

bool DirectEvent::Window::Event( const XMLPaser& EventXML, GameSystem& System )
{
	return System.WindowSys.ComWindow( EventXML );
}
