// TobiObjectBase.cpp: TobiObjectBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbCache.h"
#include "RbSound.h"
#include "DispStocker.h"
#include "TobiObjectBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiObjectBase::TobiObjectBase()
: System( GetBattleSystem() )
{
	AttackPow = 1.0f;
	DmgWait = 500;
	Life = 0;
	bPlanePoly = true;

	NEW_CHECK(this);
}

TobiObjectBase::~TobiObjectBase()
{
}

bool TobiObjectBase::SetResource()
{
	return true;
}

// �|���S���ԍ�
bool TobiObjectBase::SetPolygon( long PolyID )
{
	if( PolyID <= 0 )return false;
	pPoly = GetCache().Poly.Get( PolyID );
	if( pPoly == NULL )return false;

	float Hight = pPoly->Top()-pPoly->Bottom();
	bPlanePoly = (Hight>-0.01f)&&(Hight<0.01f);
	return true;
}

// �|���S���ԍ�
long TobiObjectBase::GetPolyID( LPCTSTR name ) const
{
	return GetCache().Poly.GetID( name );
}

bool TobiObjectBase::PlayWave( long WaveID, const D3DVECTOR& Pos ) const
{
	return SUCCEEDED(GetSoundSystem().Play( WaveID, Pos ));
}

long TobiObjectBase::GetWaveID( LPCTSTR name ) const
{
	return GetSoundSystem().GetID(name);
}

// �d��
bool TobiObjectBase::Job( BattleSystem& System )
{
	if( (--Life) > 0 )return true;
	Damege( System );

	return false;
}

// �X�L�����ʐݒ�
bool TobiObjectBase::SetEffect( const SkillEffect& Effect )
{
	if( Effect.Pow > 0 )AttackPow = Effect.Pow;
	return true;
}

// �f�[�^�Z�b�g
bool TobiObjectBase::SetSource( RbActStatus& Source )
{
	return SetTagetPos( &Source, Source.GetPosition(), Source.GetPosition() );
}

// �f�[�^�Z�b�g
bool TobiObjectBase::SetTaget( RbActStatus* pSource, RbActStatus* pTaget )
{
	if( pTaget == NULL )return false;
	if( pSource == NULL )return false;
	RbPosition BasePos = pSource->GetPosition();
	BasePos.z += pSource->Hight()/2;
	RbPosition TagetPos = pTaget->GetPosition();
	TagetPos.z += pTaget->Hight()/2;
	return SetTagetPos( pSource, BasePos, TagetPos );
}

// �f�[�^�Z�b�g
bool TobiObjectBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	return false;
}

// �т傤����
void TobiObjectBase::Draw( Display& Disp )
{
}

// �т傤����
void TobiObjectBase::DrawPolygon( Display& Disp, const RbTransform& Trans ) const
{
	if( pPoly == NULL )return;
	if( bPlanePoly )
	{
		pPoly->DrawTrans( Disp, Trans );
	}
	else
	{
		// �ʏ�}�e���A��
		DispStocker::Material StockMate( Disp );
		StockMate.Set( Disp.RefMaterial() );
		// Z �o�b�t�@�L����
		DispStocker::Render StockZ( Disp, D3DRS_ZENABLE );
		StockZ.Set( TRUE );
		// ��������
		DispStocker::Render StockCull( Disp, D3DRS_CULLMODE );
		StockCull.Set( D3DCULL_CCW );
		// �����̓X�P�[�����O���Ȃ�
		DispStocker::Render Normal( Disp, D3DRS_NORMALIZENORMALS );
		Normal.Set( TRUE );
		pPoly->DrawTrans( Disp, Trans );
	}
}

// �_���[�W
void TobiObjectBase::Damege( BattleSystem& System )
{
}

// �V�����q�b�g�����ׂ�
bool TobiObjectBase::IsNewHit( RbActStatus* pTaget )
{
	if( pTaget == NULL )return false;

	long ID = pTaget->ID();
	if( HitList.find( ID ) != HitList.end() )return false;

	HitList.insert( ID );
	return true;
}
