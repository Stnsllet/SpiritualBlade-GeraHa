// AllWorld.h: AllWorld クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLWORLD_H__71707999_CD30_4D09_A063_2AFB93C01DC6__INCLUDED_)
#define AFX_ALLWORLD_H__71707999_CD30_4D09_A063_2AFB93C01DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include "RbRect.h"
#include "DefaultPlanes.h"
#include "StringIDMap.h"
#include "RbTexture.h"
#include "LandScape.h"
#include "BillbordPlane.h"

class RbDisplay;
class RbSprite;
class XMLPaser;

class AllWorld
{
public:
	AllWorld();
	virtual ~AllWorld();
	bool MouseJob( RbDisplay& Display, GameMessage& State );
	void Job();

	bool SetWorldSystem( const XMLPaser& WorldXML );
	HRESULT InitWorld( RbDisplay& Display );
	HRESULT Draw( RbDisplay& Display );
	void ClearData();
	bool ChangeGameMode( GameMode NewMode );
	bool SetBaseZoom( float NewZoom );

	bool RndCheck( int Check ) const;
	bool CheckHouse( const String& House );
	bool AddLine( LPCTSTR House1, LPCTSTR House2 );
	bool SetPosition( LPCTSTR HouseName );
	bool MoveStart( LPCTSTR HouseName );
	bool StopMove();
	bool OutRandom();
	bool ChangeMainChara( LPCTSTR Name );
	bool IsMoveStop() const{return ( MainChara.NextMove.size() <= 0 );}

	LandScape Land;
	double JobTime() const{return 20;}
protected:
	String LandPath;
	HandPlanes HousePlaneList;
	struct HouseData
	{
		String NameID, Type;
		String Name, Help;
		RbPosition Pos;
		bool bView;
		RbRect DispRect;
		BillbordPlane BordPlane;
	};
	struct LineData
	{
		String House1, House2;
		RbPosition Pos1, Pos2;
	};
	struct CharaData
	{
		String MainName;
		String NowHouse;
		RbPosition Pos;
		int ActTime;
		float Direction;
		RefPtr<RbTexture> pTexture;
		float DispScale;
		std::list<String> NextMove;
	}MainChara;
	typedef std::map<String, HouseData> HouseDataMap;
	HouseDataMap HouseDataList;
	std::list<LineData> LineList;
	double OldTime;
	String BaseMainChara;
	float MoveSpeed;
	float MovingZoom;
	float BaseZoom;
	bool bViewLock;
	int LastRnd;
	float MinScale, MaxScale;

protected:
	void GetLinkMap( std::map<String,String>& Output, const String& StartHouse, const String& TagetHouse ) const;
	void GetLinkList( std::list<String>& Output, const String& BaseName ) const;
	bool MoveMainChara( LPCTSTR HouseName, double Passed );

	HRESULT LoadAllHouse( LPCTSTR FileName );
	HRESULT ReadHouse( const XMLPaser& HouseXML );
	HRESULT DrawMainChara( RbDisplay& Display, RbSprite& Surface );
	HRESULT DrawHouse( HouseData& Data, RbDisplay& Display ) const;
	HRESULT DrawLine( RbDisplay& Display ) const;
	HRESULT DrawLine( const BillbordPlane& LinePlane, const RbPosition& Pos1, const RbPosition& Pos2, const RbDisplay& Display ) const;

	HouseData GetHouseData( int MouseX, int MouseY ) const;
};

AllWorld& GetWorldSystem();

#endif // !defined(AFX_ALLWORLD_H__71707999_CD30_4D09_A063_2AFB93C01DC6__INCLUDED_)
