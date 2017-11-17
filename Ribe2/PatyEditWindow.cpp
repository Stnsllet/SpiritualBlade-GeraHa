// PatyEditWindow.cpp: PatyEditWindow ÉNÉâÉXÇÃÉCÉìÉvÉäÉÅÉìÉeÅ[ÉVÉáÉì
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbPaty.h"
#include "RbMember.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "BattleSystem.h"
#include "RbWindowSystem.h"
#include "DragItemWindow.h"
#include "StrCommander.h"
#include "FomationSystem.h"
#include "PatyEditWindow.h"

//////////////////////////////////////////////////////////////////////
// ç\íz/è¡ñ≈
//////////////////////////////////////////////////////////////////////

PatyEditWindow::PatyEditWindow()
: UserData( GetGameData() )
{
	EditPatyNum = 0;
	FormationSize = 32;
	FormationScale = FormationSize/BattleSystem::FormSize;

	long Count = FomationSystem::GetAllFormCount();
	BoxList.reserve( Count );
	for( int i = 0; i < Count; i++ )
	{
		const RbPointF Pos( FomationSystem::AllForm[i].x, FomationSystem::AllForm[i].y );
		BoxList.push_back( Pos );
	}
	Title = _T("Unit");
}

PatyEditWindow::~PatyEditWindow()
{
}

LPCTSTR PatyEditWindow::WindowName() const
{
	return _T("PatyEdit");
}

bool PatyEditWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_I4 )return true;
	long NewPatyNum = Value.lVal;
	if( EditPatyNum == NewPatyNum )return true;
	EditPatyNum = NewPatyNum;
	TexWindow.Release();
	return true;
}

HRESULT PatyEditWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	long NewID = UserData.UpDateID();
	if( OldUpDateID != NewID )TexWindow.Release();
	return BaseWindow::Draw( Display, System );
}

RbRect PatyEditWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( Rects.Tactics.Pin(x,y) )return Rects.Tactics;
	if( Rects.EscapeUp.Pin(x,y) )return Rects.EscapeUp;
	if( Rects.EscapeDown.Pin(x,y) )return Rects.EscapeDown;
	if( Rects.FormArea.Pin(x,y) )
	{
		for( int i = 0; i < BoxList.size(); i++ )
		{
			RbRect BoxRect = FormBoxRect( BoxList[i] );
			if( BoxRect.Pin( x, y ) )return BoxRect;
		}
	}

	return RbRect();
}

RbRect PatyEditWindow::FormBoxRect( const RbPointF& Pos ) const
{
	RbPoint Center = Rects.FormArea.Center();
	RbRect Box( -FormationSize/2, -FormationSize/2, FormationSize, FormationSize );
	Box.Move( Center.x, Center.y );
	RbPointF BoxPos = Pos * FormationSize;
	Box.Move( BoxPos.x, BoxPos.y );
	return Box;
}

PatyStatus PatyEditWindow::GetStatus()
{
	PatyStatus EditStatus;
	if( !UserData.GetPaty( EditPatyNum, EditStatus ) )
	{
		EditPatyNum = 0;
		if( !UserData.GetPaty( EditPatyNum, EditStatus ) )
			return AllZero(EditStatus);
	}
	return EditStatus;
}

HRESULT PatyEditWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;
	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;

	DrawWindowTag(System,Title);
	Rects.FormArea = pPlanes->GetRect(_T("fomation"));
	Rects.Tactics = pPlanes->GetRect(_T("tactics"));
	Rects.EscapeHp = pPlanes->GetRect(_T("ehp"));
	Rects.EscapeUp = pPlanes->GetRect(_T("eup"));
	Rects.EscapeDown = pPlanes->GetRect(_T("edown"));
	FormBox = GetCache().SystemPlane.GetPlane(_T("formbox"));

	PatyStatus EditStatus = GetStatus();
	OldUpDateID = UserData.UpDateID();

	LPCTSTR TacticsMsg = _T("ë“ã@");
	switch( EditStatus.Algo )
	{
	case eTacticsAttack:
		TacticsMsg = _T("çUåÇ");
		break;
	case eTacticsSyncAttack:
		TacticsMsg = _T("óVåÇ");
		break;
	case eTacticsHelp:
		TacticsMsg = _T("è]åR");
		break;
	}
	System.Font().Draw(TacticsMsg,Rects.Tactics,DT_CENTER);
	System.Font().Draw(_T("Å£"),Rects.EscapeUp,DT_CENTER);
	System.Font().Draw(_T("Å•"),Rects.EscapeDown,DT_CENTER);

	String EscapeHp;
	EscapeHp.Format(_T("%.0f%%"), EditStatus.EscapeHp);
	System.Font().Draw(EscapeHp,Rects.EscapeHp,DT_CENTER);

	for( int j = 0; j < BoxList.size(); j++ )DrawBox( Surface, BoxList[j] );

	RbPaty Paty( EditStatus );
	long Count = Paty.MemberCount();
	std::list<PatyStatus::PatyMember> SortList;
	for( int i = 0; i < Count; i++ )
	{
		const PatyStatus::PatyMember& Ref = EditStatus.Member[i];
		std::list<PatyStatus::PatyMember>::iterator itr = SortList.begin();
		for( ;itr != SortList.end();itr++ )if( Ref.y < itr->y )break;
		SortList.insert( itr, Ref );
	}
	std::list<PatyStatus::PatyMember>::const_iterator itr = SortList.begin();
	while( itr != SortList.end() )
	{
		const PatyStatus::PatyMember& Ref = *itr++;
		DrawMember( Surface, Ref.x, Ref.y, Ref.id );
	}

	return S_OK;
}

