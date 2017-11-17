// RbActStatus.cpp: RbActStatus クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbPosition.h"
#include "RbCache.h"
#include "RbSound.h"
#include "MoveLiner.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "RbActStatus.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbActStatus::RbActStatus()
: TagetSys( System, *this )
, AttackAlgo( System, *this, TagetSys )
, AlgoMove( *this, Status, System )
, AlgoTactics( *this, System )
, MyName(SourceStatus.PasonName)
{
	MyPatyStatus = ePatyWait;
}

RbActStatus::~RbActStatus()
{

}

bool RbActStatus::DropJob()
{
	SetDelete();
	TagetSys.ClearTaget();
	return RbBattleStatus::DropJob();
}

RbActStatus* RbActStatus::GetLeader() const
{
	RbActStatus* pLeader = const_cast<RbActStatus*>(this);
	if( pPaty == NULL )return pLeader;
	 pLeader = pPaty->GetLeader();
	if( pLeader == NULL )pLeader = const_cast<RbActStatus*>(this);
	return pLeader;
}

const RbBattleStatus::ActTime* RbActStatus::GetNowImage() const
{
	if( pDrawAction != NULL )return pDrawAction;
	if( pTexture == NULL )return NULL;
	return &(pTexture->GetAction( AttackAlgo.ActID(), Status.Direction, Status.ActTime ) );
}

long RbActStatus::GetWeponID() const
{
	return AttackAlgo.NowWepon().ItemNumber;
}

void RbActStatus::GetWeponImage( BillbordPlane& Output ) const
{
	RefBord* pImage = GetWeponImage2();
	if( pImage == NULL )return;
	Output = *pImage;
}

RefBord* RbActStatus::GetWeponImage2() const
{
	return AttackAlgo.GetWeponImage();
}

bool RbActStatus::IsAction() const
{
	if( Status.Action == eActStop )return false;
	if( Status.Action == eActMove )return false;
	return true;
}

bool RbActStatus::IsActLock() const
{
	if( Status.Action == eActDamege )return true;
	if( Status.Action == eActDead )return true;
	if( Status.Action == eActSpecial )return true;
	if( ActLock.IsLock() )return true;
	return false;
}

bool RbActStatus::IsPatyMoving() const
{
	if( MyPatyStatus == ePatyMove )return true;
	if( MyPatyStatus == ePatyEscape )return true;
	return false;
}

bool RbActStatus::IsPatyBattle() const
{
	return (MyPatyStatus == ePatyBattle);
}

HRESULT RbActStatus::SetData( RbBattlePaty* pPatyData, const MemberStatus& Member )
{
	HRESULT ret = RbBattleStatus::SetData( pPatyData, Member );
	if( pPatyData != NULL )AlgoTactics.SetTactics( pPatyData->RefTacticsType() );

	AlgoMove.SetMoveSpeed( Member.Status.Move * LeaderEffect.MoveSpeed );
	AttackAlgo.CheckWepon();
	Status.bMoreSpecial = ReCheckSpecial();
	return ret;
}

bool RbActStatus::AddActTime( BattleSystem& System )
{
	if( pTexture == NULL )return true;
	Status.ActTime += System.JobTime();

	pDrawAction = NULL;
	long ActID = AttackAlgo.ActID();
	int MaxTime = pTexture->GetActionTime( ActID, Status.Direction );

//	if( ActLock.IsLock() )return false;
	if( Status.ActTime >= MaxTime )
	{
		Status.ActTime = 0;
		return true;
	}
	return false;
}

bool RbActStatus::PatyChangeState( PatyState NewState )
{
	if( !Status.bLeader )return false;
	if( pPaty == NULL )return false;
	return pPaty->ChangeState( NewState );
}

void RbActStatus::ChangeState( PatyState NewState )
{
	MyPatyStatus = NewState;
	switch( NewState )
	{
	case ePatyBattle:
		AttackAlgo.WideWepon();
		if( Status.Action == eActMove )Cancel();
		else MoveCancel();
		break;
	case ePatyWait:
		if( !IsActLock() )Cancel();
	default:
		MoveCancel();
		AlgoMove.MoveToFormation();
		break;
	}

}

