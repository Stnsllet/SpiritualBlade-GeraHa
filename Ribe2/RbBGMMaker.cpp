// RbBGMMaker.cpp: RbBGMMaker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "RbBGMMaker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbBGMMaker::RbBGMMaker()
{
	AllZero(LastPlayed);
}

RbBGMMaker::~RbBGMMaker()
{

}

HRESULT RbBGMMaker::Play( const String& Name )
{
	if( Name == _T("_mute") )ChangeVolume(-10000);
	if( Name == _T("_replay") )ChangeVolume(LastPlayed.Volume);
	if( Name == _T("_stop") )
	{
		LastPlayed = AllZero(MidiPathData());
		ChangeVolume(-10000);
		return StopAll();
	}

	std::map<String,MidiPathData>::const_iterator itr = MidiList.find( Name );
	if( itr == MidiList.end() )return E_INVALIDARG;

	if( LastPlayed.Path == itr->second.Path )return S_FALSE;

	LastPlayed = itr->second;
	HRESULT hr = PlayBGM( LastPlayed.Path, LastPlayed.ReStratPos );
	ChangeVolume(LastPlayed.Volume);

	return hr;
}


HRESULT RbBGMMaker::Cache( const String& Name )
{
	std::map<String,MidiPathData>::const_iterator itr = MidiList.find( Name );
	if( itr == MidiList.end() )return E_INVALIDARG;

	return CacheFile( itr->second.Path );
}

HRESULT RbBGMMaker::InitSystem()
{
	return InitSystem( _T("System/BGMDefine.xml") );
}

HRESULT RbBGMMaker::InitSystem( LPCTSTR FileName )
{
	HRESULT ret = Init();
	if( FAILED(ret) )return ret;

	return ReadMidiDefine( FileName );
}

HRESULT RbBGMMaker::ReadMidiDefine( LPCTSTR FileName )
{
	MidiList.clear();
	XMLPaser AllXML;
	HRESULT ret = AllXML.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList FileList;
	ret = FileList.Query( AllXML, TAG::FILE );
	if( FAILED(ret) )return ret;

	int Count = FileList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadMidi( FileList.GetItem(i) );
	}
	return S_OK;
}

HRESULT RbBGMMaker::ReadMidi( const XMLPaser& MidiFile )
{
	MidiPathData ReadData;
	ReadData.Path = MidiFile.AttributeString(TAG::PATH);
	if( ReadData.Path.empty() )return E_FAIL;
	ReadData.Volume = MidiFile.AttributeLong(_T("vol"));
	ReadData.ReStratPos = MidiFile.AttributeLong(_T("loop_s"));

	String name = MidiFile.AttributeString(TAG::NAME);

	ATLASSERT( MidiList.find(name) == MidiList.end() );
	MidiList[name] = ReadData;
	return S_OK;
}
