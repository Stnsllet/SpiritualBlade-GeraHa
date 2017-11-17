
#include "stdafx.h"
#include "XMLList.h"
#include "GameSystem.h"
#include "DirectEvent.h"
#include "TrapSystem.h"
#include "StrCommander.h"
#include "JobEvent.h"

bool JobEvent::EventHouse::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	HouseName = EventXML.AttributeString(TAG::NAME);
	if( HouseName.empty()  )return false;
	Rnd = EventXML.AttributeLong(_T("rnd"));
	if( Rnd == 0 )Rnd=200;

	SorceXML.XMLRead( EventXML );
	return true;
}

bool JobEvent::EventHouse::Job( GameSystem& System )
{
	AllWorld& World = GetWorldSystem();
	if( !World.CheckHouse( HouseName ) )return false;
	if( !World.RndCheck(Rnd) )return false;

	DirectEvent::Choice ChoiceEvent;
	if( ChoiceEvent.Event( SorceXML, System ) )World.OutRandom();
	return false;
}

bool JobEvent::EventState::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	String Name = EventXML.AttributeString(TAG::NAME);

	BattleSystem& BatSystem = System.BattleSys;
	pStatus = BatSystem.GetCharactor(Name);
	if( pStatus == NULL  )return false;

	const RbCache& Cache = System.Cache;
	SorceXML.XMLRead( EventXML );

	CheckHp = EventXML.AttributeFloat(TAG::HP)/100.0f;
	CheckMp = EventXML.AttributeFloat(TAG::MP)/100.0f;

	CheckAct = eActNone;
	String ActName = EventXML.AttributeString(_T("act"));
	if( !ActName.empty() )CheckAct = Cache.RbTex.GetBattleID(ActName);

	SpecialID = 0;
	String SpecialName = EventXML.AttributeString(_T("special"));
	if( !SpecialName.empty() )
	{
		CheckAct = eActSpecial;
		SpecialID = Cache.Skill.GetSkillNo( SpecialName );
	}

	return true;
}

bool JobEvent::EventState::Job( GameSystem& System )
{
	if( pStatus == NULL )return true;
	if( pStatus->IsUnView() )return true;
	if( CheckAct != eActNone )if( pStatus->GetAction() != CheckAct )return false;
	if( SpecialID > 0 )if( SpecialID != pStatus->AttackAlgo.SpecialID() )return false;
	if( CheckHp > 0 )if( !pStatus->HpCheck( CheckHp ) )return false;
	if( CheckMp > 0 )if( !pStatus->MpCheck( CheckMp ) )return false;
//	if( pStatus->IsDead() )return true;

	System.EventSystem.ChildEvent( SorceXML );

	return true;
}

bool JobEvent::EventAct::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	BattleSystem& BatSystem = System.BattleSys;
	const RbCache& Cache = System.Cache;

	String Name = EventXML.AttributeString(TAG::NAME);
	pStatus = BatSystem.GetCharactor(Name);
	if( pStatus == NULL  )return false;
	ActionID = -1;
	TagetPos = pStatus->GetPosition();
	Direction = pStatus->GetAngle();

	String ActionName = EventXML.AttributeString(_T("action"));
	if( !ActionName.empty() )ActionID = Cache.ActID.GetID( ActionName );

	String MovePos = EventXML.AttributeString(_T("move"));
	if( !MovePos.empty() )
	{
		TagetPos = BatSystem.Land.GetStrPos( MovePos );
		Direction = pStatus->GetPosition().GetDirection(TagetPos);
	}

	static LPCTSTR ANGLE = _T("angle");
	if( EventXML.HasAttribute(ANGLE) )
		Direction = EventXML.AttributeFloat(ANGLE);

	ActTime = 0;
	ActStart = EventXML.AttributeFloat(_T("start"));
	ActLife = EventXML.AttributeFloat(_T("time"));
	String Look = EventXML.AttributeString(_T("look"));
	if( !Look.empty() )
	{
		Direction = pStatus->TagetAngle( BatSystem.GetCharactor( Look ) );
	}
	pStatus->SetActImage( ActionID, Direction, (int)(ActTime+ActStart) );

	LastTime = RbTimer();
	return true;
}

bool JobEvent::EventAct::Job( GameSystem& System )
{
	if( pStatus == NULL )return true;
	RbTimer NewTime;
	double Passed = NewTime.Passed( LastTime );
	if( Passed+ActTime > ActLife )
	{
		Passed = ActLife-ActTime;
		ActTime = ActLife;
	}
	else ActTime += Passed;

	const RbPosition& Pos = pStatus->GetPosition();
	if( Pos != TagetPos )
	{
		RbPosition NewPos = Sizing(TagetPos, Pos, Passed, ActLife-ActTime );
		pStatus->AlgoMove.SetPosition( NewPos );
	}
	if( ActionID > 0 )pStatus->SetActImage( ActionID, Direction, (int)(ActTime+ActStart) );

	if( ActTime >= ActLife )return true;

	LastTime = NewTime;
	return false;
}

bool JobEvent::EventDelay::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	Time = EventXML.AttributeFloat(_T("time"));

	if( EventXML.AttributeBool(_T("lock")) )Lock.Lock( GetEventSystem().ActLock );

	SorceXML.XMLRead( EventXML );
	StartTime = RbTimer();

	return true;
}

