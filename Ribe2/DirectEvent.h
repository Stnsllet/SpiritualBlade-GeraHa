// DirectEvent.h: DirectEvent クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTEVENT_H__36282408_81EC_4771_AC80_4A61926FA7E8__INCLUDED_)
#define AFX_DIRECTEVENT_H__36282408_81EC_4771_AC80_4A61926FA7E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JobEventBase.h"
#include "MemberCallBacks.h"

class DirectEvent  
{
public:

#define CEventN(x,n) struct x:public _EventCall \
	{public:bool Event( const XMLPaser& EventXML, GameSystem& System ); \
		LPCTSTR Name() const{return n;} \
		};
#define CEvent(x) CEventN(x,#x)

	CEventN( EventBase, _T("Event") );
	CEvent( Bat );
	CEvent( Sound );
	CEvent( Paty );
	CEvent( Move );
	CEvent( Land );
	CEvent( Mode );
	CEvent( Msg );
	CEvent( Cache );
	CEvent( LoadEvent );
	CEvent( List );
	CEvent( ZoomIn );
	CEvent( AddLine );
	CEvent( Choice );
	CEvent( ChkFlg );
	CEvent( Tutorial );
	CEvent( Rnd );
	CEvent( Command );
	CEvent( UserData );
	CEvent( Title );
	CEvent( Powered );
	CEvent( Poly );
	CEvent( Shop );
	CEvent( Result );
	CEvent( Popup );
	CEvent( Param );
	CEvent( CallTrans );
	CEvent( Window );

	static bool CheckFlags( const XMLPaser& EventXML, GameSystem& System );
	struct BatCallBacks
	{
		struct ChangeTactics : MemCallBack
		{
			ChangeTactics( BattleSystem& RefSystem );
			long Change( const String& RefName, const String& NewTactics );
			virtual bool Call( RbActStatus& State );
			BattleSystem& System;
			TacticsAlgoType NewType;
		};
		struct SpecialAttack : MemCallBack
		{
			SpecialAttack( GameSystem& RefSystem );
			long Go( const String& RefName, const String& Special );
			virtual bool Call( RbActStatus& State );
			GameSystem& System;
			long SkillID;
		};
		struct BraveChange : MemCallBack
		{
			BraveChange( BattleSystem& RefSystem );
			long Go( const String& RefName, float Brave );
			virtual bool Call( RbActStatus& State );
			BattleSystem& System;
			std::map<long,RefPtr<RbBattlePaty> > PatyList;
		};
		struct Escape : MemCallBack
		{
			Escape( BattleSystem& RefSystem );
			long Go( const String& RefName, const String& EscapeType );
			virtual bool Call( RbActStatus& State );
			BattleSystem& System;
			enum MyType
			{
				eEscapeNone = 0,
				ePaty,
				eArmy,
			} Type;
		};
	};
};

#endif // !defined(AFX_DIRECTEVENT_H__36282408_81EC_4771_AC80_4A61926FA7E8__INCLUDED_)
