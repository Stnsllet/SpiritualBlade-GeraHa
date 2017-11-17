// TrapSystem.h: TrapSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAPSYSTEM_H__E2D1EC2A_619C_47EE_B48B_92233390BFCA__INCLUDED_)
#define AFX_TRAPSYSTEM_H__E2D1EC2A_619C_47EE_B48B_92233390BFCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class String;
class RbActStatus;
class RbBattlePaty;
class GameSystem;

class TrapSystem  
{
public:
	TrapSystem( GameSystem& RefSystem );
	~TrapSystem();

	bool Trap( RbActStatus* pStatus, const String& Traps, const String& MonsterPath );
	bool Trap( RbBattlePaty* pPaty, const String& Traps, const String& MonsterPath );

	static float DmgRate;
	static float MindRate;
	static float BraveRate;
private:
	bool DamegeTrap( RbBattlePaty* pPaty );
	bool MindTrap( RbBattlePaty* pPaty );
	bool DownTrap( RbBattlePaty* pPaty );
	bool MonsterTrap( const String& MonsterPath );
	bool AntiTrap( RbBattlePaty* pPaty );

	GameSystem& System;
};

#endif // !defined(AFX_TRAPSYSTEM_H__E2D1EC2A_619C_47EE_B48B_92233390BFCA__INCLUDED_)
