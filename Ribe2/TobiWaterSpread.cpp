// TobiWaterSpread.cpp: TobiWaterSpread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiWaterSpread.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiWaterSpread::TobiWaterSpread()
{
	SpreadSpeed = 100 * System.JobSec();
}

bool TobiWaterSpread::SetResource()
{
	if( !TobiSpreadBase::SetResource() )return false;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("water"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWaterSpread::~TobiWaterSpread()
{

}

// �m�b�N�o�b�N
RbPosition TobiWaterSpread::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	RbPosition RetKnockBack = TobiSpreadBase::GetKnockBack( Base, Taget );
	RetKnockBack += KnockBack;
	if( Taget.GetArmyID() == pMember->GetArmyID() )return RetKnockBack.Normalize(2);
	return RetKnockBack.Normalize(8);
}

// ���ݒ�
void TobiWaterSpread::SetInfo( const RbPosition& AddKnockBack, float NewSpreadRange, float NewSpreadKnockBack )
{
	TobiSpreadBase::SetBaseInfo( NewSpreadRange, NewSpreadKnockBack );

	KnockBack = AddKnockBack;
	Vector = KnockBack;
	Vector.Normalize( SpreadSpeed );
}

// �f�[�^�Z�b�g
bool TobiWaterSpread::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSpreadBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;

	SpreadRange = SpreadMaxRange - Pos.Length(TagetPos);
	Life = (int)((SpreadMaxRange-SpreadRange)/SpreadSpeed);
	Life += 1;

	return true;
}

// �d��
bool TobiWaterSpread::Job( BattleSystem& System )
{
	if( Life > 1 ) Pos += Vector;
	else Pos = TagetPos;

	return TobiSpreadBase::Job( System );
}
