// AllWorld.cpp: AllWorld クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "RbSprite.h"
#include "RbTransform.h"
#include "XMLList.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "BillbordPlane.h"
#include "DispStocker.h"
#include "GameSystem.h"
#include "Draws.h"
#include "AllWorld.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


bool AllWorld::SetWorldSystem( const XMLPaser& WorldXML )
{
	float DefaultSpeed = WorldXML.AttributeFloat(_T("speed"));
	if( DefaultSpeed > 0 )MoveSpeed = DefaultSpeed/1000;

	String MainName = WorldXML.AttributeString(_T("main"));
	if( MainName != NULL )BaseMainChara = MainName;

	float Scale = WorldXML.AttributeFloat(_T("movingzoom"));
	if( Scale > 0 )MovingZoom = Scale;

	Scale = WorldXML.AttributeFloat(_T("maxzoom"));
	if( Scale > 0 )MaxScale = Scale;
	Scale = WorldXML.AttributeFloat(_T("minzoom"));
	if( Scale > 0 )MinScale = Scale;

	MainChara.DispScale = WorldXML.AttributeFloat(_T("charascale"));
	if( MainChara.DispScale <= 0 )MainChara.DispScale = 1;

	return true;
}

AllWorld::AllWorld()
{
	OldTime = 0;
	MoveSpeed = 200.0f /1000;
	BaseMainChara = _T("roid");
	BaseZoom = MovingZoom = 1.0;
	bViewLock = true;
	LastRnd = 99;
	MinScale = 0.2f;
	MaxScale = 0.5f;
	MainChara.DispScale = 1.0f;
}

AllWorld::~AllWorld()
{
}

void AllWorld::ClearData()
{
	LastRnd = 100;
	OldTime = RbTimer();
	LineList.clear();
	MainChara.NextMove.clear();

	HouseDataMap::iterator itr = HouseDataList.begin();
	while( itr != HouseDataList.end() )
		(itr++)->second.bView = false;
}

// ランダムチェック
bool AllWorld::RndCheck( int Check ) const
{
	if( Check < 0 )return (LastRnd<100 && MainChara.NextMove.size()<=0);
	return (Check > LastRnd);
}

// モード変更
bool AllWorld::ChangeGameMode( GameMode NewMode )
{
	if( NewMode != eWorldMode )return false;
	RbDisplay& Disp = GetRbDisplay();
	Disp.SetViewPos( MainChara.Pos.x, MainChara.Pos.y );

	HouseDataMap::const_iterator itr = HouseDataList.find(MainChara.NowHouse);
	if( itr == HouseDataList.end() )return true;

	MainChara.Pos = itr->second.Pos;
	return true;
}

bool AllWorld::SetBaseZoom( float NewZoom )
{
	if( NewZoom <= 0 )return false;
	RbDisplay& Disp = GetRbDisplay();
	BaseZoom = NewZoom;
	Disp.SetZoom( NewZoom, 0 );
	return true;
}

// 初期化
HRESULT AllWorld::InitWorld( RbDisplay& Display )
{
	ClearData();

	HRESULT ret = HousePlaneList.LoadPlane( Display, _T("System/HouseTexture.xml") );
	if( FAILED(ret) )return ret;

	ret = LoadAllHouse( _T("Data/AllHouse.xml") );
	if( FAILED(ret) )return ret;

	Land.InitLand( Display );
	ret = Land.LoadLandScape( LandPath );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// メインキャラ変更
bool AllWorld::ChangeMainChara( LPCTSTR Name )
{
	if( MainChara.MainName == Name )return false;

	RefPtr<RbTexture> pGetTexture;
	GetCache().GetTexture( Name, pGetTexture );
	if( pGetTexture == NULL )return false;

	MainChara.MainName = Name;
	MainChara.pTexture = pGetTexture;
	return true;
}

// ハウス情報 ロード
HRESULT AllWorld::LoadAllHouse( LPCTSTR FileName )
{
	XMLPaser AllHouseXML;
	HRESULT ret = AllHouseXML.Load( FileName );
	if( FAILED(ret) )return ret;

	LandPath = AllHouseXML.AttributeString(_T("worldpath"));

	XMLList HouseList;
	ret = HouseList.Query(AllHouseXML, _T("House") );
	if( FAILED(ret) )return ret;
	long Count = HouseList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadHouse( HouseList.GetItem(i) );
	}
	return S_OK;
}

