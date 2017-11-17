// LandMoveType.h: LandMoveType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDMOVETYPE_H__AFE01A7F_3A69_4004_A00C_772EE790C61F__INCLUDED_)
#define AFX_LANDMOVETYPE_H__AFE01A7F_3A69_4004_A00C_772EE790C61F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
class XMLPaser;
class LandType;

class LandMoveType  
{
public:
	struct MoveData
	{
		float Move;
		float Dmg;
		bool bUnRadar;
	};
	LandMoveType();
	virtual ~LandMoveType();
	bool SetData( const LandType& BaseType, const XMLPaser& MoveXML );
	bool SetValue( BYTE ID, const MoveData& NewValue );

	const float& RefValue( const BYTE& Index ) const{return MoveValue[Index].Move;}
	const MoveData& RefData( const BYTE& Index ) const{return MoveValue[Index];}
	const String& Name() const{return MyName;}
	const float& RefSpeed() const{return Speed;}
protected:
	float Speed;
	MoveData MoveValue[256];
	String MyName;
};

#endif // !defined(AFX_LANDMOVETYPE_H__AFE01A7F_3A69_4004_A00C_772EE790C61F__INCLUDED_)
