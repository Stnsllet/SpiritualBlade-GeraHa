// LandType.h: LandType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDTYPE_H__1BC0EB48_942D_4031_951B_B4B39EA63538__INCLUDED_)
#define AFX_LANDTYPE_H__1BC0EB48_942D_4031_951B_B4B39EA63538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "StringIDMap.h"
#include "LandMoveType.h"
class XMLPaser;

class LandType  
{
public:
	struct LandTypeData
	{
		String Type;
		long ID;
	};
	struct LandTex
	{
		String Path;
	};

	LandType();
	virtual ~LandType();

	HRESULT LoadLandType();
	HRESULT LoadLandType( LPCTSTR FilePath );
	HRESULT LoadMoveType( LPCTSTR FilePath );
	long SetMoveValue( LandMoveType& MoveData, const String& Type, const LandMoveType::MoveData& Value ) const;
	const LandMoveType& MoveDat( const String& Name ) const;
	const LandTypeData& GetTypeData( long ID ) const;
	const LandTex& GetTexData( const String& Name ) const;

protected:
	HRESULT ReadMapData( const XMLPaser& TypeXML );
	HRESULT ReadTexData( const XMLPaser& TypeXML );

	std::map<String,LandTex> LandTexList;
	std::map<long,LandTypeData> LandTypeList;
	std::map<String,LandMoveType> MoveTypeList;
	LandMoveType DefaultMoveType;
};

#endif // !defined(AFX_LANDTYPE_H__1BC0EB48_942D_4031_951B_B4B39EA63538__INCLUDED_)
