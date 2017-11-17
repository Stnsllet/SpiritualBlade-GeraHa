// HelpWindow.cpp: HelpWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "GameSystem.h"
#include "RbWindowSystem.h"
#include "HelpWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LPCTSTR HelpWindow::HelpFont = _T("default");
LPCTSTR HelpWindow::InfoFont = _T("info");

HelpWindow::HelpWindow()
{
	bDestroyOK = false;
	bHide = true;
}

HelpWindow::~HelpWindow()
{

}

LPCTSTR HelpWindow::WindowName() const
{
	return _T("Help");
}

bool HelpWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	if( State == eRDblClick )bHide = true;
	if( bHide )return false;
	StartTime = RbTimer();
	return BaseWindow::MouseJob( System, State );
}

HRESULT HelpWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	RbTimer NewTime;
	const GameMessage& Msg = g_pSystem->RefMessage();
	if( HelpSourceRect.Pin( Msg.x, Msg.y ) )StartTime = NewTime;
	if( NewTime.Passed( StartTime ) > 1000 )bHide = true;
	return BaseWindow::Draw( Display ,System );
}
// レストア
HRESULT HelpWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	RbRect HelpRect = System.Font().GetTextRect( HelpFont, HelpString );
	RbRect InfoRect, AllRect = HelpRect;
	if( !InfoString.empty() )
	{
		InfoRect = System.Font().GetTextRect( InfoFont, InfoString );
		HelpRect.Move( 0, InfoRect.Height()+3 );
		AllRect = HelpRect;
		AllRect.OR( InfoRect );
	}
	RbRect Skill;
	if( SkillList.size() > 0 )
	{
		SkillRect = GetCache().Skill.GetBaseRect();
		Skill += AllRect.LB();
		Skill.ReSize( SkillList.size()*SkillRect.Width(), SkillRect.Height() );
		AllRect.OR( Skill );
	}
	SetStringSize( System, AllRect );
	InfoRect += AllRect.LT();
	HelpRect += AllRect.LT();
	Skill += AllRect.LT();

	ExSprite Surface;
	RestoreBase( pDevice, Surface, System );

	for( int i = 0; i < SkillList.size(); i++ )
	{
		RbPoint Pos = Skill.LT();
		Pos.x += SkillRect.Width()*i;
		Surface.DrawSkill( SkillList[i].ID, SkillList[i].Level, Pos.x, Pos.y );
	}
	DrawWindowTag( System, WindowName() );

	System.Font().Draw( HelpFont, HelpString, HelpRect );
	if( !InfoString.empty() )System.Font().Draw( InfoFont, InfoString, InfoRect );

	return S_OK;
}

bool HelpWindow::SetHelp( LPCTSTR Infomation, LPCTSTR Help )
{
	if( Help == NULL )return false;
	bHide = false;
	StartTime = RbTimer();
	if( HelpString == Help )
	{
		if( Infomation == NULL && InfoString.empty() )return false;
		if( InfoString == Infomation )return false;
	}
	GetWindowSystem().MoveTop( this );
	InfoString = Infomation;
	HelpString = Help;
	SkillList.clear();

	TexWindow.Release();
	return true;
}

bool HelpWindow::AddSkill( LPCTSTR Name, long Level )
{
	long SkillID = GetCache().Skill.GetSkillNo( Name );
	return AddSkill( SkillID, Level );
}

bool HelpWindow::AddSkill( long SkillID, long Level )
{
	if( SkillID <= 0 )return false;
	SkillList.push_back( SkillDef( SkillID, Level ) );
	return true;
}

void HelpWindow::SetStringSize( WindowSystem& System, RbRect& TextRect )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return;

	const TexRect& Base = pPlanes->Base.Plane;
	RbRect Msg = pPlanes->GetRect(_T("msg"));

	Position.Width = TextRect.Width() - Msg.Width() + Base.w;
	Position.Height = TextRect.Height() - Msg.Height() + Base.h;

	TextRect.Move( Msg.left, Msg.top );
}