bool RbActStatus::BattleCheck( BattleSystem& System )
{
	if( MyPatyStatus == ePatyEscape )return false;
	float CheckRange = System.BatIn;
	if( MyPatyStatus == ePatyBattle )CheckRange = System.BatKeep;

//	long EnemyCount = System.LeaderCount( this, CheckRange );
//	if( EnemyCount <= 0 )
	RbActStatus* pTaget = TagetSys.GetMinEnemy();
	float Range = TagetSys.GetMinRange();
	bool bBattle = (pTaget!=NULL) && (Range < CheckRange);
	if( bBattle )
	{
		if( MyPatyStatus != ePatyBattle )PatyChangeState( ePatyBattle );
		return true;
	}

	if( MyPatyStatus == ePatyBattle )
	{
		if( AlgoMove.IsForce() )PatyChangeState( ePatyMove );
		else PatyChangeState( ePatyWait );
	}
	return false;
}

// ターゲットのほうを向く
float RbActStatus::TagetAngle( RbActStatus* pTaget )
{
	if( pTaget == NULL )return Status.Direction;
	const RbPosition& Pos = pTaget->GetPosition();
	SetAngle( GetPosition().GetDirection( pTaget->GetPosition() ) );
	return Status.Direction;
}

void RbActStatus::Job( BattleSystem& System )
{
	if( Status.bDelete )
	{
		if( Status.bLeader )
		{
			if( MyPatyStatus == ePatyEscape )PatyChangeState( ePatyWait );
			BattleCheck( System );
		}
		return;
	}
	Waiting();
	if( Status.HpBarTime>0 )Status.HpBarTime -= System.JobTime();

	AlgoMove.KnockBackJob();
	if( MyPatyStatus == ePatyBattle )AlgoBattle( System );
	else _Job( System );
}

void RbActStatus::_Job( BattleSystem& System )
{
	switch( Status.Action )
	{
	case eActStop: ActStop( System ); break;
	case eActMove: ActMove( System ); break;
	case eActWait: ActWait( System ); break;
	case eActDamege: ActDamege( System ); break;
	case eActAttack: ActAttack( System ); break;
	case eActSpecial: ActSpecial( System ); break;
	case eActDead: ActDead( System ); break;
	case eActRecover: ActRecover( System ); break;
	}
}

bool RbActStatus::CounterAction()
{
	Status.Wait = WaitTime();
	return true;
}

void RbActStatus::ResetCombo()
{
	if( GetArmyID() != ArmyStatus::eUser )System.BStatus.RefResult().SetMaxCombo( Status.Combo );
	Status.Combo = 0;
}

bool RbActStatus::ReCheckSpecial()
{
	if( IsDead() )return false;
	const long Count = SkillList.Action.GetCount();
	for( long i = 0; i < Count; i++ )
	{
		if( UsedSkills.find(SkillList.Action.Get( i ).ID)
			== UsedSkills.end() )return true;
	}
	return false;
}

void RbActStatus::ActStart( BattleAction Action, bool bActTimeReset )
{
	if( Status.Action == Action )return;
	Status.Action = Action;
//	Status.Wait = 0;
	ResetCombo();
	if( bActTimeReset )Status.ActTime = 0;
	pDrawAction = NULL;
}

void RbActStatus::Escape( RbActStatus* pTaget )
{
	if( Status.Action == eActRecover )return;
	if( pTaget == NULL )return;

	if( SkillList.Effect.PotionOK() )
	{
		DamegeSystem(*this, *this).HealDamege( AllStatus.Hp*PasonalEffect.PotionPow );
		return;
	}
	RbPosition Vector = Status.Pos;
	Vector -= pTaget->GetPosition();

	const float Length = Vector.Length();
	if( Length > 0 )Vector *= Size()*2.0f/Length;

	RbPosition CheckPos = Vector + Status.Pos;
	if( !System.Land.MoveOK(CheckPos,Size()) )
	{
		CheckPos = Vector.Rotate( -0.25f ) + Status.Pos;
		if( !System.Land.MoveOK(CheckPos,Size()) )
		{
			CheckPos = Vector.Rotate( 0.5f ) + Status.Pos;
			if( !System.Land.MoveOK(CheckPos,Size()) )
				CheckPos = System.Land.MovingPos( Vector.Rotate( -0.25f )+Status.Pos, Status.Pos, Size() );
		}
	}
//	Vector = System.Land.MovingPos( Vector, Status.Pos, Size() );
	if( Length < System.EscapeRange
		&& AlgoMove.Run() < System.EscapeRun
		&& !CheckPos.NiarXY2(Status.Pos,9) )
	{
		if( !AlgoMove.SetMovePos( CheckPos.x, CheckPos.y) )Recover();
	}
	else Recover();
}
void RbActStatus::EscapeMove( RbActStatus* pTaget )
{
	if( Status.Action == eActRecover )return;
	if( pTaget == NULL )return;

	RbPosition Vector = Status.Pos;
	Vector -= pTaget->GetPosition();
	float Length = Vector.Length();
	if( Length >= System.BatKeep )
	{
		Stop();
		return;
	}

	Vector += Status.Pos;
	Vector = System.Land.MovingPos( Vector, Status.Pos, Size() );
	AlgoMove.SetMovePos( Vector.x, Vector.y);
}

