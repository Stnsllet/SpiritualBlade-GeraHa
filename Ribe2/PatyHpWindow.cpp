// PatyHpWindow.cpp: PatyHpWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "BattleSystem.h"
#include "RbBattlePaty.h"
#include "RbWindowSystem.h"
#include "PatyHpWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PatyHpWindow::PatyHpWindow()
{
	FloatPos.bLock = true;
	bCursorSound = bDestroyOK = bMoveOK = false;
	bOutView = true;
}

PatyHpWindow::~PatyHpWindow()
{
}

LPCTSTR PatyHpWindow::WindowName() const
{
	return _T("PatyHp");
}

bool PatyHpWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt != VT_I4 )return false;
	long PatyID = Value.lVal;

	std::list<PatyHp>::const_iterator itr = Patys.begin();
	while( itr != Patys.end() )
	{
		const PatyHp& Ref = *itr++;
		if( Ref.ID == PatyID )return true;
	}
	return false;
}

bool PatyHpWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_I4 )return false;
	long PatyID = Value.lVal;

	RefPtr<RbBattlePaty> pPaty = GetBattleSystem().GetPaty( PatyID );
	if( pPaty == NULL )return false;

	return AddPaty( pPaty );
}

bool PatyHpWindow::AddPaty( RbBattlePaty* pPaty )
{
	if( pPaty == NULL )return false;
	if( CheckParameter( pPaty->ID() ) )return false;

	PatyHp NewPaty;
	if( !NewPaty.SetPaty( pPaty ) )return false;
	Patys.push_back( NewPaty );
	TexWindow.Release();
	return true;
}

bool PatyHpWindow::PatyHp::MemberHp::SetMember( RbActStatus* pMem )
{
	if( pMem == NULL )return false;
	pStatus = pMem;
	return true;
}

bool PatyHpWindow::PatyHp::SetPaty( RbBattlePaty* pPaty )
{
	if( pPaty == NULL )return false;
	long Count = pPaty->MemberCount();
	if( Count <= 0 )return false;
	ID = pPaty->ID();

	Members.reserve( Count );
	for( long i = 0; i < Count; i++ )
	{
		PatyHpWindow::PatyHp::MemberHp NewHp;
		if( NewHp.SetMember( pPaty->GetMember(i) ) )
			Members.push_back( NewHp );
	}

	return true;
}

HRESULT PatyHpWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	long PatyCount = Patys.size();

	const DefaultPlanes& Cache = GetCache().SystemPlane;
	Rects.HpPlane = Cache.GetPlane(_T("p_hp"));
	Rects.EHpPlane = Cache.GetPlane(_T("p_ehp"));
	Rects.MpPlane = Cache.GetPlane(_T("p_mp"));
	Rects.HMpPlane = Cache.GetPlane(_T("p_hmp"));
	Rects.SPPlane = Cache.GetPlane(_T("p_star"));
	Rects.DeadPlane = Cache.GetPlane(_T("dead"));

	Rects.Patys = pPlanes->GetRect(_T("paty"));
	Rects.Image = pPlanes->GetRect(_T("image"));
	Rects.Hmp = pPlanes->GetRect(_T("hmp"));
	Rects.Hp = pPlanes->GetRect(_T("hp"));
	Rects.Mp = pPlanes->GetRect(_T("mp"));
	Rects.SP = pPlanes->GetRect(_T("sp"));

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h
		+ (PatyCount*Rects.Patys.Height());

	ExSprite Surface;
	RestoreBase( pDevice, Surface, System );

	DrawWindowTag( System, _T("Patys") );

	std::list<PatyHp>::const_iterator itr = Patys.begin();
	RbRect DrawArea = Rects.Patys;
	while( itr != Patys.end() )
	{
		const PatyHp& Ref = *itr++;
		RbRect ImageRect = Rects.Image;
		ImageRect.Move( DrawArea.left, DrawArea.top );
		Ref.DrawImage( Surface, ImageRect );
		Ref.DrawBox( Surface, Rects, DrawArea );
		DrawArea.BoxMove( 0, 1 );
	}
	return S_OK;
}

HRESULT PatyHpWindow::PatyHp::DrawImage( RbSprite& Surface, const RbRect& Rect ) const
{
	if( Members.size() <= 0 )return S_FALSE;
	if( Members[0].pStatus == NULL )return S_FALSE;
	RefPtr<RbTexture> pTexture = Members[0].pStatus->RefTexture();
	if( pTexture == NULL )return E_PENDING;

	RbPoint DrawPos( Rect.Center().x, Rect.bottom );
	float Hight = pTexture->GetHight();
	float Size = pTexture->GetSize();
	float Scale = 1.0f;
	if( Hight>0 )Scale = Rect.Height()/Hight;
	if( Size>0 )Scale = min(Scale, Rect.Width()/Size );

	const RbTexture::ActTime& TexAct = pTexture->GetAction(_T("image"), 1.25f, 0 );

	return Surface.DrawRbTexture( TexAct, DrawPos.x, DrawPos.y, 0, Scale );
}

