// StrParamCom.h: StrParamCom クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRPARAMCOM_H__F74B1E11_64B5_40A7_9423_4F0F755AD721__INCLUDED_)
#define AFX_STRPARAMCOM_H__F74B1E11_64B5_40A7_9423_4F0F755AD721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringSplitter.h"
class RbGameData;

class StrParamCom  
{
public:
	StrParamCom( const String& Name );
	StrParamCom( const StringSplitter<>& Commands );
	virtual ~StrParamCom();

	bool Param( const StringSplitter<>& Commands );

	bool RndParam( const StringSplitter<>& Commands );
	bool ItemNameParam( long ItemNum );
	bool ItemTypeParam( long ItemNum );
	bool MemberParam( const String& MemName, const String& DataName );
	bool KillRankParam( long KillRank );
	bool BatCountParam( const String& Army, const String& Type );
	bool LeaderParam( const String& PatyNo );
	bool BatResultParam( const String& Type );
	bool BatPosParam( const String& Name );
	bool SelectCharactorParam();
protected:
	bool SetParam( const String& Value );
	bool SetParam( const CComVariant& Value );
	const String& ParamName;
	RbGameData& UserData;
};

#endif // !defined(AFX_STRPARAMCOM_H__F74B1E11_64B5_40A7_9423_4F0F755AD721__INCLUDED_)