bool PatyEditWindow::DrawMember( RbSprite& Surface, float formX, float formY, LPCTSTR Name ) const
{
	MemberStatus State;
	if( !UserData.GetMember( Name, State ) )return false;

	RbMember Mem(State);
	RbRect Rect = FormBoxRect( RbPointF(formX,formY) );
	Surface.DrawRbImage( State, Rect, true );
	return true;
}

bool PatyEditWindow::DrawBox( RbSprite& Surface, const RbPointF& BoxPos ) const
{
	RbPoint Center = Rects.FormArea.Center();
	RbPointF Pos( Center.x, Center.y );
	Pos += BoxPos * FormationSize;
	Pos.x += - (FormBox.w/2.0f);
	Pos.y += - (FormBox.h/2.0f);
	Surface.DrawPlane( FormBox, Pos.x, Pos.y );
	return true;

}

bool PatyEditWindow::ClickJob( int x, int y )
{
	if( Rects.EscapeUp.Pin( x, y ) )ChangeEscapeHp( +10.0f );
	if( Rects.EscapeDown.Pin( x, y ) )ChangeEscapeHp( -10.0f );
	if( Rects.Tactics.Pin( x, y ) )ChangeTactics();

	ClickMember( x, y );

	return true;
}

bool PatyEditWindow::ClickMember( int x, int y )
{
	String Name = GetMemberName( x, y );
	if( Name.empty() )return false;

	StrCommander().ShowMemberState( Name );
	return true;
}

bool PatyEditWindow::ChangeEscapeHp( float Add )
{
	PatyStatus State = GetStatus();
	RbPaty Paty(State);
	Paty.SetEscapeHp( State.EscapeHp + Add );
	return UserData.UpDatePatyStatus( EditPatyNum, State );
}

bool PatyEditWindow::ChangeTactics()
{
	PatyStatus State = GetStatus();
	RbPaty Paty(State);
	TacticsAlgoType NewAlgo = (TacticsAlgoType)(State.Algo+1);
	if( NewAlgo >= eTacticsMax )NewAlgo = eTacticsNone;

	Paty.SetTactics( NewAlgo );
	return UserData.UpDatePatyStatus( EditPatyNum, State );
}

String PatyEditWindow::GetMemberName( int x, int y )
{
	PatyStatus PatyState = GetStatus();
	long MemID = GetFormMember( PatyState, x, y );
	if( MemID < 0 )return String();

	return String( PatyState.Member[MemID].id );
}

long PatyEditWindow::GetFormMember( const PatyStatus& State, int x, int y ) const
{
	if( !Rects.FormArea.Pin( x, y ) )return -1;

	RbPaty Paty(State);
	long MemberCount = Paty.MemberCount();
	for( int i = 0; i < MemberCount; i ++ )
	{
		const PatyStatus::PatyMember& RefMem = State.Member[i];
		if( FormBoxRect( RbPointF(RefMem.x,RefMem.y) ).Pin( x, y ) )
			return i;
	}
	return -1;
}

bool PatyEditWindow::DragStartJob( WindowSystem& System, GameMessage& State )
{
	if( State.IsDraging() )return false;
	int x = State.x - Position.x;
	int y = State.y - Position.y;

	PatyStatus PatyState = GetStatus();
	long MemID = GetFormMember( PatyState, x, y );
	if( MemID < 0 )return false;

	if( MemID == 0 )
	{
		SystemPopup( _T("dontmove_l") );
		return true;
	}

	MemberStatus MemState;
	if( !UserData.GetMember( PatyState.Member[MemID].id, MemState ) )return false;

	bool bFromEx = (EditPatyNum==UserData.RefExPatyNum());
	RefPtr<DragItemWindow> pDrag( new DragItemWindow );
	if( !pDrag->SetMemberData( MemState,bFromEx ) )return false;
	pDrag->SetDrag( System, State );
	return true;
}

bool PatyEditWindow::DropJob( const DragItemWindow& DragItem )
{
	if( DragItem.DragType() != eDragMember )return false;

	RbPoint Pos = DragItem.Pos();

	PatyStatus PatyState = GetStatus();
	RbPaty Paty( PatyState );
	const MemberStatus& RefMem = DragItem.DragData().Member;
	if( !Paty.InMember( RefMem.PasonName ) )
	{
		if( !UserData.ChangePaty( RefMem, EditPatyNum ) )return false;
	}

	PatyState = GetStatus();
	long DragMemID = Paty.FindMember( RefMem.PasonName );
	if( DragMemID < 0 )return false;

	if( DragMemID == 0 )
	{
		SystemPopup( _T("dontmove_l") );
		return true;
	}

	PatyStatus::PatyMember& DragData = PatyState.Member[DragMemID];

	for( int i = 0; i < BoxList.size(); i++ )
	{
		if( FormBoxRect(BoxList[i]).Pin( Pos.x, Pos.y ) )break;
	}
	if( i >= BoxList.size() )return false;
	const RbPointF& NewPos = BoxList[i];

	long OldMemID = GetFormMember( PatyState, Pos.x, Pos.y );

	if( OldMemID >= 0 )
	{
		PatyStatus::PatyMember& OldData = PatyState.Member[OldMemID];
		OldData.x = DragData.x;
		OldData.y = DragData.y;
	}
	DragData.x = NewPos.x;
	DragData.y = NewPos.y;
	if( OldMemID == 0 )
	{
		Paty.ChangeIndex( DragData.id, OldMemID );
		Paty.ResetFormation( UserData );
	}

	return UserData.UpDatePatyStatus( EditPatyNum, PatyState );
}