HRESULT PatyHpWindow::PatyHp::DrawBox( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect ) const
{
	RbRect BoxRect = Rects.Hmp;
	BoxRect.Move( Rect.left, Rect.top );
	long Count = Members.size();
	for( int i = 0; i < Count; i++ )
	{
		Surface.DrawPlane( Rects.HMpPlane, BoxRect.left, BoxRect.top );
		BoxRect.BoxMove( 1, 0 );
	}
	return S_OK;
}

HRESULT PatyHpWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	HRESULT hr = BaseWindow::Draw( Display, System );
	if( FAILED(hr) )return hr;

	std::list<PatyHp>::iterator itr = Patys.begin();
	RbRect DrawArea = Rects.Patys;
	DrawArea.Move( Position.x, Position.y );
	while( itr != Patys.end() )
	{
		PatyHp& Ref = *itr++;
		Ref.DrawHp( Display, Rects, DrawArea );
		DrawArea.BoxMove( 0, 1 );
	}

	return S_OK;
}

HRESULT PatyHpWindow::PatyHp::DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect )
{
	long Count = Members.size();
	RbRect HmpRect = Rects.Hmp;
	HmpRect.Move( Rect.left, Rect.top );
	for( int i = 0; i < Count; i++ )
	{
		Members[i].DrawHp( Surface, Rects, HmpRect );
		HmpRect.BoxMove( 1, 0 );
	}
	return S_OK;
}

HRESULT PatyHpWindow::PatyHp::MemberHp::DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect )
{
	if( pStatus == NULL )return E_PENDING;
	if( pStatus->IsDead() )
	{
		Surface.DrawPlane( Rects.DeadPlane, Rect.left, Rect.top );
		return S_OK;
	}
	const DefaultStatus& Now = pStatus->NowState();
	const DefaultStatus& Max = pStatus->AllState();
	float HpHight = Rects.HpPlane.h;
	if( Max.Hp > 0 )HpHight *= Now.Hp/Max.Hp;
	float MpHight = Rects.MpPlane.h;
	if( Max.Mp > 0 )MpHight *= Now.Mp/Max.Mp;
	return DrawHp( Surface, Rects, Rect, HpHight, MpHight );
}

HRESULT PatyHpWindow::PatyHp::MemberHp::DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect, long HpHight, long MpHight )
{
	Plane<> HpPlane = Rects.HpPlane;
	if( pStatus->IsEscapeHp() )HpPlane = Rects.EHpPlane;

	long ypos = Rects.HpPlane.h - HpHight;
	HpPlane.h = HpHight;
	HpPlane.y += ypos;

	float x = Rect.left + Rects.Hp.left;
	float y = Rect.top + Rects.Hp.top+ypos;

	Surface.DrawPlane( HpPlane, x, y );

	Plane<> MpPlane = Rects.MpPlane;

	ypos = Rects.MpPlane.h - MpHight;
	MpPlane.h = MpHight;
	MpPlane.y += ypos;

	x = Rect.left + Rects.Mp.left;
	y = Rect.top + Rects.Mp.top+ypos;

	Surface.DrawPlane( MpPlane, x, y );
	if( pStatus->RefBattleStatus().bMoreSpecial )
	{
		x = Rect.left + Rects.SP.left;
		y = Rect.top + Rects.SP.top;
		Surface.DrawPlane( Rects.SPPlane, x, y );
	}
	return S_OK;
}

RbRect PatyHpWindow::HitCursor( WindowSystem& System, int x, int y )
{
	std::list<PatyHp>::iterator itr = Patys.begin();
	RbRect PatyArea = Rects.Patys;
	while( itr != Patys.end() )
	{
		PatyHp& Ref = *itr++;
		if( PatyArea.Pin( x, y ) )
		{
			RbRect MemberArea = Rects.Hmp;
			MemberArea.Move( PatyArea.left, PatyArea.top );
			long Count = Ref.Members.size();
			for( int i = 0; i < Count; i++ )
			{
				if( MemberArea.Pin(x,y) )return MemberArea;
				MemberArea.BoxMove(1,0);
			}
		}
		PatyArea.BoxMove( 0, 1 );
	}
	return RbRect();
}

bool PatyHpWindow::ClickJob( int x, int y )
{
	std::list<PatyHp>::iterator itr = Patys.begin();
	RbRect PatyArea = Rects.Patys;
	while( itr != Patys.end() )
	{
		PatyHp& Ref = *itr++;
		if( PatyArea.Pin( x, y ) )
		{
			long Count = Ref.Members.size();
			if( Count <= 0 )continue;

			if( Rects.Image.Shift(PatyArea.left,PatyArea.top).Pin(x,y) )
				return ClickMember( Ref.Members[0] );

			RbRect MemberArea = Rects.Hmp;
			MemberArea.Move( PatyArea.left, PatyArea.top );
			for( int i = 0; i < Count; i++ )
			{
				if( MemberArea.Pin(x,y) )
					return ClickMember( Ref.Members[i] );
				MemberArea.BoxMove(1,0);
			}
		}
		PatyArea.BoxMove( 0, 1 );
	}
	return true;
}

bool PatyHpWindow::ClickMember( const PatyHp::MemberHp& RefMember )
{
	BattleSystem& System = GetBattleSystem();
	System.SetSelectCharactor( RefMember.pStatus );
	System.SetViewOn( RefMember.pStatus );
	return true;
}
