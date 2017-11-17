// TobiWhiteNoise.h: TobiWhiteNoise クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWHITENOISE_H__EAEC2467_C0AA_422C_BE99_BC4522B3BF71__INCLUDED_)
#define AFX_TOBIWHITENOISE_H__EAEC2467_C0AA_422C_BE99_BC4522B3BF71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFlare.h"

class TobiWhiteNoise : public TobiFlare  
{
public:
	TobiWhiteNoise();
	virtual ~TobiWhiteNoise();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIWHITENOISE_H__EAEC2467_C0AA_422C_BE99_BC4522B3BF71__INCLUDED_)
