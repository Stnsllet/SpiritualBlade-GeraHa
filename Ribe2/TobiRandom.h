// TobiRandom.h: TobiRandom クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIRANDOM_H__D0582249_EE66_4632_A646_B01FEC7D7B1E__INCLUDED_)
#define AFX_TOBIRANDOM_H__D0582249_EE66_4632_A646_B01FEC7D7B1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiObjectBase.h"
#include <vector>
class TobiCreator;

class TobiRandom : public TobiObjectBase  
{
public:
	TobiRandom();
	virtual ~TobiRandom();
	virtual bool Job( BattleSystem& System );

	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	struct RndList
	{
		bool Init( BattleSystem& System );
		TobiObjectBase* Create() const;
		std::vector<TobiCreator*> Default;
		std::vector<TobiCreator*> Rare;
	};
	static RndList CreateList;
	SkillEffect MyEffect;
};

#endif // !defined(AFX_TOBIRANDOM_H__D0582249_EE66_4632_A646_B01FEC7D7B1E__INCLUDED_)
