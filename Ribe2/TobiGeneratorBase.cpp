// TobiGeneratorBase.cpp: TobiGeneratorBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiGeneratorBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiGeneratorBase::TobiGeneratorBase()
{
	GenerateTime = 0;
	GenerateInterval = 0;
	bActLock = true;
}

TobiGeneratorBase::~TobiGeneratorBase()
{
}

// �X�L�����ʐݒ�
bool TobiGeneratorBase::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	if( MyEffect.Count <= 1 )bActLock = false;
	return TobiObjectBase::SetEffect( Effect );
}

// �����_���^�[�Q�b�g���w�肵�Ēǉ�
bool TobiGeneratorBase::AddRandomTaget( TobiObjectBase* pBit, float Range )
{
	if( pBit == NULL )return false;
	if( pMember == NULL )return false;
	RefPtr<RbActStatus> pTaget
		= System.GetRndEnemy( Pos, pMember->GetArmyID(), Range );
	if( pTaget == NULL )return TobiGeneratorBase::Generate();
	if( !pBit->SetTagetPos(pMember, Pos, pTaget->GetPosition()) )return false;

	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}

// �d��
bool TobiGeneratorBase::Job( BattleSystem& System )
{
	if( pMember->IsDead() )return false;

	GenerateTime += System.JobTime();
	if( GenerateTime >= GenerateInterval )
	{
		GenerateTime -= GenerateInterval;
		return Generate();
	}
	return true;
}

// �f�[�^�Z�b�g
bool TobiGeneratorBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Pos = BasePos;
	TagetPos = NewTagetPos;
	if( bActLock )Locker.Lock( pSource->ActLock );
	return true;
}

bool TobiGeneratorBase::Generate()
{
	return ((--MyEffect.Count)>0);
}