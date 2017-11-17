// ExCreator.cpp: ExCreator クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLPaser.h"
#include "BattleSystem.h"
#include "RbCache.h"
#include "RbSound.h"
#include "TobiExSimple.h"
#include "TobiExArrow.h"
#include "TobiExBurst.h"
#include "TobiExHoming.h"
#include "TobiExSpread.h"
#include "TobiExLiner.h"
#include "TobiExCircle.h"
#include "TobiExRndHit.h"
#include "TobiExAllHit.h"
#include "TobiExAround.h"
#include "ExCreator.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiCreator* ExCreator::CreateCreator( BaseDef* pDef )
{
	if( pDef == NULL )return NULL;
	Base* pNew = NULL;
	if( pDef->Type == _T("simple") )pNew = new ExCreator::Simple;
	else if( pDef->Type == _T("arrow") )pNew = new ExCreator::Arrow;
	else if( pDef->Type == _T("burst") )pNew = new ExCreator::Burst;
	else if( pDef->Type == _T("homing") )pNew = new ExCreator::Homing;
	else if( pDef->Type == _T("spread") )pNew = new ExCreator::Spread;
	else if( pDef->Type == _T("liner") )pNew = new ExCreator::Liner;
	else if( pDef->Type == _T("circle") )pNew = new ExCreator::Circle;
	else if( pDef->Type == _T("rndhit") )pNew = new ExCreator::RndHit;
	else if( pDef->Type == _T("allhit") )pNew = new ExCreator::AllHit;
	else if( pDef->Type == _T("around") )pNew = new ExCreator::Around;

	if( pNew == NULL )return NULL;
	pNew->SetDef( pDef );
	return pNew;
}

ExCreator::DefBaseData::DefBaseData()
{
	AllZero(*this);
}

ExCreator::BaseDef::BaseDef()
{
}

ExCreator::BaseDef::~BaseDef()
{
}

bool ExCreator::BaseDef::ReadXML( XMLPaser& ReadXML )
{
	BattleSystem& Bat = GetBattleSystem();

	Name = ReadXML.AttributeString(TAG::NAME);
	if( Name.empty() )return false;
	Type = ReadXML.AttributeString(TAG::TYPE);
//	if( Type.empty() )return true;

	Poly = ReadXML.AttributeString(_T("poly"));
	Wave = ReadXML.AttributeString(_T("wave"));
	Speed = ReadXML.AttributeFloat(_T("speed")) * Bat.JobSec();
	if( Speed <= 0 )Speed = 450.0f * Bat.JobSec();
	Range = ReadXML.AttributeFloat(TAG::RANGE);
	if( Range <= 0 )Range = 8.0f;
	UseMp = ReadXML.AttributeFloat(TAG::MP);
	bMagic = ReadXML.AttributeBool(TAG::MAG);

	if( ReadXML.HasAttribute(_T("failhit")) )bAllHit = ReadXML.AttributeBool(_T("failhit"));
	else bAllHit = bMagic;

	KnockPow = ReadXML.AttributeFloat(_T("knock"));
	Shake = ReadXML.AttributeFloat(_T("shake"));

	XMLPaser ChiledXML;
	if( SUCCEEDED(ReadXML.Find(_T("Ex"), ChiledXML)) )
	{
		RefPtr<BaseDef> pRead(new BaseDef);
		if( pRead->ReadXML( ChiledXML ) )
			if(pRead->Name != Name )pChiled = pRead;
	}

	return true;
}

bool ExCreator::BaseDef::Init()
{
	if( bInit )return true;
	RbCache& Cache = GetCache();

	PolyID = Cache.Poly.GetID(Poly);
	WaveID = GetSoundSystem().GetID(Wave);
	if( pChiled != NULL )pChiled->Init();

	bInit = true;
	return true;
}

ExCreator::Base::Base()
{
}

ExCreator::Base::~Base()
{
}

bool ExCreator::Base::SetDef( BaseDef* pNew )
{
	if( pNew == NULL )return false;
	pDef = pNew;
	MpData.Level = 0.05f;
	MpData.Base = pDef->UseMp;
	return true;
}

bool ExCreator::Base::InitCreator() const
{
	if( pDef == NULL )return false;
	if( pDef->bInit )return true;

	return pDef->Init();
}

TobiObjectBase* ExCreator::Simple::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExSimple* pNew = new TobiExSimple;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Arrow::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExArrow* pNew = new TobiExArrow;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Burst::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExBurst* pNew = new TobiExBurst;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Homing::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExHoming* pNew = new TobiExHoming;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Spread::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExSpread* pNew = new TobiExSpread;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Liner::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExLiner* pNew = new TobiExLiner;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Circle::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExCircle* pNew = new TobiExCircle;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::RndHit::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExRndHit* pNew = new TobiExRndHit;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::AllHit::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExAllHit* pNew = new TobiExAllHit;
	pNew->SetExDefine( pDef );
	return pNew;
}

TobiObjectBase* ExCreator::Around::Create() const
{
	if( !InitCreator() )return NULL;

	TobiExAround* pNew = new TobiExAround;
	pNew->SetExDefine( pDef );
	return pNew;
}
