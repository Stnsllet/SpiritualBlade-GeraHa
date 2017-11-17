// TobiSystem.cpp: TobiSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "DispStocker.h"
#include "RbSprite.h"
#include "TobiSystem.h"

#include "AllTobiObject.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiSystem::TobiSystem()
{

}

TobiSystem::~TobiSystem()
{
}

void TobiSystem::ClearData()
{
	TobiList.clear();
	Damege.ClearData();
	Planes.ClearData();
}

TobiObjectBase* TobiSystem::CreateObject( const String& Name ) const
{
	TobiCreator* pCreator = GetCreator( Name );
	if( pCreator == NULL )return NULL;
	return pCreator->Create();
}

TobiCreator* TobiSystem::GetCreator( const String& Name ) const
{
	std::map<String,MasterPtr<TobiCreator> >::const_iterator itr = TobiCreatorList.find( Name );
	if( itr == TobiCreatorList.end() )return NULL;
	return itr->second;
}

template <class T> TobiSystem::CreateAdder<T>::CreateAdder( std::map<String,MasterPtr<TobiCreator> >& List, AttackObjectDefine& RefDef, LPCTSTR Str )
:Ref(List),Def(RefDef),Name(Str)
{
	ATLASSERT(List.find(Name) == List.end() );
	List[Name] = new TobiCreatorT<T>(Def.RefData(Name).UseMp);
}

