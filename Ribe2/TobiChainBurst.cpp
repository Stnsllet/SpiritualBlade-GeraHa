// TobiChainBurst.cpp: TobiChainBurst �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiBurst.h"
#include "TobiChainBurst.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiChainBurst::TobiChainBurst()
{
	GenerateTime = GenerateInterval = 200;
	MemberNo = 0;
}

TobiChainBurst::~TobiChainBurst()
{

}

bool TobiChainBurst::Generate()
{
	if( pPaty == NULL )return false;

	RefPtr<RbActStatus> pStatus = pPaty->GetMember(MemberNo++);
	if( pStatus == NULL )return true;
	if( pStatus->IsDead() )return true;

	TobiRefPtr<TobiBurst> pBurst(MyEffect);
	if( !pBurst->SetSource( *pStatus ) )return true;
	System.TobiSys.Add( pBurst );

	if( MemberNo >= pPaty->MemberCount() )return false;
	return true;
}

// �f�[�^�Z�b�g
bool TobiChainBurst::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	pPaty = pSource->GetPaty();
	if( pPaty == NULL )return false;

//	Locker.Lock( pSource->ActLock );
	return true;
}
