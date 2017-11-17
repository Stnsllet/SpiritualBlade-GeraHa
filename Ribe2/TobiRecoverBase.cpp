// TobiRecoverBase.cpp: TobiRecoverBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiRecoverBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiRecoverBase::TobiRecoverBase()
{
}

TobiRecoverBase::~TobiRecoverBase()
{
}

// �X�L�����ʐݒ�
bool TobiRecoverBase::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	return true;
}

// �d��
bool TobiRecoverBase::Job( BattleSystem& System )
{
	Damege( System );

	return false;
}

// �f�[�^�Z�b�g
bool TobiRecoverBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Pos = BasePos;
	TagetPos = NewTagetPos;

	return true;
}

void TobiRecoverBase::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, MyEffect.Range, pMember->GetArmyID(), false );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
	//	if( !IsNewHit(&Ref) )continue;
		Heal( *(*itr++) );
	}
}

bool TobiRecoverBase::Heal( RbActStatus& Taget)
{
	return true;
}