bool RbActStatus::Damege( float Damege, float DamegeWait, const D3DVECTOR& KockBack )
{
	if( IsDead() )return false;

	pDrawAction = NULL;
	Status.HpBarTime = 20000;
	NowStatus.Hp -= Damege;
	Update();
	TexSound( eActDamege );

	// 死亡？
	if( NowStatus.Hp < 1 )if( DeadCheck() )return true;

	if( Status.Action == eActDamege )++Status.Combo;

	// 技中はよろめかない
	if( Status.Action == eActSpecial )return false;
	if( DamegeWait <= 0 )return false;
	// スキルでよろめかない
	if( Damege <= (AllStatus.Hp*PasonalEffect.UnShake) || IsOverDrive() )return false;

	AlgoMove.AddKnockBack( KockBack );

	if( Status.Action != eActDamege )Status.OldAction = Status.Action;
	if( Status.OldAction == eActRecover )Status.Wait = 0;

	Status.Action = eActDamege;
	Status.ActTime = 0;
	float NewWait = DamegeWait + (Status.DamegeWait/3.0f);
	if( NewWait > Status.DamegeWait )Status.DamegeWait = NewWait;
	return false;
}

 // 死亡時チェック
bool RbActStatus::DeadCheck()
{
	if( NowStatus.Hp >= 1 )return false;

	if( LeaderEffect.GutsLife > 0 )
		if( Rnd100(LeaderEffect.GutsLife) )
		{
			NowStatus.Hp = 1;
			return false;
		}

	if( SkillList.Effect.ReviveOK() )
	{
		DamegeSystem Heal( *this, *this );
		NowStatus.Hp = 1;
		Heal.AllRecover( LeaderEffect.RevivePow );
		if( Status.Action != eActSpecial )Cancel();
		return false;
	}

	System.TobiSys.AddSelfBomb( *this, PasonalEffect.SelfBomb );

	Dead();
	return true;
}

bool RbActStatus::RecoverLife( float Hp, float Mp )
{
	if( IsDead() )return true;
	HpJob( Hp );
	MpJob( Mp );
	if( NowStatus.Hp >= AllStatus.Hp
		&& NowStatus.Mp >= AllStatus.Mp )return true;
	return false;
}

void RbActStatus::Recover()
{
	ActStart( eActRecover );
	Status.Wait = 0;
	AlgoMove.Run() = 0;
}

void RbActStatus::Dead()
{
//	TagetSys.ClearTaget();

	ActStart( eActDead );
	TexSound();
	Status.HpBarTime = 0;
	NowStatus.Hp = 0;
	NowStatus.Mp = 0;
	Status.bMoreSpecial = false;
	if( pPaty != NULL )pPaty->DeadCheck( ID() );
}

void RbActStatus::Cancel()
{
	ActStart( eActStop, false );
	AttackLock.Release();
	SpecialLock.Release();
	if( !AlgoMove.Cancel() )
	{
		ActStart( eActMove, false );
		AlgoMove.ResetAngle();
	}
	else if( !AlgoMove.CheckLeaderLength() )
		if( !IsPatyBattle() )
			AlgoMove.MoveToFormation();
}

bool RbActStatus::MoveCancel()
{
	if( Status.Action != eActRecover
		&& Status.Action != eActWait)return false;
	Cancel();
	return true;
}

void RbActStatus::Stop()
{
	if( MyPatyStatus != ePatyBattle )PatyChangeState( ePatyWait );
	AlgoMove.Run() = 0;

	if( !IsActLock() )ActStart( eActStop, false );
	if( !IsPatyBattle() )AlgoMove.MoveToFormation();
}

