// TobiHealMist.cpp: TobiHealMist �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiHealMist.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiHealMist::TobiHealMist()
{
	SpreadSpeed = 100 * System.JobSec();
	EffectCount = 1;
	EffectInterval = 1000;
	NowEffectTime = 0;
}

bool TobiHealMist::SetResource()
{
	if( !TobiSpreadBase::SetResource() )return false;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("mist"));
	SetPolygon( MyPolyID );

	return true;
}

TobiHealMist::~TobiHealMist()
{
}

// �X�L�����ʐݒ�
bool TobiHealMist::SetEffect( const SkillEffect& Effect )
{
	if( !TobiSpreadBase::SetEffect( Effect ) )return false;
	EffectCount = Effect.Count;
	return true;
}

// �f�[�^�Z�b�g
bool TobiHealMist::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSpreadBase::SetTagetPos(pSource,BasePos,NewTagetPos) )return false;

	Locker.Lock( pSource->ActLock );
	Life = EffectCount*EffectInterval/System.JobTime();
	return true;
}

// �d��
bool TobiHealMist::Job( BattleSystem& System )
{
	if( pMember == NULL || pMember->IsDead() )return false;
	NowEffectTime += System.JobTime();
	Pos = pMember->GetPosition();

	if( Life > 1 ) SpreadRange += SpreadSpeed;
	else SpreadRange = SpreadMaxRange;
	if( SpreadRange > SpreadMaxRange)SpreadRange = SpreadMaxRange;

	if( NowEffectTime >= EffectInterval )
	{
		NowEffectTime = 0;
		Damege( System );
	}
	if( (--Life) > 0 )return true;

	return false;
}

// �_���[�W
void TobiHealMist::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), false, false );
	if( Count == 0 )return;

	BattleTagetSystem::CharaList::const_iterator itr = TagetList.begin();
	while( itr != TagetList.end() )
	{
		DamegeSystem DSys( *pMember, *(*itr++) );
		DSys.Heal( AttackPow );
	}
}
