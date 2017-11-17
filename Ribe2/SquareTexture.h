// SquareTexture.h: SquareTexture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQUARETEXTURE_H__736362FA_BB51_47B2_A827_DCD5544A7458__INCLUDED_)
#define AFX_SQUARETEXTURE_H__736362FA_BB51_47B2_A827_DCD5544A7458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Texture.h"

class SquareTexture : public Texture
{
public:
	SquareTexture();
	virtual ~SquareTexture();

	HRESULT LoadSquare( DISPDEVICE* pDevice, LPCTSTR pFileName );
};

#endif // !defined(AFX_SQUARETEXTURE_H__736362FA_BB51_47B2_A827_DCD5544A7458__INCLUDED_)
