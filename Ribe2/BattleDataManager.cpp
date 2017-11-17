// BattleDataManager.cpp: BattleDataManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "XMLList.h"
#include "RbWindowSystem.h"
#include "DispStocker.h"
#include "BattleDataManager.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BattleDataManager::BattleDataManager()
{
}

BattleDataManager::~BattleDataManager()
{
}

bool BattleDataManager::SetSelectCharactor( RbActStatus* pCharactor )
{
	if( pCharactor == NULL )return false;
	pSelectCharactor = pCharactor;
	return true;
}

// �f�[�^�N���A
void BattleDataManager::ClearData()
{
	pSelectCharactor = NULL;
	BattleArmys.clear();
	DeleteUnUsePaty( true );
	ATLASSERT( BattlePatys.size() <= 0 );
	BattleCharactors.clear();
}

long BattleDataManager::DeleteUnUsePaty( bool bAll )
{
	long Count = 0;
	BPatyMap::iterator itr = BattlePatys.begin();
	while( itr != BattlePatys.end() )
	{
		RbBattlePaty& Ref = *(itr->second);
		if( bAll || Ref.IsDeleteOK() )
		{
			Ref.DropJob();
			itr = BattlePatys.erase(itr);
			Count++;
		}
		else itr++;
	}
	return Count;
}

// ���������s��
HRESULT BattleDataManager::InitSystem( Display& Disp )
{
	ClearData();

	return S_OK;
}

// �R���擾
ArmyStatus& BattleDataManager::ArmyData( const String& Name )
{
	BArmyMap::iterator itr = BattleArmys.find(Name);
	if( itr != BattleArmys.end() )return itr->second;

	ArmyStatus& Army = BattleArmys[Name];
	Army.SetArmyName( Name );
	return Army;
}

// ���Ô�v�Z
long BattleDataManager::GetMedical( long ArmyID ) const
{
	long Medical = 0;
	BArmyMap::const_iterator itr = BattleArmys.begin();
	while( itr != BattleArmys.end() )
	{
		const ArmyStatus& Ref = (itr++)->second;
		if( Ref.RefArmyID() != ArmyID )continue;
		Medical += Ref.GetMedical();
	}
	return Medical;
}

// �`��
HRESULT BattleDataManager::DrawMain( Display& Disp )
{
	SortCharactorList();
	DispStocker::AllTrans Trans( Disp );

	DispStocker::Render StockBias( Disp, D3DRS_ZBIAS );
	DispStocker::Material MaterialStock( Disp );
	MaterialStock.Set( Disp.RefPlaneMaterial() );

	BCharaMap::reverse_iterator itr = BattleCharactors.rbegin();
	while( itr != BattleCharactors.rend() )
	{
		(*itr)->Draw( Disp, Trans.All );
		itr++;
	}

	return S_OK;
}

// �\�[�g����
void BattleDataManager::SortCharactorList()
{
	BCharaMap::iterator Nextitr = BattleCharactors.begin();
	do
	{
		Nextitr = SortCharactor( Nextitr, BattleCharactors.end() );
	}while( Nextitr != BattleCharactors.end() );
}

// �\�[�g����
BattleDataManager::BCharaMap::iterator BattleDataManager::SortCharactor(BCharaMap::iterator First, BCharaMap::iterator Last)
{
	if( First == Last )return Last;
	BCharaMap::iterator itr = First;
	++itr;
	BCharaMap::iterator Next = itr;

	const RbPosition& FirstPos = (*First)->GetPosition();
	while( itr != Last
		&& FirstPos.y < (*itr)->GetPosition().y )itr++;

	if( Next == itr )return Next;

	itr = SortCharactor( Next, itr );
	itr = BattleCharactors.insert( itr, *First );
	BattleCharactors.erase(First);
	return ++itr;
}

// �p�[�e�B�ǉ�
RbBattlePaty* BattleDataManager::AddPaty( const XMLPaser& PatyXML )
{
	RefPtr<RbBattlePaty> pPaty( new RbBattlePaty );
	if( FAILED(pPaty->SetPaty( PatyXML )) )return NULL;

	return AddPaty( pPaty );
}

// �p�[�e�B�ǉ�
RbBattlePaty* BattleDataManager::AddPaty( const PatyStatus& Paty, float x, float y, float angle )
{
	RefPtr<RbBattlePaty> pPaty( new RbBattlePaty );
	if( FAILED(pPaty->SetPaty( Paty, x, y, angle )) )return NULL;

	return AddPaty( pPaty );
}

// �p�[�e�B�ǉ�
RbBattlePaty* BattleDataManager::AddPaty( RbBattlePaty* pPaty )
{
	if( pPaty == NULL )return NULL;

	BattlePatys[ pPaty->ID() ] = pPaty;
	if( pPaty->IsUserData() )GetWindowSystem().AddPatyHp( pPaty->ID() );
	return pPaty;
}

// �L�����ǉ�
RbActStatus* BattleDataManager::AddCharactor( RbBattlePaty* pPaty, const MemberStatus& Member )
{
	RefPtr<RbActStatus> pChara( new RbActStatus );
	pChara->SetData( pPaty, Member );

	BCharaMap::iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( pChara->GetPosition().y >= (*itr)->GetPosition().y )break;
		itr++;
	}
	BattleCharactors.insert( itr, pChara );

	if( pSelectCharactor == NULL && pChara->IsUserData() )
		SetSelectCharactor( pChara );
	return pChara;
}

// �R�c�ԍ��擾
long BattleDataManager::GetArmyID( LPCTSTR name ) const
{
	return ArmyStatus::GetArmyID( name );
}

//�p�[�e�B�[���擾
RbBattlePaty* BattleDataManager::GetPaty( long PatyID ) const
{
	BPatyMap::const_iterator itr = BattlePatys.find( PatyID );
	if( itr == BattlePatys.end() )return NULL;
	return itr->second;
}

// �ŏ��Ɍ��������L�����N�^���擾
RbActStatus* BattleDataManager::GetCharactor( LPCTSTR name ) const
{
	if( name == NULL )return NULL;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == name )return &Ref;
	}
	return NULL;
}

// �������O�̃L�����N�^���擾
long BattleDataManager::GetCharactors( CharaList& Output, LPCTSTR name ) const
{
	if( name == NULL )return NULL;
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == name )
		{
			Count++;
			Output.push_back( &Ref );
		}
	}
	return Count;
}

// �s���̃|�C���^����o�g��ID�擾
long BattleDataManager::GetBattleID( RbStatus* pStatus ) const
{
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( pStatus == (RbStatus*)(*itr) )return (*itr)->ID();
		itr++;
	}
	return 0;
}

// �L�����N�^���擾
RbActStatus* BattleDataManager::GetCharactor( long MemberID ) const
{
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		if( MemberID == (*itr)->ID() )return (*itr);
		itr++;
	}
	return NULL;
}

// ���O�ŃR�[���o�b�N
long BattleDataManager::NameCall( MemCallBack* pCallBack, LPCTSTR Name ) const
{
	if( pCallBack == NULL )return 0;
	long Count = 0;
	BCharaMap::const_iterator itr = BattleCharactors.begin();
	while( itr != BattleCharactors.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( Ref.IsDeleteOK() )continue;
		if( Ref.MemberState().PasonName == Name )
			if( pCallBack->Call( Ref ) )Count++;
	}
	return Count;
}
