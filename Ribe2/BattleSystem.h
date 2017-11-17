// BattleSystem.h: BattleSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__D9E940C2_C730_435E_A703_38441661D91D__INCLUDED_)
#define AFX_BATTLESYSTEM_H__D9E940C2_C730_435E_A703_38441661D91D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LandEdit.h"
#include "TobiSystem.h"
#include "RbPolyPoly.h"
#include "BattleSystemStatus.h"
#include "ArmyFormation.h"
#include "BattleTagetSystem.h"

class XMLPaser;
class Display;
class RbDisplay;
class RbSprite;

class BattleSystem  : public BattleTagetSystem
{
public:
	BattleSystem();
	virtual ~BattleSystem();

	virtual void ClearData();
	bool ChangeGameMode( GameMode NewMode );
	virtual HRESULT InitSystem( Display& Disp );
	virtual bool LoadSystemXML( const XMLPaser& SystemXML );
	virtual RbBattlePaty* AddPaty( const XMLPaser& PatyXML );
	RbBattlePaty* AddPaty( RbBattlePaty* pPaty ){return BattleTagetSystem::AddPaty(pPaty);}
	long EscapeArmy( long ArmyID );

	bool KeyJob( int KeyCode, bool bPress );
	bool MouseJob( RbDisplay& Disp, GameMessage& State );
	virtual HRESULT Job();
	virtual HRESULT DrawMain( Display& Disp, const GameMessage& MouseState );
	RbActStatus* GetPosCharactor( Display& Disp, int x, int y ) const;
	virtual bool SetSelectCharactor( RbActStatus* pCharactor );

	RbPosition GetRndPosition( long ArmyID, float Size ) const;
	RbPosition GetRndPosition(  float Size ) const;
	D3DXVECTOR3 GetClickPos( Display& DisplaySystem, int MouseX, int MouseY ) const;
	D3DXVECTOR3 GetFirstPos() const;
	DWORD JobTime() const{ return 1000/50;	/* 50 (frame/sec) */ }
	float JobSec() const{return JobTime()/1000.0f;}
	const double& GetBattleTime() const{return BattleTime;}
	double Passed( double OldTime ) const{return (BattleTime-OldTime);}

	bool SetViewOn( long BattleID ) const;
	bool SetViewOn( LPCTSTR Name ) const;
	bool SetViewOn( RbBattleStatus* pStatus ) const;
	bool LoackView( LPCTSTR Name, double Life=500 ) const;
	bool LoackView( RbBattleStatus* pStatus, double Life=500 ) const;
	RefBord* GetShadowBord();
	LockCounter BattleLock;

public:
	TobiSystem TobiSys;
	TobiPlanes TobiPlaneSys;
	LandEdit Land;
	BattleSystemStatus BStatus;
	ArmyFormation Formation;
protected:
	void DrawHpBar( Display& Disp, RbSprite& Surface ) const;
	void DeleteDead();
	double OldTime;
	double BattleTime;
	long OverCount;
	RefPtr<RbPolyPoly> pCursor;
	RefPtr<RefBord> pShadowBill;

	bool IsBattleLock() const{return BattleLock.IsLock();}
public:
	static float BatIn;
	static float BatKeep;
	static float FormSize;
	static float EscapeRange;
	static float EscapeRun;
	static float MinScale;
	static float MaxScale;
	static float ComboRate;
	static float SoulRate;
};

BattleSystem& GetBattleSystem();

#endif // !defined(AFX_BATTLESYSTEM_H__D9E940C2_C730_435E_A703_38441661D91D__INCLUDED_)
