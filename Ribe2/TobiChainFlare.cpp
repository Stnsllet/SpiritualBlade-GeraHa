// TobiChainFlare.cpp: TobiChainFlare クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiBurnFlare.h"
#include "TobiChainFlare.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiChainFlare::TobiChainFlare()
{
	GenerateTime = GenerateInterval = 200;
	MemberNo = 0;
}

TobiChainFlare::~TobiChainFlare()
{

}

bool TobiChainFlare::Generate()
{
	if( pPaty == NULL )return false;

	RefPtr<RbActStatus> pStatus = pPaty->GetMember(MemberNo++);
	if( pStatus == NULL )return true;
	if( pStatus->IsDead() )return true;

	TobiRefPtr<TobiBurnFlare> pBurst( MyEffect );
	if( !pBurst->SetSource( *pStatus ) )return true;
	System.TobiSys.Add( pBurst );

	if( MemberNo >= pPaty->MemberCount() )return false;
	return true;
}

// データセット
bool TobiChainFlare::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	pPaty = pSource->GetPaty();
	if( pPaty == NULL )return false;

//	Locker.Lock( pSource->ActLock );
	return true;
}
