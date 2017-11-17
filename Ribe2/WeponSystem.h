// WeponSystem.h: WeponSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEPONSYSTEM_H__1DD43244_1F01_4442_8C40_2D71F02D1C8F__INCLUDED_)
#define AFX_WEPONSYSTEM_H__1DD43244_1F01_4442_8C40_2D71F02D1C8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RbBattleStatus;
class RbActStatus;
class TobiObjectBase;
class TobiCreator;
#include "BillbordPlane.h"

struct WeponSystem  
{

public:
	struct WeponData
	{
		WeponData(){AllZero(*this);}
		bool SetData( long ItemNum, const RbActStatus& Source, long* pChainWepon=NULL, bool bWepon=false );
		bool CheckMp( const RbActStatus& Source ) const;
		bool IsAttackOK( RbBattleStatus* pTaget, const RbActStatus& Source ) const;
		bool InTagetRange( RbBattleStatus* pTaget, const RbActStatus& Source ) const;
		bool InAttackRange( RbBattleStatus* pTaget, const RbActStatus& Source ) const;
		long ItemNumber;
		long ActID;
		TobiCreator* pCreator;
		float AttackRange, TagetRange;
		RefPtr<RefBord> pImage;
		SkillEffect Effect;
		float UseMp( long Level ) const;
	};

	class WeponList
	{
	public:
		WeponList();
		bool Add( long ItemID, const RbActStatus& Source, bool bWepon=false );
		bool Add( const WeponData& NewData );
		bool ChangeWepon( RbActStatus* pTaget, const RbActStatus& Source );
		bool RandomWepon( RbActStatus& Source );
		bool SetWideWepon( const RbActStatus& Source );
		long Count() const{return Wepons.size();}
		const WeponData& Now() const;
	protected:
		WeponData* pNowWepon;
		std::list<WeponData> Wepons;
		RefPtr<RefBord> pBaseImage;
	};
};

#endif // !defined(AFX_WEPONSYSTEM_H__1DD43244_1F01_4442_8C40_2D71F02D1C8F__INCLUDED_)