void RbActStatus::Wait()
{
	AlgoMove.Run() = 0;
	if( !AttackAlgo.CheckMp() )
	{
		Recover();
		return;
	}
	if( IsActOK() )
	{
		Attack();
		return;
	}
	ActStart( eActWait );
	TagetAngle( TagetSys.GetAttackTaget() );
}

void RbActStatus::Regenation()
{
	const float& MaxHp = AllState().Hp;
	const float& Hp = NowState().Hp;
	if( Hp >= MaxHp )return;
	if( PasonalEffect.Regenation > 0 )HpJob( PasonalEffect.Regenation*MaxHp );
	if( LeaderEffect.LifeTrans <= 0 )return;
	if( Hp >= MaxHp )return;
	float RecoverHp = MaxHp * LeaderEffect.LifeTrans;
	if( RecoverHp > MaxHp - Hp )RecoverHp = MaxHp - Hp;
	const float UseMp = RecoverHp*LeaderEffect.MagicForce;
	if( UseMp > NowState().Mp )RecoverHp = NowState().Mp;
	HpJob( RecoverHp );
	MpJob( -UseMp );
}

void RbActStatus::Waiting()
{
	if( IsDead() )return;

	Regenation();
	if( Status.Action == eActDamege )return;

	if( IsFly() )
	{
		float Move = 10.0f*System.JobSec();
		Status.FlyHight = min( PasonalEffect.FlyHight, Status.FlyHight+Move );
		Status.Pos.z = AlgoMove.MyLandHight() + Status.FlyHight;
	}
	if( IsActOK() )return;

	// Speed 0 、約4秒で行動
	Status.Wait += (NowStatus.Speed + 400)*LeaderEffect.Activate;
}
void RbActStatus::Attack()
{
	ActStart( eActAttack );
	AttackAlgo.AttackStart();
	AttackLock.Lock( ActLock );
//	TagetAngle( AttackAlgo.GetTaget() );
}

void RbActStatus::ActStop( BattleSystem& System )
{
	if(MyPatyStatus == ePatyWait )
		if( NowStatus.Hp < AllStatus.Hp
		|| NowStatus.Mp < AllStatus.Mp )
	{
		Recover();
		return;
	}

	AddActTime( System );

	if( IsPatyMoving() )
	{
		if( Status.bLeader )
		{
			ActStart( eActMove );
			AlgoMove.ResetAngle();
		}
		else AlgoMove.MoveToFormation();
	}
	else if( Status.bLeader )
	{
		AlgoTactics.CheckTactics();
		BattleCheck( System );
	}
	else AlgoMove.CheckLeaderLength();
}

void RbActStatus::ActDead( BattleSystem& System )
{
	if( AddActTime( System ) )
	{
		if( pTexture  != NULL )
			pDrawAction = &(pTexture->GetAction(_T("delete"),Status.Direction,0));
	//	ActStart(eActDead);
	//	pDrawAction = GetNowImage();
		Status.bDelete = true;
	}
}

void RbActStatus::ActDamege( BattleSystem& System )
{
	Status.DamegeWait -= System.JobTime();
	if( Status.DamegeWait > 0 )AddActTime( System );
	else
	{
		switch(Status.OldAction)
		{
		case eActSpecial: Cancel(); break;
		case eActMove:case eActStop:
			{ Cancel();break; }
		case eActWait: case eActAttack: if( IsEscapeHp() ){Status.OldAction = eActStop;Cancel();} //no break;
		default: 
			if( AlgoMove.IsForce() ){Cancel();break;}
			if( !Status.bLeader )if( IsPatyMoving() ){Cancel();break;}
			Status.Action = Status.OldAction; break;
		}
		ResetCombo();
		if( Status.Action == eActMove )
		{
			AlgoMove.ResetAngle();
		}
	}

	if( Status.FlyHight > 0 )
	{
		Status.FlyHight = max( 0.0f, Status.FlyHight-1.0f);
		Status.Pos.z = AlgoMove.MyLandHight()+Status.FlyHight;
	}
}

void RbActStatus::ActWait( BattleSystem& System )
{
	if( !AttackAlgo.InTagetRange(TagetSys.GetNowTaget())
		|| IsPatyMoving() )
	{
		Cancel();
		return;
	}
	AddActTime(System);
	// Speed 0 、約3秒で行動
	if( IsActOK() )Attack();
}

