// TobiSystem.h: TobiSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISYSTEM_H__5BE1A0F3_B48B_4B42_A69F_917D3F96A4D7__INCLUDED_)
#define AFX_TOBISYSTEM_H__5BE1A0F3_B48B_4B42_A69F_917D3F96A4D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include "TobiObjectBase.h"
#include "TobiCreator.h"
#include "TobiPlanes.h"
#include "TobiDamege.h"
#include "AttackObjectDefine.h"

class RbSprite;
class RbActStatus;

class TobiSystem
{
public:
	TobiSystem();
	virtual ~TobiSystem();
	void Init();
	void Job( BattleSystem& System );
	void Draw( Display& Disp );
	void Add( TobiObjectBase* pAddObj );
	void ClearData();

	TobiObjectBase* CreateObject( const String& Name ) const;
	TobiCreator* GetCreator( const String& Name ) const;

	void AddErase( RbActStatus& State );
	void AddHealLight( RbActStatus& State, long Count=5 );
	bool AddSelfBomb( RbActStatus& State, const SkillEffect& Effect );

	TobiPlanes Planes;
	TobiDamege Damege;
protected:
	AttackObjectDefine AtcObjDef;
	typedef std::list<RefPtr<TobiObjectBase> > TobiDataList;
	TobiDataList TobiList;
	std::map<String,MasterPtr<TobiCreator> > TobiCreatorList;

	template <class T> class CreateAdder
	{
		public:
		CreateAdder( std::map<String,MasterPtr<TobiCreator> >& List, AttackObjectDefine& RefDef,LPCTSTR Str );
		AttackObjectDefine& Def;
		std::map<String,MasterPtr<TobiCreator> >& Ref;
		String Name;
	};
};

#endif // !defined(AFX_TOBISYSTEM_H__5BE1A0F3_B48B_4B42_A69F_917D3F96A4D7__INCLUDED_)
