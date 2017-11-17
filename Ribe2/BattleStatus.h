
#ifndef __BATTLE_STATUS___
#define __BATTLE_STATUS___
#include "StatusStruct.h"
#include "RbPosition.h"

	struct BattleStatus
	{
		bool bUserData;
		bool bLeader;
		long ArmyID;
		BattleAlgoType BattleType;
		long WeponRandom;

		bool bDelete;
		BattleAction Action;
		BattleAction OldAction;
		int ActTime;
		int HpBarTime;
		float Wait;
		float DamegeWait;
		float EscapeHp;

		RbPosition Pos;
		float Direction;
		float FlyHight;
		long Combo;
		bool bMoreSpecial;
	};

#endif // __BATTLE_STATUS___