bool RbActStatus::SpecialGo( const String& SkillName )
{
	return SpecialGo( GetCache().Skill.GetSkillNo( SkillName ) );
}

bool RbActStatus::SpecialGo( long SkillID )
{
	if( IsDead() )return false;
	if( Status.Action == eActSpecial )return false;
	if( UsedSkills.find(SkillID)!=UsedSkills.end() )return false;

	long Level = SkillList.Action.GetSkill( SkillID ).Level;
	if( Level <= 0 )return false;

	if( !AttackAlgo.SpecialGo( SkillID, Level ) )return false;

	AlgoMove.NoKnockBack();
	UsedSkills.insert( SkillID );
	ActStart( eActSpecial );
	Status.bMoreSpecial = ReCheckSpecial();
	SpecialLock.Lock( ActLock );
	AttackLock.Release();

	return true;
}

void RbActStatus::ActRecover( BattleSystem& System )
{
	AddActTime(System);
	// Speed 0 、約3秒で回復開始 20秒で回復
	if( IsActOK() )
	{
		float Recover = System.JobSec()/20;
		Recover *= LeaderEffect.MedicalPow;
		float RecoverHp = Recover * AllStatus.Hp;
		float RecoverMp = Recover * AllStatus.Mp;
		if( RecoverLife( RecoverHp, RecoverMp ) )Cancel();
	}
	if( Status.bLeader )
	{
		if( !BattleCheck( System ) )
			if(AlgoTactics.ForceTactics() )
				if( AlgoTactics.CheckTactics() )return;
	}
}

void RbActStatus::ActAttack( BattleSystem& System )
{
	if( AddActTime(System) )
	{
		if( AttackLock.Release() )AttackAlgo.AttackEnd();
		if( !ActLock.IsLock() )
		{
			if( SkillList.Effect.RushOK() )CounterAction();
			else Status.Wait = 0;
			Cancel();
		}
	}
	else  AttackAlgo.AttackJob( Status.ActTime );
}

void RbActStatus::ActSpecial( BattleSystem& System )
{
	if( AddActTime(System) )
	{
		if( SpecialLock.Release() )AttackAlgo.SpecialEnd();
		if( !ActLock.IsLock() )
		{
			Status.Wait = 0;
			Cancel();
		}
	}
}

void RbActStatus::ActMove( BattleSystem& System )
{
	AlgoMove.ActMove();
	return;
}

void RbActStatus::AlgoBattle( BattleSystem& System )
{
	if( Status.bLeader )BattleCheck( System );
	bool bActLock = IsActLock();
	if( bActLock || !IsPatyBattle() || Status.BattleType==eBatNotBat )
	{
		_Job( System );
		return;
	}
	if( AlgoMove.IsForce() )
	{
		if( AlgoMove.IsFromUser() )
		{
			_Job( System );
			if( !AlgoMove.IsForce() )
			{
				RefPtr<RbActStatus> pEnemy = TagetSys.GetMinEnemy();
				if( pEnemy != NULL )if( pEnemy->GetPosition().Length( GetPosition() ) > System.EscapeRange )
					if( Status.BattleType!=eBatRaging )Recover();
			}
		}
		else
		{
			_Job( System );
		}
		return;
	}

	RefPtr<RbActStatus> pEnemy = TagetSys.GetMinEnemy();
	if( pEnemy == NULL )
	{
		Stop();
		return;
	}

	if( Status.BattleType == eBatEscape )
	{
		EscapeMove( pEnemy );
		_Job( System );
		return;
	}
	if( IsEscapeHp() && Status.BattleType!=eBatRaging )
	{
		Escape( pEnemy );
		_Job( System );
		return;
	}

	bool bAction = IsAction();
	if( !bAction )
	{
		if( AttackAlgo.ChangeWepon( pEnemy )
			&& AttackAlgo.NowWepon().InTagetRange(pEnemy,*this) )
		{
			Wait();
			return;
		}
		else if( !AttackAlgo.CheckMp() )
		{
			Recover();
			return;
		}
		else if( Status.BattleType != eBatDontMove )
		{
			if( Status.Action != eActMove || AlgoMove.MoveInterval(250) )
				if( !AlgoMove.CheckLeaderLength() )
					AlgoMove.SetMovePos( pEnemy );
			ActMove( System );
			return;
		}
	}
	_Job( System );
}
