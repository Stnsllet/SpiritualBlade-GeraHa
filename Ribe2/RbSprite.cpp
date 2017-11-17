// RbSprite.cpp: RbSprite クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbRect.h"
#include "RbMember.h"
#include "RbCache.h"
#include "RbTransform.h"
#include "RbSprite.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbSprite::RbSprite()
{

}

RbSprite::~RbSprite()
{

}

HRESULT RbSprite::DrawPlane( LPCTSTR name, float x, float y )
{
	return DrawPlane( GetCache().SystemPlane.GetPlane( name ), x, y );
}

HRESULT RbSprite::DrawPlane( const Plane<>& SourcePlane, float x, float y )
{
	RbRect Rect( SourcePlane );
	return Draw( SourcePlane, x, y, Rect );
}

HRESULT RbSprite::DrawPlane( const Plane<HandTexRect>& SourcePlane, float x, float y, float Scale )
{
	RbRect Rect( SourcePlane );
	RbTransform Trans;
	Trans.AddScroll( -Rect.Width()/2, -Rect.Height() );
	Trans.Mirror( SourcePlane.w<0,  SourcePlane.h<0 );
	Trans.RotateZ( SourcePlane.angle );
	Trans.AddScroll( SourcePlane.px, SourcePlane.py );
	Trans.Scale( Scale );
	Trans.AddScroll( x, y );

	return Draw( SourcePlane, Trans, Rect );
}

HRESULT RbSprite::DrawRbImage( const MemberStatus& State, const RECT& Rect, bool bInRect )
{
	const RbMember DrawStatus(State);
	return DrawRbImage( DrawStatus.GetTexture(), Rect, bInRect );
}

HRESULT RbSprite::DrawRbImage( LPCTSTR TexName, const RECT& Rect, bool bInRect )
{
	RbCache::TexPtr pTexture;
	GetCache().GetTexture( TexName, pTexture );
	if( pTexture == NULL )return E_FAIL;
	RbPoint Pos( (Rect.left+Rect.right)/2, Rect.bottom );
	float Scale = 1.0f;
	if( bInRect )
	{
		long Width = Rect.right-Rect.left;
		long Height = Rect.bottom-Rect.top;
		float Size = pTexture->GetSize();
		if( Size > 0 && Width < Size )Scale = Width/Size;
		Size = pTexture->GetHight();
		if( Size > 0 && Height < Size )
			Scale = min(Scale, Height/Size );
	}

	return DrawRbTexture( pTexture, Pos.x, Pos.y, _T("image"), 1.25, 0, 0, Scale );
}

HRESULT RbSprite::DrawRbTexture( const MemberStatus& State, float x, float y, LPCTSTR ActName, float Direct, DWORD Time, float Scale )
{
	const RbMember DrawStatus(State);
	return DrawRbTexture( DrawStatus.GetTexture(), x, y, ActName, Direct, Time, NULL, Scale );
}

HRESULT RbSprite::DrawRbTexture( LPCTSTR name, float x, float y, LPCTSTR ActName, float Direct, DWORD Time, RefBord* ItemImage, float Scale )
{
	RbCache::TexPtr pTexture;
	GetCache().GetTexture( name, pTexture );
	return DrawRbTexture( pTexture, x, y, ActName, Direct, Time, ItemImage, Scale );
}

HRESULT RbSprite::DrawRbTexture( RbTexture* pTexture, float x, float y, LPCTSTR ActName, float Direct, DWORD Time, RefBord* ItemImage, float Scale )
{
	if( pTexture == NULL )return E_POINTER;
	const RbTexture::ActTime& ActionTexture = pTexture->GetAction( ActName, Direct, Time );

	return DrawRbTexture( ActionTexture, x, y, ItemImage, Scale );
}

HRESULT RbSprite::DrawRbTexture( const RbTexture::ActTime& Image, float x, float y, RefBord* ItemImage, float Scale )
{
	std::vector<RbTexture::TexturePlane>::const_iterator itr = Image.PlaneList.begin();
	for( ;itr != Image.PlaneList.end(); itr++ )
	{
		const RbTexture::TexturePlane& TexPlane = *itr;
		Plane<HandTexRect> DrawData;
		if( TexPlane.bItem )
		{
			float dx = x + TexPlane.Rect.px*Scale;
			float dy = y + TexPlane.Rect.py*Scale;
			DrawItem( ItemImage, dx, dy, TexPlane.Rect, Scale );
			continue;
		}
		else
		{
			DrawData = TexPlane.Rect;
			DrawData.pTexture = TexPlane;
		}
		DrawPlane( DrawData, x, y, Scale );
	}
	return S_OK;
}

HRESULT RbSprite::DrawItem( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale )
{
	if( Is3D() )return DrawItem3D( ItemImage, x, y, TagetRect, Scale );
	return DrawItem2D( ItemImage, x, y, TagetRect, Scale );
}

HRESULT RbSprite::DrawItem2D( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale )
{
	if( ItemImage == NULL )return S_FALSE;
	const HandTexRect& ItemPlane = ItemImage->SourcePlane;

	RbRect Rect( ItemPlane );
	RbTransform Trans;
	Trans.AddScroll( -Rect.Width()/2, -Rect.Height() );
	Trans.AddScroll( ItemPlane.px, ItemPlane.py );
	Trans.Scale( Scale );
	Trans.Mirror( TagetRect.w<0,  TagetRect.h<0 );
	Trans.RotateZ( TagetRect.angle );
	Trans.AddScroll( x, y );

	Draw( ItemImage->pTexture, Trans, Rect );
	return S_OK;
}

HRESULT RbSprite::DrawItem3D( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale )
{
	if( ItemImage == NULL )return S_FALSE;

	RbTransform Trans;
	Trans.Scale( Scale );
	Trans.Mirror( TagetRect.w<0,  TagetRect.h<0 );
	Trans.RotateZ( TagetRect.angle );
	Trans.AddScroll( x, y );

	BillbordPlane Bord = *ItemImage;
	Bord.Trans( Trans );
	return Bord.Draw( MyStock.pDevice );
//	Draw( ItemImage->pTexture, Trans, Rect );
	return S_OK;
}

HRESULT RbSprite::DrawSkill( LPCTSTR SkillName, long level, float x, float y)
{
	return DrawSkill( GetCache().Skill.GetSkillNo(SkillName), level, x, y );
}

HRESULT RbSprite::DrawSkill( long SkillNo, long level, float x, float y)
{
	const SkillCache& Cache = GetCache().Skill;
	const SkillCache::SkillData *pData = Cache.GetSkill( SkillNo );
	if( pData == NULL )return S_FALSE;

	if( level > SkillCache::MaxLevel )level = SkillCache::MaxLevel;

	Plane<> BasePlane = Cache.GetBasePlane( pData->Type );

	DrawPlane( BasePlane, x, y);

	DrawPlane( pData->Pos, x, y );

	Plane<> LevPlane = Cache.GetLevelPlane(level);

	DrawPlane( LevPlane, x+BasePlane.w-LevPlane.w, y+BasePlane.h-LevPlane.h);

	return S_OK;
}

HRESULT RbSprite::DrawBreak( const RECT& Rect )
{
	Plane<> Break = GetCache().SystemPlane.GetPlane("break");
	RbRect SorceRect( Break );
	return Draw( Break, SorceRect, Rect );
}

HRESULT RbSprite::DrawCursor( const RECT& Rect )
{
	Plane<> Cur = GetCache().SystemPlane.GetPlane("cursor");
	RbRect SorceRect( Cur );
	return Draw( Cur, SorceRect, Rect );
}
