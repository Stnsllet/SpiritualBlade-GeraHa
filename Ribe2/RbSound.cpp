// RbSound.cpp: RbSound クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "RbSound.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbSound::RbSound()
{

}

RbSound::~RbSound()
{

}

// 音を鳴らす
HRESULT RbSound::Play( int WaveNo, const D3DVECTOR& Pos )
{
	return SoundPlay( WaveNo, Pos.x, Pos.y, Pos.z );
}

// 音を鳴らす
HRESULT RbSound::Play( LPCTSTR WaveName, const D3DVECTOR& Pos )
{
	return Play( IDMap.GetID(WaveName), Pos );
}

// すべて初期化
HRESULT RbSound::InitRb( HWND hWnd )
{
	HRESULT ret = InitSound( hWnd );
	if( FAILED(ret) )return ret;

	ret = ReadDefineFile( "System/WaveDefine.xml" );
	if( FAILED(ret) )return ret;

	return S_OK;
}

HRESULT RbSound::ReadDefineFile( LPCTSTR pFileName )
{
	XMLPaser AllXML;
	HRESULT ret = AllXML.Load( pFileName );
	if( FAILED(ret) )return ret;

	float RollOff = AllXML.AttributeFloat(_T("rolloff"));
	if( RollOff < 0 )RollOff = 0;	// 無限

	ret = SetRollOff( RollOff );
	if( FAILED(ret) )return ret;

	XMLList FileList;
	FileList.Query( AllXML, TAG::FILE );
	long FileCount = FileList.GetCount();

	for( int i = 0; i < FileCount; i++ )
	{
		FileRead( FileList.GetItem(i) );
	}
	return S_OK;
}

// ファイル読み込み
HRESULT RbSound::FileRead( const XMLPaser& File )
{
	String FilePath = File.AttributeString(TAG::PATH);
	String Name = File.AttributeString( TAG::NAME );
	long vol = File.AttributeLong("vol");
	long interval = File.AttributeLong("interval");
	if( interval == 0 )interval = 200;

	long no = IDMap.GetID( Name );

	HRESULT ret = SetData( FilePath, no, vol, interval );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// キャッシュ
bool RbSound::Cache( const String& Name )
{
	const IDManager& IDList = IDMap;
	long WaveID = IDList.GetID( Name );
	if( WaveID <= 0 )return false;
	std::map<int,WaveFile>::const_iterator itr = WaveFileList.find(WaveID);
	if( itr == WaveFileList.end() )return false;

	const String& Path = itr->second.RefPath();
	if( Path.empty() )return false;
	BufCache.Cache( Path );
	return true;
}
