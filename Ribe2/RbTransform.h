// RbTransform.h: RbTransform クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBTRANSFORM_H__6F9F30BD_69AD_4914_ACFF_897507B4118B__INCLUDED_)
#define AFX_RBTRANSFORM_H__6F9F30BD_69AD_4914_ACFF_897507B4118B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RbPosition.h"

class RbTransform : public D3DXMATRIX
{
public:
	RbTransform();
	RbTransform( const D3DMATRIX& Source ):D3DXMATRIX(Source){}
	~RbTransform();
	RbPosition Trans( const D3DVECTOR& TransSource ) const;

	RbTransform& Scale( float AddScale );
	RbTransform& Scale( float x, float y, float z );
	RbTransform& AddScroll( float x, float y, float z=0 );
	RbTransform& AddScroll( const D3DVECTOR& Scroll ){return AddScroll(Scroll.x, Scroll.y, Scroll.z);}
	RbTransform& FromPosture( const D3DVECTOR& NewPosture );
	RbTransform& Posture( const D3DVECTOR& AddPosture );
	RbTransform& RotateX( float angle );
	RbTransform& RotateY( float angle );
	RbTransform& RotateZ( float angle );
	RbTransform& Mirror( bool x, bool y=false, bool z=false );
	RbTransform& Inverse();

	RbTransform GetInverse() const;
	RbPosition GetScrollPos() const;
};

#endif // !defined(AFX_RBTRANSFORM_H__6F9F30BD_69AD_4914_ACFF_897507B4118B__INCLUDED_)
