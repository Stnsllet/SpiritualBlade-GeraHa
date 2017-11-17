// RbPaty.h: RbPaty クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPATY_H__5F108FE0_1D64_4ED7_94CE_050C361A34FD__INCLUDED_)
#define AFX_RBPATY_H__5F108FE0_1D64_4ED7_94CE_050C361A34FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Statusstruct.h"

class XMLPaser;
class RbGameData;

class RbPaty  
{
public:
	RbPaty( const PatyStatus& Source );
	RbPaty( PatyStatus& Source );
	~RbPaty();
	float SetEscapeHp( float NewData );
	bool SetTactics( TacticsAlgoType NewData );
	bool AddMember( const MemberStatus& AddMember, long Index=-1 );
	bool DropMember( LPCTSTR Name );
	bool ChangeIndex( LPCTSTR Name, long Index );
	bool Create();

	bool AddOK() const;
	long FindMember( LPCTSTR Name ) const;
	bool InMember( LPCTSTR Name ) const;
	bool IsMaxMember() const;
	long MemberCount() const;
	const PatyStatus& Status() const{return RefPaty;}
	PatyStatus& RefPaty;

	bool ResetFormation( const RbGameData& UserData );

private:
	void Insert( long Index, const PatyStatus::PatyMember& Data );
	float GetWeponRange( LPCTSTR Member, const RbGameData& UserData ) const;
	bool ResetFormation( const MemberStatus& Member, PatyStatus::PatyMember& Data );
	const long MaxMember;
};

#endif // !defined(AFX_RBPATY_H__5F108FE0_1D64_4ED7_94CE_050C361A34FD__INCLUDED_)
