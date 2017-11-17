// TobiDamege.h: TobiDamege クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDAMEGE_H__B100BCC0_F4D7_4319_BAC6_531AE7657B73__INCLUDED_)
#define AFX_TOBIDAMEGE_H__B100BCC0_F4D7_4319_BAC6_531AE7657B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "Plane.h"
#include "RbPosition.h"

class RbSprite;

class TobiDamege  
{
public:
	enum DamegeColorType
	{
		eDmgColWhite = 0,
		eDmgColGreen,
		eDmgColRed,
		eDmgColPurple,

		eDmgColMax,
	};
	TobiDamege();
	~TobiDamege();
	void AddDamege( long Damege, const D3DVECTOR& BasePos, DamegeColorType Color=eDmgColWhite, long AddHight=0 );
	void Job();
	void Draw( Display& Disp, RbSprite& Surface ) const;
	void ClearData();

private:
	struct DamegeInfo;
	void Draw( Display& Disp, RbSprite& Surface, const DamegeInfo& Info ) const;

	void Init();
	long NumHight;
	bool bInit;
	int MaxDamege;
	Plane<> ColorDamege[eDmgColMax][10];
	struct DamegeInfo
	{
		DamegeColorType Color;
		int Count;
		int Width,Height;
		int Dmg[4+3];
		RbPosition BasePos;
		int LeftPos, HightPos;
		int Life;
	};
	std::list<DamegeInfo> DmgList;
};

#endif // !defined(AFX_TOBIDAMEGE_H__B100BCC0_F4D7_4319_BAC6_531AE7657B73__INCLUDED_)
