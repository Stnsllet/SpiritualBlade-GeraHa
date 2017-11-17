// RbSprite.h: RbSprite クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBSPRITE_H__18542B91_9A26_464B_B0C8_6B1280BC7F01__INCLUDED_)
#define AFX_RBSPRITE_H__18542B91_9A26_464B_B0C8_6B1280BC7F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Plane.h"
#include "Sprite.h"
#include "Sprite2.h"
#include "RbTexture.h"
#include "BillbordPlane.h"

class RbTexture;
struct RbTexture::ActTime;

class RbSprite : public Sprite2
{
public:
	RbSprite();
	virtual ~RbSprite();

	HRESULT DrawPlane( LPCTSTR name, float x, float y );
	HRESULT DrawPlane( const Plane<>& SourcePlane, float x, float y );
	HRESULT DrawPlane( const Plane<HandTexRect>& SourcePlane, float x, float y, float Scale=1 );

	HRESULT DrawBreak( const RECT& Rect );
	HRESULT DrawCursor( const RECT& Rect );

	HRESULT DrawRbImage( LPCTSTR TexName, const RECT& Rect, bool bInRect=false );
	HRESULT DrawRbImage( const MemberStatus& State, const RECT& Rect, bool bInRect=false );
	HRESULT DrawRbTexture( const MemberStatus& State, float x, float y, LPCTSTR ActName, float Direct=0, DWORD Time=0, float Scale=1 );
	HRESULT DrawRbTexture( LPCTSTR name, float x, float y, LPCTSTR ActName, float Direct=0, DWORD Time=0, RefBord* ItemImage=NULL, float Scale=1 );
	HRESULT DrawRbTexture( RbTexture* pTexture, float x, float y, LPCTSTR ActName, float Direct=0, DWORD Time=0, RefBord* ItemImage=NULL, float Scale=1 );
	HRESULT DrawRbTexture( const RbTexture::ActTime& Image, float x, float y, RefBord* ItemImage=NULL, float Scale=1 );
	HRESULT DrawItem( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale=1 );

	HRESULT DrawSkill( LPCTSTR SkillName, long level, float x, float y);
	HRESULT DrawSkill( long SkillNo, long level, float x, float y);

protected:
	HRESULT DrawItem3D( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale );
	HRESULT DrawItem2D( RefBord* ItemImage, float x, float y, const HandTexRect& TagetRect, float Scale );

};

#endif // !defined(AFX_RBSPRITE_H__18542B91_9A26_464B_B0C8_6B1280BC7F01__INCLUDED_)
