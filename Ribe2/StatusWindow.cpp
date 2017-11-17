// StatusWindow.cpp: StatusWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbMember.h"
#include "RbCache.h"
#include "StrCommander.h"
#include "RbWindowSystem.h"
#include "MiniStatusWindow.h"
#include "StatusWindow.h"
#include "BattleSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

StatusWindow::StatusWindow()
{
	bEquipMode = false;
	UpdateID = 0;
	Rects.SkillCount = 0;
}

StatusWindow::~StatusWindow()
{

}

bool StatusWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_BSTR )return StatusBaseWinow::SetParameter( Value );
	String Name = Value.bstrVal;
	if( Name.empty() )return false;

	RefPtr<RbActStatus> pNewStatus;
	if( !bEquipMode )pNewStatus = GetBattleSystem().GetCharactor(Name);
	if( pNewStatus == NULL )return false;

	pStatus = pNewStatus;
	TexWindow.Release();

	return false;
}

LPCTSTR StatusWindow::WindowName() const
{
	return _T("Status");
}

// レストア
HRESULT StatusWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	LastRedraw = RbTimer();
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName() );
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surfase;
	RestoreBase( pDevice, Surfase, System );

	DrawWindowTag( System, StatusWindow::WindowName() );

	if( pStatus == NULL )return E_FAIL;
	UpdateID = pStatus->GetUpdateID();
	const RbCache& Cache = GetCache();

	ImageArea = pPlanes->GetRect(_T("image"));

	Rects.Name = pPlanes->GetRect(_T("name"));
	Rects.Job = pPlanes->GetRect(_T("job"));
	Rects.NewsBtn = pPlanes->GetRect(_T("news"));
	Rects.Wepon = pPlanes->GetRect(_T("WEP"));
	Rects.Body = pPlanes->GetRect(_T("BODY"));
	Rects.Acs = pPlanes->GetRect(_T("ACS"));

	Rects.Tag = pPlanes->GetRect(_T("tag"));
	Rects.Value = pPlanes->GetRect(_T("value"));
	Rects.Now = pPlanes->GetRect(_T("now"));
	Rects.Slash = pPlanes->GetRect(_T("slash"));
	Rects.Source = pPlanes->GetRect(_T("src"));
	Rects.Plus = pPlanes->GetRect(_T("plus"));
	Rects.Leader = pPlanes->GetRect(_T("leader"));
	Rects.UpDown = pPlanes->GetRect(_T("updown"));
	Rects.ItemTag = pPlanes->GetRect(_T("itemtag"));
	Rects.Item = pPlanes->GetRect(_T("item"));
	Rects.SkillArea = pPlanes->GetRect(_T("skillarea"));
	Rects.Skill = Cache.Skill.GetBaseRect();

	const MemberStatus& Refmem = pStatus->MemberState();

	DrawStr( System, Cache.Pason.Get(Refmem.PasonName).Name, Rects.Name );
	DrawStr( System, Cache.Job.Get(Refmem.JobName).Name, Rects.Job );

	const String& News = RbMember(Refmem).PasonData().News;
	if( News.empty() )Rects.NewsBtn.ReSize(0,0);
	else Surfase.DrawPlane( _T("chnew"), Rects.NewsBtn.left, Rects.NewsBtn.top );

	RbRect Rect = pPlanes->GetRect(_T("Level"));
//	DrawStr( System, _T("Level"), Rects.Tag+Rect.LT() );
	System.Font().Draw( TagFont, _T("Level"), Rects.Tag+Rect.LT(), DT_CENTER );
	DrawStr( System, String().Format(_T("%d"),Refmem.Level), Rects.Value+Rect.LT() );
	DrawValue( System, pPlanes, _T("HitPoint") );
	DrawValue( System, pPlanes, _T("MagicPoint") );
	DrawValue( System, pPlanes, _T("Attack") );
	DrawValue( System, pPlanes, _T("Defence") );
	DrawValue( System, pPlanes, _T("Magic") );
	DrawValue( System, pPlanes, _T("Speed") );
	DrawItem( System, pPlanes, _T("WEP"), Refmem.Wepon.Number );
	DrawItem( System, pPlanes, _T("BODY"), Refmem.Body.Number );
	DrawItem( System, pPlanes, _T("ACS"), Refmem.Acs.Number );

	DrawSkills( Surfase );

	return S_OK;
}

void StatusWindow::DrawValue( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name ) const
{
	if( pPlanes == NULL )return;
	if( pStatus == NULL )return;
	RbRect AllRect = pPlanes->GetRect(Name);
	if( AllRect.IsNone() )return;
	RbPoint LT=AllRect.LT();

	System.Font().Draw( TagFont, Name, Rects.Tag+LT, DT_CENTER );

	System.Font().Draw( NumFont, _T("/"), Rects.Slash+LT, DT_CENTER|DT_VCENTER );
	System.Font().Draw( MinNumFont, _T("+"), Rects.Plus+LT, DT_CENTER|DT_VCENTER );

	DrawFloat( System, NumFont, GetValue(pStatus->NowState(),Name), Rects.Now+LT, DT_RIGHT|DT_VCENTER );
	DrawFloat( System, NumFont, GetValue(pStatus->MemberState().Status,Name), Rects.Source+LT, DT_LEFT|DT_VCENTER );
	DrawFloat( System, MinNumFont, GetValue(pStatus->LAppend(),Name), Rects.Leader+LT, DT_LEFT|DT_VCENTER );
}

