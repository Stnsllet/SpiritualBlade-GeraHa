// SPWindow.cpp: SPWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbSprite.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "RbWindowSystem.h"
#include "SPWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SPWindow::SPWindow()
{
	bDestroyOK = bMoveOK = false;
	bOutView = true;
	FloatPos.bLock = true;
	SkillCount = 0;
	CheckCount = CheckLevel = 0;
}

SPWindow::~SPWindow()
{

}

LPCTSTR SPWindow::WindowName() const
{
	return _T("SpecialAttack");
}


bool SPWindow::SetCharactor( RbActStatus* pNewStatus )
{
	if( pNewStatus == NULL )return false;
	pStatus = pNewStatus;
	return true;
}

HRESULT SPWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( GetBattleSystem().GetSelectCharactor() != pStatus )
	{
		bHide = false;
		TexWindow.Release();
	}
	if( pStatus != NULL )
	{
		if( CheckLevel != pStatus->Level() )TexWindow.Release();
		if( CheckCount != pStatus->UsedSkillCount() )TexWindow.Release();
	}
	return BaseWindow::Draw( Display, System );
}

RbRect SPWindow::GetRect( long Index ) const
{
	return RbRect( ImgPlane.x, ImgPlane.y+Index*ImgPlane.h,
		NamePlane.x+NamePlane.w-ImgPlane.x, ImgPlane.h );
}

long SPWindow::HitSkill( int x, int y ) const
{
	for( long i = 0; i < SkillCount; i++ )
	{
		if( GetRect( i ).Pin( x, y ) )return i;
	}
	return -1;
}

RbRect SPWindow::HitCursor( WindowSystem& System, int x, int y )
{
	long SkillIndex = HitSkill( x, y );
	if( SkillIndex < 0 )return RbRect();
	return GetRect( SkillIndex );
}

// レストア
HRESULT SPWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	SkillCount = 0;
	bHide = true;

	const WindowPlanes* pSelPlanes = System.Define().GetWindowPlanes(_T("SpecialAttack"));
	if( pSelPlanes == NULL )return E_FAIL;
	FloatPos.sy = -pSelPlanes->Base.Plane.h;

	pStatus = GetBattleSystem().GetSelectCharactor();
	if( pStatus == NULL )return S_FALSE;
	SkillCount = pStatus->Level();
	CheckCount = pStatus->UsedSkillCount();

	const WindowPlane* pImgPlane = pSelPlanes->Find(_T("image"));
	const WindowPlane* pNamePlane = pSelPlanes->Find(_T("name"));
	if( pImgPlane == NULL || pNamePlane == NULL )return E_FAIL;
	ImgPlane = pImgPlane->Plane;
	NamePlane = pNamePlane->Plane;

	if( pStatus != NULL )SkillCount = pStatus->SkillList.Action.GetCount();
	if( SkillCount <= 0 )return S_FALSE;

	Position.Width = pSelPlanes->Base.Plane.w;
	Position.Height = ImgPlane.h * SkillCount + 8+ ImgPlane.y;
	SetFloatPosition( System, FloatPos.x, FloatPos.y );

	ExSprite Surfase;
	RestoreBase( pDevice, Surfase, System );

	DrawWindowTag( System, WindowName(), ImgPlane.x+ImgPlane.w );

	for( int i = 0; i< SkillCount; i++ )
	{
		EquipSkill::SkillDef Def = pStatus->SkillList.Action.Get( i );
		bool bUsed = pStatus->IsUsedSkill(Def.ID);
		DrawSkill( Surfase, System, Def.ID, Def.Level, bUsed, i );
	}

	bHide = false;
	return S_OK;
}

HRESULT SPWindow::DrawSkill( RbSprite& Surface, WindowSystem& System, long ID, long level, bool bUsed, long Line )
{
	const float SkillSize = ImgPlane.h;
	float left = ImgPlane.x, right = NamePlane.x+NamePlane.w;
	float x = left;
	float y = ImgPlane.y + Line * SkillSize;
	HRESULT hr = Surface.DrawSkill( ID, level, x, y );
	if( FAILED(hr) )return hr;

	const SkillCache::SkillData* pData = GetCache().Skill.GetSkill( ID );
	if( pData == NULL )return E_FAIL;

	RbRect MsgRect( NamePlane );
	MsgRect.Move( 0, Line * SkillSize );

	hr = System.Font().Draw( "default", pData->Name, MsgRect, DT_LEFT|DT_VCENTER );
	if( !bUsed )return hr;

	return Surface.DrawBreak( RbRect(left, y, right-left, SkillSize) );
}

bool SPWindow::Floating( int x, int y )
{
	if( pStatus == NULL )return false;
	long SkillIndex = HitSkill( x, y );
	EquipSkill::SkillDef Def = pStatus->SkillList.Action.Get( SkillIndex );
	GetWindowSystem().HelpSkill( Def.ID, ToGlobalRect(GetRect(SkillIndex)) );
	return true;
}

bool SPWindow::ClickJob( int x, int y )
{
	if( pStatus == NULL )return false;
	long SkillIndex = HitSkill( x, y );
	if( SkillIndex < 0 )return false;
	EquipSkill::SkillDef Def = pStatus->SkillList.Action.Get( SkillIndex );
	if( Def.ID <= 0 )return false;
	bool bExec = pStatus->SpecialGo( Def.ID );
	if( bExec )
	{
		TexWindow.Release();
		GetSoundSystem().Play(_T("click"));
	}
	return bExec;
}
