// TobiBurstBase.cpp: TobiBurstBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiBurstBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiBurstBase::TobiBurstBase()
{
	SpreadRange = 32;
}

TobiBurstBase::~TobiBurstBase()
{
}
/*
// �f�[�^�Z�b�g
bool TobiBurstBase::SetTaget( RbActStatus* pSource, RbActStatus* pTaget )
{
	if( pTaget == NULL )return false;
	if( pSource == NULL )return false;

	const RbPosition& Pos = pSource->GetPosition();
	const RbPosition& EnePos = pTaget->GetPosition();

	const float& AttackRange = pSource->AttackAlgo.NowWepon().AttackRange;
	float CheckRange = AttackRange - Pos.Length( EnePos );
	if( Pos.z > EnePos.z )CheckRange += Pos.z - EnePos.z;

	if( CheckRange > SpreadRange+3 )CheckRange = SpreadRange+3;
	else if( CheckRange < 0 )CheckRange = 0;

	RbPosition TagetPos
		= System.CircleTaget( EnePos,
		SpreadRange, CheckRange, pSource->GetArmyID(), pSource->GetAngle() );
	return SetTagetPos( pSource, Pos, TagetPos );
}
*/

bool TobiBurstBase::SetEffect( const SkillEffect& Effect )
{
	AttackRange = Effect.Range;
	return TobiMagicBase::SetEffect( Effect );
}

bool TobiBurstBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;

	const RbPosition& Pos = BasePos;
	const RbPosition& EnePos = NewTagetPos;

	if( AttackRange <= 0 )AttackRange = pSource->AttackAlgo.NowWepon().AttackRange;
	float CheckRange = AttackRange - Pos.Length( EnePos );
	if( Pos.z > EnePos.z )CheckRange += Pos.z - EnePos.z;

	if( CheckRange > SpreadRange )CheckRange = SpreadRange;
	else if( CheckRange < 0 )CheckRange = 0;

	RbPosition TagetPos
		= System.CircleTaget( EnePos,
		SpreadRange, CheckRange, pSource->GetArmyID(), pSource->GetAngle() );
	return TobiMagicBase::SetTagetPos( pSource, Pos, TagetPos );
}

// �_���[�W����Ȃ��ăI�u�W�F�N�g�����
void TobiBurstBase::Damege( BattleSystem& System )
{
	RefPtr<TobiObjectBase> pBurst = GetBurstObject();
	if( pBurst != NULL ) System.TobiSys.Add( pBurst );
	return;
}

// �I�u�W�F�N�g�擾
RefPtr<TobiObjectBase> TobiBurstBase::GetBurstObject()
{
	return NULL;
}
