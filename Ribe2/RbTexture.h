// RbTexture.h: RbTexture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBTEXTURE_H__C5B0A5B8_D1C4_40F1_8E61_DD1671414082__INCLUDED_)
#define AFX_RBTEXTURE_H__C5B0A5B8_D1C4_40F1_8E61_DD1671414082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include "StringIDMap.h"
#include "StatusStruct.h"
#include "Plane.h"
#include "RefObject.h"
#include "BillbordPlane.h"
#include "DefaultPlanes.h"

class RbPosition;
class Texture;
class XMLPaser;
class XMLList;

class RbTexture : public RefAuto
{
public:
	struct TexturePlane
	{
		TexturePlane(){AllZero(*this);}
		Plane<HandTexRect> Rect;
		bool bItem;
		RefBord* GetRefBord() const;
		RefPtr<RefBord> pRefBord;
		operator MYTEXTURE*()const{return Rect;}
		RECT BoundRect() const;
		bool Reverse();
	};
	struct ActTime
	{
		ActTime(){Min=Max=0;}
		int Min, Max;
		std::vector<TexturePlane> PlaneList;
		RECT BoundRect() const;
		bool InTime( int Time ) const;
		bool Reverse();
	};
	struct AttackInfo
	{
		int MaxTime;
		float AttackStart;
		float AttackEnd;
		bool bSlash;
	};
protected:
	struct Direction
	{
		Direction(){MinDir=MaxDir=0;AllZero(AtcInfo);}
		float MinDir, MaxDir;
		std::vector<ActTime> TimeList;
		AttackInfo AtcInfo;
		RECT BoundRect() const;
		bool InDirection( const float& Dir ) const;
		bool Reverse();
	};
	struct Action
	{
		Action(){ActNumber=0;}
		long ActNumber;
		String Name;
		String Sound;
		long WaveID;
		std::vector<Direction> DirList;
	};
	std::map<long,Action> ActionList;
	HandPlanes NamedPlane;

	float Size;
	float Hight;
	String MyName;
	long BattleID[eActMax];
public:
	RbTexture();
	virtual ~RbTexture();
	float GetSize() const{return Size;}
	float GetHight() const{return Hight;}

	operator LPCTSTR() const{return MyName;}

	bool IsRegister( LPCTSTR ActName ) const;
	bool IsRegister( long ActID ) const;
	long GetActID( BattleAction ActID ) const;
	bool Sound( BattleAction ActID, const RbPosition& Pos );
	bool Sound( long ActID, const RbPosition& Pos );
	int GetBattleActionTime( BattleAction ActID, float Direct ) const;
	int GetActionTime( LPCTSTR ActName, float Direct ) const;
	int GetActionTime( long ActID, float Direct ) const;
	AttackInfo GetActionInfo( long ActID, float Direct ) const;

	const ActTime& GetAction( LPCTSTR ActName, float Direct, DWORD Time ) const;
	const ActTime& GetAction( long ActID, float Direct, DWORD Time ) const;
	const ActTime& GetBattleAction( BattleAction ActID, float Direct, DWORD Time ) const;

	HRESULT LoadRbTexture( DISPDEVICE* pDevice, LPCTSTR NameID );
	bool SetBattleID( BattleAction ActID, LPCTSTR ActName );

protected:
	virtual HRESULT LoadActionTexture( DISPDEVICE* pDevice, LPCTSTR FileName );
	const Direction* GetDirection( long ActID, float Direct ) const;
	HRESULT StockTexture( DISPDEVICE* pDevice, const XMLList& FileList );
	HRESULT CreateSweep( const ActTime& Start,  const ActTime& End, Direction& Output, long Sweep ) const;

	HRESULT ReadActionTexture( DISPDEVICE* pDevice, XMLPaser& File );
	HRESULT ReadAction( const XMLPaser& ActionXML );
	HRESULT ReadDirection( const XMLPaser& DirXML, Action& Output ) const;
	HRESULT ReadTimeData( const XMLPaser& TimeXML, Direction& Output ) const;
	HRESULT ReadPlaneData( const XMLPaser& PlaneXML, ActTime& Output ) const;

	ActTime Dummy;
};

#endif // !defined(AFX_RBTEXTURE_H__C5B0A5B8_D1C4_40F1_8E61_DD1671414082__INCLUDED_)
