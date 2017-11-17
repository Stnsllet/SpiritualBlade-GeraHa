// TobiDamege.cpp: TobiDamege クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "BattleSystem.h"
#include "RbDisplay.h"
#include "RbSprite.h"
#include "TobiDamege.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDamege::TobiDamege()
{
	bInit = false;
	NumHight = 0;
}

TobiDamege::~TobiDamege()
{
}

void TobiDamege::ClearData()
{
	DmgList.clear();
}

void TobiDamege::Init()
{
	MaxDamege = 1;
	int DCount = count_of(DamegeInfo().Dmg);
	while( DCount-- )MaxDamege *= 10;
	MaxDamege -=1;

	DefaultPlanes& SysMsg = GetCache().SystemPlane;
	Plane<> White = SysMsg.GetPlane(_T("whitenum"));
	const RbPoint& WhitePos = RbRect(White).LT();
	const RbPoint& GreenPos = RbRect(SysMsg.GetPlane(_T("greennum"))).LT();
	const RbPoint& RedPos = RbRect(SysMsg.GetPlane(_T("rednum"))).LT();
	const RbPoint& PurplePos = RbRect(SysMsg.GetPlane(_T("purplenum"))).LT();

	NumHight = White.h;

	for( int i = 0; i < count_of(ColorDamege[eDmgColWhite]); i++ )
	{
		String Format;
		Format.Format( _T("N%d"), i );
		ColorDamege[eDmgColWhite][i] = SysMsg.GetPlane(Format);
		ColorDamege[eDmgColGreen][i] = ColorDamege[eDmgColWhite][i];
		ColorDamege[eDmgColRed][i] = ColorDamege[eDmgColWhite][i];
		ColorDamege[eDmgColPurple][i] = ColorDamege[eDmgColWhite][i];

		ColorDamege[eDmgColWhite][i].x += WhitePos.x;
		ColorDamege[eDmgColWhite][i].y += WhitePos.y;
		ColorDamege[eDmgColGreen][i].x += GreenPos.x;
		ColorDamege[eDmgColGreen][i].y += GreenPos.y;
		ColorDamege[eDmgColRed][i].x += RedPos.x;
		ColorDamege[eDmgColRed][i].y += RedPos.y;
		ColorDamege[eDmgColPurple][i].x += PurplePos.x;
		ColorDamege[eDmgColPurple][i].y += PurplePos.y;
	}
	bInit = true;
}

void TobiDamege::AddDamege( long Damege, const D3DVECTOR& BasePos, DamegeColorType Color, long AddHight )
{
	if( !bInit )Init();

	DamegeInfo AddInfo;
	AllZero(AddInfo);
	if( Damege < 0 )Damege = -Damege;
	if( Damege > MaxDamege )Damege = MaxDamege;

	AddInfo.BasePos = BasePos;
	AddInfo.Life = 2000/GetBattleSystem().JobTime();
	AddInfo.Color = Color;

	do
	{
		int DmgNum = Damege % 10;
		AddInfo.Dmg[AddInfo.Count] = DmgNum;
		AddInfo.Width += ColorDamege[Color][DmgNum].w-1;
		AddInfo.Count++;

	}while( (Damege/=10)>0 );
	AddInfo.Width += 1;

	AddInfo.Height = ColorDamege[Color][0].h;
	AddInfo.HightPos = -AddInfo.Height;
	AddInfo.HightPos -= NumHight*AddHight;

	DmgList.push_back( AddInfo );
}

void TobiDamege::Job()
{
	std::list<DamegeInfo>::iterator itr = DmgList.begin();
	while( itr != DmgList.end() )
	{
		(itr->HightPos)--;
		if( ((itr->Life)--)<=0 )itr = DmgList.erase(itr);
		else itr++;
	}
}

#include "PaformanceChecker.h"
void TobiDamege::Draw( Display& Disp, RbSprite& Surface ) const
{
	PEFORMANCE("Damege Draw");
//	if( DmgList.size() <= 0 )return;

	std::list<DamegeInfo>::const_iterator itr = DmgList.begin();
	while( itr != DmgList.end() )Draw( Disp, Surface, *itr++ );
}

void TobiDamege::Draw( Display& Disp, RbSprite& Surface, const DamegeInfo& Info ) const
{
	RbPosition Pos = Disp.ViewToDisp( Info.BasePos );
	Pos.x -= (Info.Width)/2.0f;
	Pos.y += Info.HightPos;

	if( Pos.y+(Info.Height) < 0 || Pos.y > Surface.GetSize().y )return;
	if( Pos.x+(Info.Width) < 0 || Pos.x > Surface.GetSize().x )return;

	int WPos = 0;
	const Plane<>* pCol = ColorDamege[Info.Color];
	Pos.x = (long)Pos.x;
	Pos.y = (long)Pos.y;
	for( int i = Info.Count-1; i >= 0; i-- )
	{
		const Plane<>& Ref = pCol[ Info.Dmg[i] ];
		Surface.DrawPlane( Ref, Pos.x+WPos, Pos.y );
		WPos += Ref.w-1;
	}
}