void TobiSystem::Init()
{
	AtcObjDef.ReadDef();

	{CreateAdder<TobiKnife> Add(TobiCreatorList, AtcObjDef, _T("k") );}
	{CreateAdder<TobiArrow> Add(TobiCreatorList, AtcObjDef, _T("a") );}
	{CreateAdder<TobiLaser> Add(TobiCreatorList, AtcObjDef, _T("ray") );}
	{CreateAdder<TobiFire> Add(TobiCreatorList, AtcObjDef, _T("f") );}
	{CreateAdder<TobiWater> Add(TobiCreatorList, AtcObjDef, _T("w") );}
	{CreateAdder<TobiIce> Add(TobiCreatorList, AtcObjDef, _T("i") );}
	{CreateAdder<TobiBomb> Add(TobiCreatorList, AtcObjDef, _T("b") );}
	{CreateAdder<TobiThunder> Add(TobiCreatorList, AtcObjDef, _T("t") );}
	{CreateAdder<TobiStar> Add(TobiCreatorList, AtcObjDef, _T("s") );}
	{CreateAdder<TobiDark> Add(TobiCreatorList, AtcObjDef, _T("d") );}
	{CreateAdder<TobiHopper> Add(TobiCreatorList, AtcObjDef, _T("hop") );}
	{CreateAdder<TobiWhiteBlowBit> Add(TobiCreatorList, AtcObjDef, _T("blow") );}
	{CreateAdder<TobiRandom> Add(TobiCreatorList, AtcObjDef, _T("rnd") );}
	{CreateAdder<TobiDarkRainBit> Add(TobiCreatorList, AtcObjDef, _T("dshot") );}

	{CreateAdder<TobiWindBit> Add(TobiCreatorList, AtcObjDef, _T("wind") );}
	{CreateAdder<TobiBurst> Add(TobiCreatorList, AtcObjDef, _T("burst") );}
	{CreateAdder<TobiBlueWave> Add(TobiCreatorList, AtcObjDef, _T("bwave") );}
	{CreateAdder<TobiPunishWave> Add(TobiCreatorList, AtcObjDef, _T("punish") );}
	{CreateAdder<TobiFenix> Add(TobiCreatorList, AtcObjDef, _T("fenix") );}
	{CreateAdder<TobiBall> Add(TobiCreatorList, AtcObjDef, _T("ball") );}
	{CreateAdder<TobiComet> Add(TobiCreatorList, AtcObjDef, _T("comet") );}
	{CreateAdder<TobiFeather> Add(TobiCreatorList, AtcObjDef, _T("feather") );}
	{CreateAdder<TobiRoket> Add(TobiCreatorList, AtcObjDef, _T("roket") );}
	{CreateAdder<TobiMagicSword> Add(TobiCreatorList, AtcObjDef, _T("magswd") );}
	{CreateAdder<TobiWindThrustGene> Add(TobiCreatorList, AtcObjDef, _T("w_thrust") );}

	{CreateAdder<TobiBlastEdge> Add(TobiCreatorList, AtcObjDef, _T("b_edge") );}
	{CreateAdder<TobiHalfMoon> Add(TobiCreatorList, AtcObjDef, _T("halfmoon") );}
	{CreateAdder<TobiArrowRain> Add(TobiCreatorList, AtcObjDef, _T("a_rain") );}
	{CreateAdder<TobiBurnFlare> Add(TobiCreatorList, AtcObjDef, _T("b_flare") );}
	{CreateAdder<TobiWhiteNoise> Add(TobiCreatorList, AtcObjDef, _T("wnoise") );}
	{CreateAdder<TobiSplash> Add(TobiCreatorList, AtcObjDef, _T("splash") );}
	{CreateAdder<TobiShockWave> Add(TobiCreatorList, AtcObjDef, _T("shockwave") );}
	{CreateAdder<TobiLightning> Add(TobiCreatorList, AtcObjDef, _T("lightning") );}
	{CreateAdder<TobiDeathSickle> Add(TobiCreatorList, AtcObjDef, _T("sickle") );}
	{CreateAdder<TobiDeadlySlash> Add(TobiCreatorList, AtcObjDef, _T("dslash") );}

	{CreateAdder<TobiChainBurst> Add(TobiCreatorList, AtcObjDef, _T("c_burst") );}
	{CreateAdder<TobiChainFlare> Add(TobiCreatorList, AtcObjDef, _T("c_flare") );}
	{CreateAdder<TobiDarkRainGene> Add(TobiCreatorList, AtcObjDef, _T("d_rain") );}
	{CreateAdder<TobiWhiteBlowGene> Add(TobiCreatorList, AtcObjDef, _T("wblow") );}
	{CreateAdder<TobiCrimson> Add(TobiCreatorList, AtcObjDef, _T("crimson") );}
	{CreateAdder<TobiHealMist> Add(TobiCreatorList, AtcObjDef, _T("healmist") );}
	{CreateAdder<TobiSoulBurst> Add(TobiCreatorList, AtcObjDef, _T("soulburst") );}
	{CreateAdder<TobiNoahGene> Add(TobiCreatorList, AtcObjDef, _T("noah") );}

	{CreateAdder<TobiHellFireGene> Add(TobiCreatorList, AtcObjDef, _T("hellfire") );}
	{CreateAdder<TobiFrameCircleGene> Add(TobiCreatorList, AtcObjDef, _T("fcircle") );}
	{CreateAdder<TobiDarkBlazeGene> Add(TobiCreatorList, AtcObjDef, _T("dblaze") );}
	{CreateAdder<TobiWaterBitGene> Add(TobiCreatorList, AtcObjDef, _T("waterbit") );}

	{CreateAdder<TobiBeast> Add(TobiCreatorList, AtcObjDef, _T("beastshot") );}
	{CreateAdder<TobiHado> Add(TobiCreatorList, AtcObjDef, _T("hispirit") );}
	{CreateAdder<TobiIwasi> Add(TobiCreatorList, AtcObjDef, _T("iwasi") );}

	{CreateAdder<TobiStarLightBlade> Add(TobiCreatorList, AtcObjDef, _T("slb") );}
	{CreateAdder<TobiSwordDance> Add(TobiCreatorList, AtcObjDef, _T("sdance") );}
	{CreateAdder<TobiMeteorGene> Add(TobiCreatorList, AtcObjDef, _T("meteor") );}
	{CreateAdder<TobiRandomBall> Add(TobiCreatorList, AtcObjDef, _T("rndball") );}
	{CreateAdder<TobiDarkIraption> Add(TobiCreatorList, AtcObjDef, _T("dblow") );}
	{CreateAdder<TobiAngelDust> Add(TobiCreatorList, AtcObjDef, _T("adust") );}
	{CreateAdder<TobiFlameBirdGene> Add(TobiCreatorList, AtcObjDef, _T("flamebird") );}
	{CreateAdder<TobiRuncherGene> Add(TobiCreatorList, AtcObjDef, _T("runcher") );}
	{CreateAdder<TobiMindDowner> Add(TobiCreatorList, AtcObjDef, _T("mdown") );}
	{CreateAdder<TobiBlink> Add(TobiCreatorList, AtcObjDef, _T("blink") );}
	{CreateAdder<TobiGenocide> Add(TobiCreatorList, AtcObjDef, _T("genocide") );}

	{CreateAdder<TobiHpRecover> Add(TobiCreatorList, AtcObjDef, _T("h_recover") );}
	{CreateAdder<TobiMpRecover> Add(TobiCreatorList, AtcObjDef, _T("m_recover") );}
	{CreateAdder<TobiFullRecover> Add(TobiCreatorList, AtcObjDef, _T("f_recover") );}

	typedef std::vector<RefPtr<ExCreator::BaseDef> > ExList;
	const ExList& ExRef = AtcObjDef.RefAllEx();
	ExList::const_iterator itr = ExRef.begin();
	while( itr != ExRef.end() )
	{
		RefPtr<ExCreator::BaseDef> pDef = *itr++;
		while( pDef != NULL )
		{
			MasterPtr<TobiCreator> pCreator = ExCreator::CreateCreator(pDef);
			if( pCreator == NULL )break;
			ATLASSERT(TobiCreatorList.find(pDef->Name) == TobiCreatorList.end() );
			TobiCreatorList[pDef->Name] = pCreator;
			pDef = pDef->pChiled;
		}
	}
}

