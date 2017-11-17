// RbSound.h: RbSound クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBSOUND_H__11685DBD_8A65_4D65_AC3D_4ED09285F712__INCLUDED_)
#define AFX_RBSOUND_H__11685DBD_8A65_4D65_AC3D_4ED09285F712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultiWave.h"
#include "StringIDMap.h"

class XMLPaser;

class RbSound : public MultiWave  
{
public:
	RbSound();
	virtual ~RbSound();

	HRESULT InitRb( HWND hWnd );
	long GetID( LPCTSTR WaveName ) const{return IDMap.GetID(WaveName);}

	HRESULT Play( int WaveNo){return Play(WaveNo, LisnerPos);}
	HRESULT Play( LPCTSTR WaveName){return Play(WaveName, LisnerPos);}
	HRESULT Play( int WaveNo, const D3DVECTOR& Pos );
	HRESULT Play( LPCTSTR WaveName, const D3DVECTOR& Pos );

	bool Cache( const String& Name );
private:
	HRESULT ReadDefineFile( LPCTSTR pFileName );
	HRESULT FileRead( const XMLPaser& File );

	IDManager IDMap;

};

RbSound& GetSoundSystem();

#endif // !defined(AFX_RBSOUND_H__11685DBD_8A65_4D65_AC3D_4ED09285F712__INCLUDED_)
