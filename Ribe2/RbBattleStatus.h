// RbBattleStatus.h: RbBattleStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBBATTLESTATUS_H__A7092FFB_397E_408F_BD2C_44BB7FDDEDFA__INCLUDED_)
#define AFX_RBBATTLESTATUS_H__A7092FFB_397E_408F_BD2C_44BB7FDDEDFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbStatus.h"
#include "RbRect.h"
#include "RbPosition.h"
#include "BattleStatus.h"
class Display;
class BattleSystem;
class RbBattlePaty;
class RbActStatus;
class RbSprite;
class RbTransform;

class RbBattleStatus  : public RbStatus
{
friend class RbActStatus;
public:
	typedef RbTexture::TexturePlane TexturePlane;

	RbBattleStatus();
	virtual ~RbBattleStatus();

	bool SetActImage( long ActionID, float Direction, int Time );
	virtual const ActTime* GetNowImage() const;

	virtual bool DropJob();

	HRESULT Draw( Display& Disp, const RbTransform& AllProj );
	HRESULT ResetData();
	virtual HRESULT SetData( RbBattlePaty* pPatyData, const MemberStatus& Member );
	void SetPaty( RbBattlePaty* pPatyData );
	bool SetAngle( float NewAngle );

	bool UseMp( float Mp );
	const RbPosition& GetPosition() const;
	RbPosition GetHeadPosition( Display& Disp ) const;
	RbRectF GetDisplayRect( Display& Disp ) const;

	bool ChangeBatType ( BattleAlgoType NewType );
	RbBattlePaty* GetPaty() const{return pPaty;}
	float GetAngle() const{return Status.Direction;}
	float GetLinerAngle( float Range, float Dips=2/12.0f, float Width=0 );

	BattleAction GetAction() const{return Status.Action;}
	bool SetDelete();
	bool IsDead() const{return (NowStatus.Hp<=0);}
	float GetComboRate() const;
	long ID() const{return BattleID;}
	long GetArmyID() const{return Status.ArmyID;}
	bool IsUnView() const{return (Status.bDelete);}
	bool IsLifeBar() const{return (Status.HpBarTime>0);}
	bool IsEscapeHp() const{return (NowStatus.Hp <= Status.EscapeHp);}
	bool IsUserData() const{return Status.bUserData;}
	bool IsDeleteOK() const;
	bool IsUsedSkill( long ID ){return (UsedSkills.find(ID)!=UsedSkills.end()); }
	bool InRange( const D3DVECTOR& Pos, float Range ) const;
	bool IsTexAct( long ActID ) const;
	bool IsOverDrive() const;
	bool IsFly() const{return (PasonalEffect.FlyHight>0);}

	bool TexSound() const{return TexSound(Status.Action);}
	bool TexSound( BattleAction Action ) const;
	bool TexSound( long ActID ) const;
	HRESULT DrawPos( Display& Disp, const ActTime& TimeData, const D3DVECTOR& Pos, const RbTransform& AllProj );
	long UsedSkillCount() const{return UsedSkills.size();}
protected:
	HRESULT DrawShadow( Display& Disp, const D3DVECTOR& Pos );
	HRESULT DrawColor( Display& Disp, const ActTime& TimeData, const D3DVECTOR& Pos, const RbTransform& AllProj );
	HRESULT Draw2( Display& Disp, const TexturePlane& SourcePlane );
	virtual bool RestoreUserData();

	static long IDManage;
	long BattleID;

	RefPtr<RbBattlePaty> pPaty;
	const ActTime* pDrawAction;

	BattleStatus Status;

	std::set<long> UsedSkills;

	BattleSystem& System;
public:
	const BattleEffect::_EffectSet& PasonalEffect;
	const BattleEffect::_Leader& LeaderEffect;
	const BattleStatus& RefBattleStatus() const{return Status;}
};

#endif // !defined(AFX_RBBATTLESTATUS_H__A7092FFB_397E_408F_BD2C_44BB7FDDEDFA__INCLUDED_)