void StatusWindow::DrawFloat( WindowSystem& System, LPCTSTR Font, float Value, const RbRect& Rect, DWORD Style ) const
{
	System.Font().Draw( Font, String().Format(_T("%.0f"), Value), Rect, Style );
}

void StatusWindow::DrawItem( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name, long ItemNum ) const
{
	const ItemStatus& Item = GetCache().Item.Get(ItemNum);
	RbPoint Pos = RbRect(pPlanes->GetRect(Name)).LT();
	DrawStr( System, Name, Rects.ItemTag+Pos );
	DrawStr( System, Item.Name, Rects.Item+Pos );
}

void StatusWindow::DrawStr( WindowSystem& System, LPCTSTR Msg, const RbRect& Rect ) const
{
	System.Font().Draw( StatusFont, Msg, Rect, DT_CENTER );
}

RbRect StatusWindow::GetSkillRect( long Index ) const
{
	if( Index >= Rects.SkillCount )return RbRect();
	float Width = Rects.Skill.Width();
	if( Rects.SkillCount > 1 )
	{
		if( Width*Rects.SkillCount > Rects.SkillArea.Width() )
		{
			Width = (Rects.SkillArea.Width()-Width)/(Rects.SkillCount-1);
		}
	}
	RbRect Rect = Rects.Skill;
	Rect+=Rects.SkillArea.LT();
	Rect.Move( Index*Width, 0 );
	return Rect.Clip(Rects.SkillArea);
}

void StatusWindow::DrawSkills( RbSprite& Surfase )
{
	if( pStatus == NULL )return;
	Rects.SkillCount = pStatus->SkillList.AllSkillCount();
	for( int i = Rects.SkillCount-1; i >= 0; i-- )
	{
		const RbRect& Rect = GetSkillRect(i);
		if( Rect.IsNone() )continue;
		RbPoint SkillPos = Rect.LT();
		EquipSkill::SkillDef Def = pStatus->SkillList.Get( i );
		Surfase.DrawSkill( Def.ID, Def.Level, SkillPos.x, SkillPos.y );
	}
}

RbRect StatusWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( Rects.NewsBtn.Pin(x,y) )return Rects.NewsBtn;
	if( Rects.Name.Pin(x,y) )return Rects.Name;
	if( Rects.Job.Pin(x,y) )return Rects.Job;

	if( Rects.Wepon.Pin(x,y) )return Rects.Wepon;
	if( Rects.Body.Pin(x,y) )return Rects.Body;
	if( Rects.Acs.Pin(x,y) )return Rects.Acs;

	if( Rects.SkillArea.Pin(x,y) )
	{
		for( int i = 0; i < Rects.SkillCount; i++ )
		{
			RbRect Rect = GetSkillRect(i);
			if( Rect.Pin(x,y) )return Rect;
		}
	}
	return RbRect();
}

bool StatusWindow::Floating( int x, int y )
{
	RbWindowSystem& System = GetWindowSystem();
	RbCache& Cache = GetCache();
	if( pStatus == NULL )return false;

	const MemberStatus& Status = pStatus->MemberState();
	if( Rects.Name.Pin(x,y) )
	{
		const PasonCache::PasonST& Ref = Cache.Pason.Get( Status.PasonName );
		return System.Help( Ref.Help, Ref.Name, ToGlobalRect(Rects.Name) );
	}
	if( Rects.Job.Pin(x,y) )
	{
		const JobStatus& Ref = Cache.Job.Get( Status.JobName );
		return System.Help( Ref.Help, Ref.Name, ToGlobalRect(Rects.Job) );
	}

	if( Rects.Wepon.Pin(x,y) )return System.HelpItem(Status.Wepon,ToGlobalRect(Rects.Wepon));
	if( Rects.Body.Pin(x,y) )return System.HelpItem(Status.Body,ToGlobalRect(Rects.Body));
	if( Rects.Acs.Pin(x,y) )return System.HelpItem(Status.Acs,ToGlobalRect(Rects.Acs));

	if( Rects.SkillArea.Pin(x,y) )
	{
		for( int i = 0; i < Rects.SkillCount; ++i )
		{
			if( GetSkillRect(i).Pin(x,y) )
				return System.HelpSkill(pStatus->SkillList.Get( i ).ID, ToGlobalRect(Rects.SkillArea));
		}
	}
	return false;
}

bool StatusWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eScrollUp:
		if( bEquipMode || !bDestroyOK )break;
		{
			RefPtr<MiniStatusWindow> pWnd(new MiniStatusWindow);
			if( !CopyStatusWindow( System, pWnd ) )break;
			Close( System );
		}
		return true;
	}
	return StatusBaseWinow::MouseJob( System, State );
}

bool StatusWindow::ClickJob( int x, int y )
{
	if( pStatus == NULL )return false;
	if( Rects.Name.Pin(x,y) || Rects.NewsBtn.Pin(x,y) )
	{
		const String& News = RbMember(pStatus->MemberState()).PasonData().News;
		if(!News.empty())
		{
			String Com = _T("window/show/News/");
			Com += News;
			StrCommander().Command(Com);
		}
		return true;
	}
	if( bEquipMode )return StatusBaseWinow::ClickJob(x,y);
	if( !ImageArea.Pin(x,y) )return StatusBaseWinow::ClickJob(x,y);
	SetViewOn();
	return true;
}
