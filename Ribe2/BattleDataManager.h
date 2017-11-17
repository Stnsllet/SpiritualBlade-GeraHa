// BattleDataManager.h: BattleDataManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEDATAMANAGER_H__2E5A3B6C_E5B2_4FEA_8449_0DD28094AC53__INCLUDED_)
#define AFX_BATTLEDATAMANAGER_H__2E5A3B6C_E5B2_4FEA_8449_0DD28094AC53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include "ArmyStatus.h"
#include "RbActStatus.h"
#include "RbBattlePaty.h"
#include "MemberCallBacks.h"

class XMLPaser;
class RbPosition;
class Display;
class RbSprite;

class BattleDataManager  
{
public:
	typedef std::list<RefPtr<RbActStatus> > CharaList;
	class CharaItr
	{
	public:
		CharaItr( CharaList& Source ):Ref(Source),itr(Source.begin()){}
		operator RbActStatus*() const{return *itr;}
		RbActStatus* operator ->() const{return *this;}
		bool Next(){if(IsEnd())return false;itr++;return true;}
		bool IsEnd() const{return (itr==Ref.end());}
		CharaList& Ref;
		CharaList::const_iterator itr;
	};

	BattleDataManager();
	virtual ~BattleDataManager();

	virtual void ClearData();
	virtual HRESULT InitSystem( Display& Disp );

	virtual HRESULT DrawMain( Display& Disp );

	virtual RbBattlePaty* AddPaty( const XMLPaser& PatyXML );
	RbBattlePaty* AddPaty( const PatyStatus& Paty, float x, float y, float angle );
	RbBattlePaty* AddPaty( RbBattlePaty* pPaty );
	RbActStatus* AddCharactor( RbBattlePaty* pPaty, const MemberStatus& Member );
	long DeleteUnUsePaty( bool bAll=false );

	long NameCall( MemCallBack* pCallBack, LPCTSTR Name ) const;

	RbActStatus* GetSelectCharactor() const{return pSelectCharactor;}
	virtual bool SetSelectCharactor( RbActStatus* pCharactor );
	const CharaList& RefAllCharactor() const{return BattleCharactors;}
	long GetArmyID( LPCTSTR name ) const;

	long GetBattleID( RbStatus* pStatus ) const;
	RbActStatus* GetCharactor( long MemberID ) const;
	RbActStatus* GetCharactor( LPCTSTR name ) const;
	RbBattlePaty* GetPaty( long PatyID ) const;
	long GetCharactors( CharaList& Output, LPCTSTR name ) const;
	ArmyStatus& ArmyData( const String& Name );
	long GetMedical( long ArmyID ) const;

protected:
	typedef std::map<String,ArmyStatus> BArmyMap;
	typedef std::list<RefPtr<RbActStatus> > BCharaMap;
	typedef std::map<long,RefPtr<RbBattlePaty> > BPatyMap;
	BArmyMap BattleArmys;
	BCharaMap BattleCharactors;
	BPatyMap BattlePatys;
	void SortCharactorList();
	BCharaMap::iterator SortCharactor(BCharaMap::iterator First, BCharaMap::iterator Last);
protected:
	RefPtr<RbActStatus> pSelectCharactor;
};

#endif // !defined(AFX_BATTLEDATAMANAGER_H__2E5A3B6C_E5B2_4FEA_8449_0DD28094AC53__INCLUDED_)