// ハウス情報 ロード
HRESULT AllWorld::ReadHouse( const XMLPaser& HouseXML )
{
	HouseData ReadData;
	ReadData.NameID = HouseXML.AttributeString(TAG::NAME);
	if( ReadData.NameID.empty() )return E_FAIL;

	ReadData.Type = HouseXML.AttributeString(TAG::TYPE);
	ReadData.Name = HouseXML.AttributeString(TAG::STR);
	ReadData.Help = HouseXML.AttributeString(_T("help"));
	ReadData.Pos.x = HouseXML.AttributeFloat(TAG::X);
	ReadData.Pos.y = HouseXML.AttributeFloat(TAG::Y);
	ReadData.Pos.z = Land.GetHight( ReadData.Pos.x, ReadData.Pos.y );
	const Plane<HandTexRect>& HousePlane = HousePlaneList.GetPlane(ReadData.Type);
	ReadData.BordPlane.SetData( HousePlane, HousePlane );
	ReadData.bView = false;

	ATLASSERT( HouseDataList.find(ReadData.NameID) == HouseDataList.end() );
	HouseDataList[ReadData.NameID] = ReadData;
	return S_OK;
}

// 描写
HRESULT AllWorld::Draw( RbDisplay& Display )
{
	HRESULT ret = Land.Draw( Display );
	if( FAILED(ret) )return ret;

	DispStocker::TexUV UV( Display );
	UV.Set( D3DTADDRESS_BORDER );
	{
		DispStocker::BillBord BordSet( Display );

		DrawLine( Display );

		HouseDataMap::iterator itr = HouseDataList.begin();
		while( itr != HouseDataList.end() )
			DrawHouse( (itr++)->second, Display );
	}
	Land.DrawGhost( Display );

	RbSprite Surface;
	ret = Surface.InitSprite( Display );
	if( FAILED(ret) )return ret;

	DrawMainChara( Display, Surface );

	return S_OK;
}

// 描写
HRESULT AllWorld::DrawMainChara( RbDisplay& Display, RbSprite& Surface )
{
	if( MainChara.pTexture == NULL )
	{
		if( !ChangeMainChara(BaseMainChara) )return false;
	}
	RbPosition Pos = Display.ViewToDisp( MainChara.Pos );

	return Surface.DrawRbTexture( MainChara.pTexture,
		Pos.x, Pos.y, _T("move"), MainChara.Direction, MainChara.ActTime,
		0, MainChara.DispScale );
}

// 描写
HRESULT AllWorld::DrawHouse( HouseData& Data, RbDisplay& Display ) const
{
	if( !Data.bView )return S_FALSE;
	RbPosition LT = Display.BillBordPos( Data.BordPlane.Ver[0], Data.Pos );
	RbPosition RB = Display.BillBordPos( Data.BordPlane.Ver[2], Data.Pos );
	Data.DispRect.left = LT.x;
	Data.DispRect.right = RB.x;
	Data.DispRect.top = LT.y;
	Data.DispRect.bottom = RB.y;

	Draws::Bord Draw( Display );
	return Draw.Draw( Data.BordPlane, Data.Pos );
}

// ライン描写
HRESULT AllWorld::DrawLine( RbDisplay& Display ) const
{
	Draws::Liner LineDraw;
	Plane<HandTexRect> LPlane = HousePlaneList.GetPlane(_T("line"));
	if( !LineDraw.InitLine( LPlane, LPlane ) )return false;

	std::list<LineData>::const_iterator itrL = LineList.begin();
	while( itrL != LineList.end() )
	{
		LineDraw.Draw( Display, itrL->Pos1, itrL->Pos2 );
		itrL++;
	}

	return S_OK;
}

