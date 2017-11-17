
#ifndef __TOBI_EX_OBJECT_BASE__
#define __TOBI_EX_OBJECT_BASE__

#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "ExCreator.h"

template <class T> class TobiExObjectBase : public T  
{
public:
	TobiExObjectBase():bMagic(false),bAllHit(false),ChiledRange(32){}
	virtual ~TobiExObjectBase(){}
	virtual bool SetEffect( const SkillEffect& Effect )
	{
		ExEffect = Effect;
		return T::SetEffect( Effect );
	}

protected:
	virtual void Damege( BattleSystem& System ){}
	bool CreateChiled( const String& Name )
	{
		return CreateChiled( Name, Pos, TagetPos );
	}
	bool CreateChiled( const String& Name, const RbPosition& Pos, const RbActStatus& Taget )
	{
		RbPosition TagetPos = Taget.GetPosition();
		TagetPos.z += Taget.Hight()/2.0f;
		return CreateChiled( Name, Pos, TagetPos );
	}
	bool CreateChiled( const String& Name, const RbPosition& Pos, const RbPosition& TagetPos )
	{
		if( Name.empty() )return false;
		TobiCreator* pCreator = System.TobiSys.GetCreator( Name );
		if( pCreator == NULL )return false;
		RefPtr<TobiObjectBase> pNew = pCreator->Create();
		if( pNew == NULL )return false;
		SkillEffect Effect = ExEffect;
		Effect.Range = ChiledRange;
		pNew->SetEffect( Effect );
		if( !pNew->SetTagetPos( pMember, Pos, TagetPos ) )return false;
		System.TobiSys.Add( pNew );
		return true;
	}
	SkillEffect ExEffect;
	float ChiledRange;
	bool bMagic;
	bool bAllHit;
	String ChiledName;
};

template <class T> class TobiExObjectBase2 : public TobiExObjectBase<T>  
{
public:
	TobiExObjectBase2(){}
	virtual ~TobiExObjectBase2(){}
protected:
	virtual void Damege( BattleSystem& System )
	{
		if( pMember == NULL )return;
		if( CreateChiled(ChiledName) )return;

		BattleSystem::CharaList List;
		System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID(), true, bAllHit );

		BattleSystem::CharaList::const_iterator itr = List.begin();
		while( itr != List.end() )
		{
			RbActStatus& Ref = *(*itr++);
		//	if( !IsNewHit(&Ref) )continue;
			DamegeSystem DSys( *pMember, Ref );
			if( bMagic )DSys.MagicDamege( AttackPow, KnockBack, DmgWait );
			else DSys.AttackDamege( AttackPow, KnockBack, DmgWait );
		}
	}
};

template <class T> class TobiExObjectBase3 : public TobiExObjectBase<T>  
{
public:
	TobiExObjectBase3(){}
	virtual ~TobiExObjectBase3(){}
protected:
	virtual void Damege( BattleSystem& System )
	{
		if( pMember == NULL )return;

		BattleSystem::CharaList List;
		System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID(), true, bAllHit );
			
		BattleSystem::CharaList::const_iterator itr = List.begin();
		while( itr != List.end() )
		{
			RbActStatus& Ref = *(*itr++);
			if( !IsNewHit(&Ref) )continue;
			if( CreateChiled(ChiledName,Pos,Ref) )continue;
			DamegeSystem DSys( *pMember, Ref );
			if( bMagic )DSys.MagicDamege( AttackPow, KnockBack, DmgWait );
			else DSys.AttackDamege( AttackPow, KnockBack, DmgWait );
		}
	}
};

#endif __TOBI_EX_OBJECT_BASE__