void TobiSystem::Job( BattleSystem& System )
{
	TobiDataList::iterator itr = TobiList.begin();

	while( itr != TobiList.end() )
	{
		if( !((*itr)->Job(System)) )itr = TobiList.erase(itr);
		else itr++;
	}

	Damege.Job();
	Planes.Job();
}

void TobiSystem::Draw( Display& Disp )
{
	// Z バッファ無効化
	DispStocker::Render StockZ( Disp, D3DRS_ZENABLE );
	StockZ.Set( FALSE );
	// Z バッファ書き込まない
	DispStocker::Render ZWrite( Disp, D3DRS_ZWRITEENABLE );
	ZWrite.Set( FALSE );
	// 裏も書く
	DispStocker::Render StockCull( Disp, D3DRS_CULLMODE );
	StockCull.Set( D3DCULL_NONE );
	// 影なし
	DispStocker::Material StockMaterial( Disp );
	StockMaterial.Set( Disp.RefPlaneMaterial() );

	TobiDataList::iterator itr = TobiList.begin();
	while( itr != TobiList.end() )
	{
		(*itr)->Draw( Disp );
		itr++;
	}

	RbSprite Surface;
	HRESULT ret = Surface.InitSprite( Disp );
	if( FAILED(ret) )return;

	Damege.Draw( Disp, Surface );
	Planes.Draw( Disp, Surface );
}


void TobiSystem::Add( TobiObjectBase* pAddObj )
{
	TobiList.push_back( pAddObj );
}

void TobiSystem::AddErase( RbActStatus& State )
{
	TobiRefPtr<TobiImage> pImage;
	if( !pImage->SetErase( State ) )return;
	Add( pImage );
}

void TobiSystem::AddHealLight( RbActStatus& State, long Count )
{
	for( int i = 0; i < Count; i++ )
	{
		TobiRefPtr<TobiImage> pImage;
		if( !pImage->SetLightBit( State ) )return;
		Add( pImage );
	}
}

bool TobiSystem::AddSelfBomb( RbActStatus& State, const SkillEffect& Effect )
{
	if( Effect.Pow <= 0 )return false;
	if( Effect.Range <= 0 )return false;

	TobiRefPtr<TobiSelfBomb> pSelfBomb( Effect );
	pSelfBomb->SetSource( State );

	Add( pSelfBomb );
	return true;
}