// ライン描写
HRESULT AllWorld::DrawLine( const BillbordPlane& LinePlane, const RbPosition& Pos1, const RbPosition& Pos2, const RbDisplay& Display ) const
{
	BillbordPlane TransPlane = LinePlane;

	RbPosition Vector = Pos2;
	Vector -= Pos1;

	RbTransform Trans;
	Trans.Scale( 1, Vector.Length(), 1 );
	Trans.Posture( Vector );
	Trans.AddScroll( Pos1 );
	TransPlane.Trans( Trans );

	return TransPlane.Draw( Display );
}

// マウス位置にある情報取得
AllWorld::HouseData AllWorld::GetHouseData( int MouseX, int MouseY ) const
{
	HouseDataMap::const_iterator itr = HouseDataList.begin();
	while( itr != HouseDataList.end() )
	{
		const HouseData& Ref = (itr++)->second;
		if( !Ref.bView )continue;
		if( Ref.DispRect.Pin( MouseX, MouseY ) )return Ref;
	}
	return HouseData();
}

// ライン追加
bool AllWorld::AddLine( LPCTSTR House1, LPCTSTR House2 )
{
	HouseDataMap::iterator itr1 = HouseDataList.find(House1);
	if( itr1 == HouseDataList.end() )return false;

	HouseDataMap::iterator itr2 = HouseDataList.find(House2);
	if( itr2 == HouseDataList.end() )return false;

	if( itr1 == itr2 )return false;

	itr1->second.bView = true;
	itr2->second.bView = true;

	LineData NewData;
	NewData.House1 = House1;
	NewData.House2 = House2;
	NewData.Pos1 = itr1->second.Pos;
	NewData.Pos2 = itr2->second.Pos;

	LineList.push_back( NewData );
	return true;
}

// 今の位置を変更する
bool AllWorld::SetPosition( LPCTSTR HouseName )
{
	RbGameData& UserData = GetGameData();
	if( HouseName == NULL )
		HouseName = UserData.GetHousePos();

	HouseDataMap::iterator itr = HouseDataList.find(HouseName);
	if( itr == HouseDataList.end() )return false;

	MainChara.NowHouse = HouseName;
	MainChara.ActTime = 0;
	MainChara.Direction = 1;

	MainChara.Pos = itr->second.Pos;
	UserData.SetHousePos( MainChara.NowHouse );
	return true;
}

// 移動を開始する
bool AllWorld::MoveStart( LPCTSTR HouseName )
{
	if( HouseName == NULL )return false;
	if( MainChara.NextMove.size() > 0 )return false;
	if( MainChara.NowHouse == HouseName )return false;

	std::map<String,String> LinkData;
	GetLinkMap( LinkData, MainChara.NowHouse, String(HouseName) );

	std::map<String,String>::const_iterator itr = LinkData.find( HouseName );
	if( itr == LinkData.end() )return false;
	while( itr->first != MainChara.NowHouse )
	{
		MainChara.NextMove.push_front( itr->first );
		itr = LinkData.find( itr->second );
	}
	return true;
}

bool AllWorld::OutRandom()
{
	LastRnd = 100;
	return true;
}

// 移動停止
bool AllWorld::StopMove()
{
	OutRandom();
	if( MainChara.NextMove.size() <= 0 )return false;
	MainChara.NextMove.clear();
	return true;
}

// リンクマップを取得
void AllWorld::GetLinkMap( std::map<String,String>& Output, const String& StartHouse, const String& TagetHouse ) const
{
	Output[StartHouse] = _T("_");	//dummy
	std::list<String> BaseList;
	BaseList.push_back( StartHouse );
	while( BaseList.size() > 0 )
	{
		std::list<String> NextBaseList;
		std::list<String>::const_iterator itrBase = BaseList.begin();
		for( ;itrBase != BaseList.end(); itrBase++ )
		{
			std::list<String> LinkList;
			GetLinkList( LinkList, *itrBase );
			std::list<String>::const_iterator itrLink = LinkList.begin();
			for( ; itrLink != LinkList.end(); itrLink++ )
				if( Output[*itrLink].empty() )
				{
					Output[*itrLink] = *itrBase;
					if( *itrLink == TagetHouse )return;
					NextBaseList.push_back( *itrLink );
				}
		}
		BaseList = NextBaseList;
	};
}

