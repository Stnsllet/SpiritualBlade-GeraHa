// LandBase.h: LandBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDBASE_H__1767275B_C20E_4772_B056_FDAC892DEC3F__INCLUDED_)
#define AFX_LANDBASE_H__1767275B_C20E_4772_B056_FDAC892DEC3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include "String.h"
#include "Texture.h"
#include "VArray.h"
#include "RbPosition.h"
#include "RbRect.h"
#include "RbBuffer.h"
#include "LandMapData.h"

class XMLPaser;
class File;

class LandBase  
{
public:
	enum eLandDataType
	{
		eLandTypeNone = 0,
		eLandTypeGlobal,
		eLandTypeSinplePlane,
		eLandTypeBasePlane,
	};
	typedef LandMapData::MapID MapID;

	LandBase();
	virtual ~LandBase();
	virtual void ClearData();

	virtual HRESULT LoadLandScape( LPCTSTR FileName );
	bool IsInit() const{return bInit; }
	virtual void InitLand( DISPDEVICE* pDevice );
	RbRect GetLandRect() const;

protected:
	HRESULT LoadGlobalData( File& fp );
	HRESULT LoadBasePlane( File& fp, DISPDEVICE* pDevice );
	HRESULT LoadSinplePlane( File& fp, DISPDEVICE* pDevice );
	struct LandTypeData
	{
		String Name;
		MapID ID;
		DWORD Color;
		String TexturePath;
	};
	struct SinplePlane
	{
		MapID MapNum;
		Texture ChipTexture;
		IndexBuffer IndexData;
	};
	struct GlobalData
	{
		float Version;
		RbPoint Face;
		RbPointF ChipSize;
	}GData;

	bool bInit;
	String LandFilePath;

	std::map<String,SinplePlane> SinpleList;
	RbBuffer<MapID>		MapData;
	LandBuffer	LandData;
	IndexBuffer	IndexData;
	RbBuffer<PanelVector>	FaceNormal;

	SinplePlane& GetSinplePlane( const String& Name );
};

#endif // !defined(AFX_LANDBASE_H__1767275B_C20E_4772_B056_FDAC892DEC3F__INCLUDED_)
