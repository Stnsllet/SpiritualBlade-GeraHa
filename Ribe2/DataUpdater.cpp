// DataUpdater.cpp: DataUpdater �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "LevelUpSystem.h"
#include "DataUpdater.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DataUpdater::DataUpdater()
: UserData( GetGameData() )
{
	AllZero( Mem );
}

DataUpdater::DataUpdater( const String& PasonName )
: UserData( GetGameData() )
{
	AllZero( Mem );
	SetData( PasonName );
}

DataUpdater::~DataUpdater()
{
	if( !Mem.PasonName.IsNone() )UserData.UpDateMemberStatus( Mem );
}

bool DataUpdater::SetData( const String& PasonName )
{
	return UserData.GetMember( PasonName, Mem );
}

bool DataUpdater::ClassChange( const String& NewClass )
{
	if( Mem.PasonName.IsNone() )return false;
	return LevelUpSystem(Mem).ClassChange( NewClass );
}

bool DataUpdater::AddSkill( const String& Skill, long Level )
{
	if( Mem.PasonName.IsNone() )return false;
	return LevelUpSystem(Mem).AddSkill( Skill,Level );
}
