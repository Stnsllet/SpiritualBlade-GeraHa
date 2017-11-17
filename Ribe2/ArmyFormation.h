// ArmyFormation.h: ArmyFormation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMYFORMATION_H__1987C38D_E6F1_469E_BFDB_63798BECCC6E__INCLUDED_)
#define AFX_ARMYFORMATION_H__1987C38D_E6F1_469E_BFDB_63798BECCC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include "String.h"
class BattleSystem;
class XMLPaser;
class RbBattlePaty;

class ArmyFormation  
{
public:
	ArmyFormation( BattleSystem& RefSystem );
	virtual ~ArmyFormation();
	HRESULT LoadFile();
	HRESULT LoadFile( const String& FileName );

	RbBattlePaty* AddPatys( const String& FormationName, const XMLPaser& PatyXML ) const;

	struct PatyForm
	{
		bool SetXML( const XMLPaser& ReadXML );
		String Name;
		struct MemPos
		{
			float x, y;
		}Pos[8];
	};

	struct PatyPos
	{
		bool SetXML( const XMLPaser& ReadXML );
		long PatyID;
		float x, y;
		float Angle;
	};
	struct ArmyPos
	{
		bool SetXML( const XMLPaser& ReadXML );
		String FormationName;
		std::list<PatyPos> PatyList;
	};
	const PatyForm* GetPatyForm( const String& Name ) const;
protected:
	RbBattlePaty* AddPatys( BattleSystem& System, const ArmyPos& Form, const XMLPaser& PatyXML ) const;

	std::map<String,ArmyPos> ArmyPosList;
	std::map<String,PatyForm> PatyFormList;
	BattleSystem& System;
};

#endif // !defined(AFX_ARMYFORMATION_H__1987C38D_E6F1_469E_BFDB_63798BECCC6E__INCLUDED_)