// リンク先を調べる
void AllWorld::GetLinkList( std::list<String>& Output, const String& BaseName ) const
{
	std::list<LineData>::const_iterator itr = LineList.begin();
	while( itr != LineList.end() )
	{
		const LineData& Ref = *(itr++);
		if( Ref.House1 == BaseName )Output.push_back( Ref.House2 );
		else if( Ref.House2 == BaseName )Output.push_back( Ref.House1 );
	}
}

// 移動する
bool AllWorld::MoveMainChara( LPCTSTR HouseName, double Passed )
{
	HouseDataMap::const_iterator itr = HouseDataList.find(HouseName);
	if( itr == HouseDataList.end() )return false;

	float MoveLength = MoveSpeed * Passed;
	RbPosition Vector = itr->second.Pos;
	Vector -= MainChara.Pos;
	MainChara.Direction = Vector.GetDirection();

	if( MoveLength >= Vector.Length() )
	{
		MainChara.Pos = itr->second.Pos;
		return false;
	}
	MainChara.Pos += Vector.Normalize( MoveLength );
	return true;
}

// イベント
bool AllWorld::CheckHouse( const String& House )
{
	if( MainChara.NowHouse == House )return true;
	return false;
}

// マウス
bool AllWorld::MouseJob( RbDisplay& Display, GameMessage& State )
{
	HouseData Data = GetHouseData( State.x, State.y );
	switch( State.Msg() )
	{
	case eMouseMove:
		if( State.IsDraging() )bViewLock = false;
		if( !State.IsDraging() )if( !Data.NameID.empty() )
				GetWindowSystem().Help( Data.Help, Data.Name, Data.DispRect );
		break;
	case eLDown:
		if( Data.NameID.empty() )break;
		if( !MoveStart( Data.NameID ) )break;
		Display.SetViewOn( MainChara.Pos.x, MainChara.Pos.y );
		Display.SetZoom( MovingZoom );
		bViewLock = true;
		return true;
	case eScrollUp:
		BaseZoom = Trim( BaseZoom+(MaxScale-MinScale)/5, MaxScale, MinScale );
		Display.SetZoom( BaseZoom );
		return true;
	case eScrollDown:
		BaseZoom = Trim( BaseZoom-(MaxScale-MinScale)/5, MaxScale, MinScale );
		Display.SetZoom( BaseZoom );
		return true;
	}
	return Land.MouseJob( Display, State );
}

// 仕事
void AllWorld::Job()
{
	RbTimer NewTime;
	double Passed = NewTime.Passed( OldTime );
	if( Passed < JobTime() )return;
	OldTime = NewTime;

	if( Passed > 1000 )Passed = JobTime();
	MainChara.ActTime += Passed;

	RbPosition OldPos = MainChara.Pos;
	if( MainChara.NextMove.size() > 0 )
		if( !MoveMainChara( *(MainChara.NextMove.begin()), Passed ) )
	{
		MainChara.NowHouse = *(MainChara.NextMove.begin());
		GetGameData().SetHousePos( MainChara.NowHouse );
		MainChara.NextMove.pop_front();//( itr );
		LastRnd = Rnd(99);
		if( MainChara.NextMove.size() <= 0 )
		{
			GetRbDisplay().SetZoom( BaseZoom );
			LastRnd /= 9;
		}
	//	GetEventSystem().Job();
	}
	GetEventSystem().Job();
	if( bViewLock )GetRbDisplay().ScrollView( MainChara.Pos.x - OldPos.x, MainChara.Pos.y - OldPos.y );
}
