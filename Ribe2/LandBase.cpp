// LandBase.cpp: LandBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "XMLList.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "LandBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandBase::LandBase()
{
	bInit = false;

	GData.ChipSize.x = 48.0f;
	GData.ChipSize.y = 48.0f;
	GData.Face.x = GData.Face.y = 0;
	GData.Version = 1.0f;
}

LandBase::~LandBase()
{
}

// ランドスケープの大きさをドットで取得
RbRect LandBase::GetLandRect() const
{
	return RbRect( GData.Face.x*GData.ChipSize.x/2.0f,
		GData.Face.y*GData.ChipSize.y );
}

void LandBase::InitLand( DISPDEVICE* pDevice )
{
	ClearData();
	bInit = true;
}


void LandBase::ClearData()
{
	GData.ChipSize.x = 48.0f;
	GData.ChipSize.y = 48.0f;
	GData.Face.x = GData.Face.y = 0;
	GData.Version = 1.0f;

	LandFilePath.erase();

	SinpleList.clear();
	MapData.clear();
	LandData.clear();
	IndexData.clear();
	FaceNormal.clear();
}

// プレーン取得
LandBase::SinplePlane& LandBase::GetSinplePlane( const String& Name )
{
	std::map<String,SinplePlane>::iterator itr = SinpleList.find( Name );
	if( itr != SinpleList.end() )return itr->second;

	RbCache& Cache = GetCache();
	const LandType::LandTex& Ref = Cache.Land.GetTexData( Name );

	SinplePlane& NewPlane = SinpleList[Name];
	Plane<>& RefPlane = Cache.PathTex.Get(Ref.Path);
	NewPlane.ChipTexture.SetTexture( RefPlane );

	return NewPlane;
}

// ランドスケープロード
HRESULT LandBase::LoadLandScape( LPCTSTR FileName )
{
	if( FileName == NULL )return E_INVALIDARG;

	if( !IsInit() )return E_FAIL;

	File fp;
	HRESULT ret = fp.Open( FileName, false );
	if( FAILED(ret) )return ret;

	ClearData();
	Display& Disp = GetRbDisplay();

	eLandDataType ReadType = eLandTypeNone;
	while( SUCCEEDED( fp.Read(ReadType) ) )
	{
		switch( ReadType )
		{
		case eLandTypeGlobal:
			ret = LoadGlobalData( fp );
			break;

		case eLandTypeBasePlane:
			ret = LoadBasePlane( fp, Disp );
			break;

		case eLandTypeSinplePlane:
			ret = LoadSinplePlane( fp, Disp );
			break;
		}
		if( FAILED(ret) )return ret;
	}

	LandFilePath = FileName;
	return S_OK;
}

// 全体情報取得
HRESULT LandBase::LoadGlobalData( File& fp )
{
	HRESULT ret = fp.Read( GData );
	if( FAILED(ret) )return ret;

	ret = MapData.Load( fp );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// 土地基本情報取得
HRESULT LandBase::LoadBasePlane( File& fp, DISPDEVICE* pDevice )
{
	HRESULT ret = LandData.Load( pDevice, fp );
	if( FAILED(ret) )return ret;

	ret = IndexData.Load( pDevice, fp );
	if( FAILED(ret) )return ret;

	ret = FaceNormal.Load( fp );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// シンプルロード
HRESULT LandBase::LoadSinplePlane( File& fp, DISPDEVICE* pDevice )
{
	long SinpleCount = 0;
	HRESULT ret = fp.Read( SinpleCount );
	if( FAILED(ret) )return ret;
	for( long i = 0; i < SinpleCount; i ++ )
	{
		String Name;
		ret = fp.ReadStr( Name );
		if( FAILED(ret) )return ret;

		SinplePlane& Ref = GetSinplePlane( Name );
		ret = Ref.IndexData.Load( pDevice, fp );
		if( FAILED(ret) )return ret;
	}

	return S_OK;
}
