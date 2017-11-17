// RbBGMMaker.h: RbBGMMaker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBBGMMAKER_H__F9717A0A_21C0_43C8_A2DB_1C4127D48563__INCLUDED_)
#define AFX_RBBGMMAKER_H__F9717A0A_21C0_43C8_A2DB_1C4127D48563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "BGMMaker.h"

class XMLPaser;

class RbBGMMaker : public BGMMaker  
{
public:
	RbBGMMaker();
	virtual ~RbBGMMaker();

	HRESULT InitSystem();
	HRESULT InitSystem( LPCTSTR FileName );
	HRESULT Play( const String& Name );
	HRESULT Cache( const String& Name );

private:
	HRESULT ReadMidiDefine( LPCTSTR FileName );
	HRESULT ReadMidi( const XMLPaser& MidiFile );
	struct MidiPathData
	{
		String Path;
		long Volume;
		MUSIC_TIME ReStratPos;
	}LastPlayed;
	std::map<String,MidiPathData> MidiList;
};

#endif // !defined(AFX_RBBGMMAKER_H__F9717A0A_21C0_43C8_A2DB_1C4127D48563__INCLUDED_)