bool JobEvent::EventDelay::Job( GameSystem& System )
{
	double Passed = RbTimer().Passed( StartTime );
	if( Passed < Time )return false;

	System.EventSystem.ChildEvent( SorceXML );
	return true;
}

bool JobEvent::EventTimer::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	Time = EventXML.AttributeFloat(_T("time"));
	Cycle = EventXML.AttributeLong(_T("cycle"));
	if( Cycle == 0 )Cycle = 1;
	SorceXML.XMLRead( EventXML );
	StartTime = System.BattleSys.GetBattleTime();

	return true;
}

bool JobEvent::EventTimer::Job( GameSystem& System )
{
	double Passed = System.BattleSys.GetBattleTime() - StartTime;
	if( Passed < Time )return false;

	System.EventSystem.ChildEvent( SorceXML );
	if( Cycle > 0 )--Cycle;
	StartTime += Time;
	return (Cycle==0);
}

bool JobEvent::EventCount::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	CheckName = EventXML.AttributeString(TAG::NAME);
	CheckCount = EventXML.AttributeLong(_T("count"));
	String Army = EventXML.AttributeString(_T("leader"));
	if( Army.empty() )ArmyID = -1;
	else ArmyID = ArmyStatus::GetArmyID( Army );

	SorceXML.XMLRead( EventXML );

	return true;
}

bool JobEvent::EventCount::Job( GameSystem& System )
{
	if( ArmyID >= 0 )
	{
		long Live = System.BattleSys.LeaderCount( ArmyID );
		if( Live > CheckCount )return false;
	}
	if( !CheckName.empty() )
	{
		long Live = System.BattleSys.ArmyData( CheckName ).RefLiveCount();
		if( Live > CheckCount )return false;
	}

	System.EventSystem.ChildEvent( SorceXML );
	return true;
}

bool JobEvent::EventRange::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	Range = EventXML.AttributeFloat(_T("length"));
	String Name = EventXML.AttributeString(TAG::NAME);
	pStatus = System.BattleSys.GetCharactor( Name );
	String Taget = EventXML.AttributeString(_T("taget"));
	pTaget = System.BattleSys.GetCharactor( Taget );
	bPos = false;
	String PosStr = EventXML.AttributeString(_T("pos"));
	if( !PosStr.empty() )
	{
		Pos = System.ActiveLand().GetStrPos( PosStr );
		bPos = true;
	}
	else if( pStatus == NULL )return false;
	EneCount = EventXML.AttributeLong(_T("count"));

	SorceXML.XMLRead( EventXML );

	return true;
}

bool JobEvent::EventRange::Job( GameSystem& System )
{
	if( pStatus != NULL )
	{
		if( pStatus->IsDead() )return true;
		const RbPosition& Ref = pStatus->GetPosition();
		if( pTaget != NULL )
		{
			if( pTaget->IsDead() )return true;
			if( Ref.Length(pTaget->GetPosition()) > Range )return false;
		}
		if( bPos )if( Ref.Length(Pos) > Range )return false;
	}
	if( EneCount > 0 )
	{
		if( pStatus != NULL )
		{
			if( EneCount > System.BattleSys.GetCharaCount( pStatus->GetPosition(),
				Range, pStatus->GetArmyID() ) )return false;
		}
		else if( bPos )
		{
			if( EneCount > System.BattleSys.GetCharaCount( Pos, Range, 0, false ) )return false;
		}
	}

	System.EventSystem.ChildEvent( SorceXML );
	return true;
}

bool JobEvent::EventTres::ReadEvent( const XMLPaser& EventXML, GameSystem& System )
{
	String StrPos = EventXML.AttributeString(_T("pos"));
	if( StrPos.empty() )return false;
	LandScape& Land = System.ActiveLand();
	Pos = Land.GetStrPos(StrPos);
	const float LandHight = Land.GetHight( Pos.x, Pos.y );
	RbPosition PolyPos( Pos.x, Pos.y, Pos.z-LandHight);

	String PolyName = EventXML.AttributeString(_T("poly"));
	if( !PolyName.empty() )pStation = Land.AddPolygon( PolyName, PolyPos, EventXML.AttributeFloat(_T("dir")) );

	Range = 0;
	if( pStation != NULL )
	{
		RefPtr<RbPolyPoly> pPoly = pStation->RefPoly();
		if( pPoly != NULL )Range = pPoly->Range();
	}
	if( Range <= 0 )Range = System.BattleSys.FormSize;

	TrapRnd = EventXML.AttributeLong(_T("per"));
	Traps = EventXML.AttributeString(_T("traps"));
	MonsterPath = EventXML.AttributeString(_T("mons"));

	SorceXML.XMLRead( EventXML );

	return true;
}

bool JobEvent::EventTres::Job( GameSystem& System )
{
	float Check = 0;
	RefPtr<RbActStatus> pLeader = System.BattleSys.GetFriendPaty( Pos, 0, RbActStatus::ePatyNoState, &Check );
	if( Check > Range )return false;

	if( pStation != NULL )
	{
		System.ActiveLand().DeletePolygon( pStation );
	}
	if( Rnd100(TrapRnd) )TrapSystem(System).Trap( pLeader, Traps, MonsterPath );
	else System.EventSystem.ChildEvent( SorceXML );
	return true;
}
