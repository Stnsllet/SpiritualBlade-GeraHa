// EquipSkill.cpp: EquipSkill �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "RbCache.h"
#include "EquipSkill.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

EquipSkill::EquipSkill()
: Cache( GetCache().Skill )
{
}

EquipSkill::~EquipSkill()
{
}

// �N���A
void EquipSkill::Clear()
{
	Action.clear();
	Pasonal.clear();
	Leader.clear();
}

// �S�X�L����
long EquipSkill::AllSkillCount() const
{
	return (Action.size()+Pasonal.size()+Leader.size() );
}

// �R���X�g���N�^
EquipSkill::SkillDef::SkillDef()
{
	ID = 0;
	Level = 0;
}

// �X�L���擾
EquipSkill::SkillDef EquipSkill::SkillLists::Get( long Index ) const
{
	if( Index < 0 || Index >= size() )return EquipSkill::SkillDef();
	const_iterator itr = begin();
	while( Index-- )itr++;
	return itr->second;
}

// �X�L���擾
EquipSkill::SkillDef EquipSkill::SkillLists::GetSkill( long ID ) const
{
	const_iterator itr = find( ID );
	if( itr == end() )return EquipSkill::SkillDef();
	return itr->second;
}

// ���ʎ擾
SkillEffect EquipSkill::SkillLists::GetEffect( long SkillID ) const
{
	const_iterator itr = find( SkillID );
	if( itr == end() )return SkillEffect();

	return GetCache().Skill.GetEffect( itr->second.ID, itr->second.Level );
}

// ���x���ǉ�
bool EquipSkill::SkillDef::AddLevel( long Lev )
{
	if( Level >= SkillCache::MaxLevel )return false;
	Level += Lev;
	if( Level > SkillCache::MaxLevel ) Level = SkillCache::MaxLevel;
	return true;
}

// ���x���ǉ�
bool EquipSkill::SkillDef::AddLevel( long Lev, long SetID )
{
	ID = SetID;
	return AddLevel( Lev );
}

// �X�L���ǉ�
bool EquipSkill::AddSkill( LPCTSTR NameID, long Level )
{
	return AddSkill( Cache.GetSkillNo(NameID), Level );
}

// �X�L���ǉ�
bool EquipSkill::AddSkill( long SkillID, long Level )
{
	if( SkillID <= 0 )return false;
	if( Level <= 0 )return false;

	const SkillCache::SkillData* pData = Cache.GetSkill( SkillID );
	ATLASSERT( pData != NULL );
	if( pData == NULL )return false;

	if( pData->Type == "a" )return AddActionSkill( pData->NoID, Level );
	else if( pData->Type == "p" )return AddPasonalSkill( pData->NoID, Level );
	else if( pData->Type == "l" )return AddLeaderSkill( pData->NoID, Level );

	ATLASSERT( NULL && "SkillType");
	return false;
}

// �A�N�V�����X�L���ǉ�
bool EquipSkill::AddActionSkill( long ID, long Level )
{
	return Action[ID].AddLevel( Level, ID );
}

// �p�[�\�i���X�L���ǉ�
bool EquipSkill::AddPasonalSkill( long ID, long Level )
{
	return Pasonal[ID].AddLevel( Level, ID );
}

// ���[�_�[�X�L���ǉ�
bool EquipSkill::AddLeaderSkill( long ID, long Level )
{
	return Leader[ID].AddLevel( Level, ID );
}

// �����o�[�̃X�L���ݒ�
bool EquipSkill::SetMemberSkill( const MemberStatus& MemStatus )
{
	Clear();
	RbMember Mem( MemStatus );
	long MemSkillCount = Mem.SkillCount();

	for( long i = 0; i < MemSkillCount; i++ )
	{
		AddSkill( MemStatus.Skill[i].name, MemStatus.Skill[i].Level );
	}
	AddItemSkill( MemStatus.Wepon.Number );
	AddItemSkill( MemStatus.Body.Number );
	AddItemSkill( MemStatus.Acs.Number );

	Effect.SetSkillEffect( *this );
	return true;
}

// �A�C�e���̃X�L���ǉ�
bool EquipSkill::AddItemSkill( short ItemNo )
{
	const ItemCache& Cache = GetCache().Item;
	const ItemStatus& RefItem = Cache.Get( ItemNo );

	for( long i = 0; i < count_of(RefItem.Skill); i++ )
	{
		if( RefItem.Skill[i].name.IsNone() )break;
		AddSkill( RefItem.Skill[i].name, RefItem.Skill[i].Level );
	}
	return (i>0);
}

// �X�L���擾
EquipSkill::SkillDef EquipSkill::Get( long Index ) const
{
	if( Index < 0 )return EquipSkill::SkillDef();
	if( Index < Action.size() )return Action.Get( Index );
	Index -= Action.size();
	if( Index < Pasonal.size() )return Pasonal.Get( Index );
	Index -= Pasonal.size();
	if( Index < Leader.size() )return Leader.Get( Index );

	return EquipSkill::SkillDef();
}
