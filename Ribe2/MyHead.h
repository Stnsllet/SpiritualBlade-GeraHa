
// ヘッダなどなど
#ifndef __MYHEAD__
#define __MYHEAD__

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dxerr8.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dmusics.h>
#include <dmusici.h>
#include <dsound.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")
#pragma comment(lib, "dxerr8.lib")

#define USEDIRECT3D	IDirect3D8
#define DISPDEVICE	IDirect3DDevice8
#define MYTEXTURE	IDirect3DTexture8
#define MYSURFACE	IDirect3DSurface8
#define MYSPRITE	ID3DXSprite
//#define GAMETITLE	_T("Eternal Fate")

#ifndef MYMACROS

#ifndef count_of
#define count_of(x) (sizeof(x)/sizeof(x[0]))
#endif //count_of

template<class T> bool SafeDelete( T*& pClass )
{
	T* pStock = pClass;
	pClass = NULL;
	if( pStock == NULL )return false;
	delete pStock;
	return true;
}

inline float AClip( const float& Angle )
{
	return (Angle>=0)?(fmodf(Angle,2.0f)):(fmodf(Angle,2.0f)+2.0f);
}

inline int Rnd(const int& Max)
{
	return (Max<=0)?(0):(rand()%(Max+1));
}

inline int Rnd2(const int& Min, const int& Max)
{
	return Min+Rnd(Max-Min);
}

inline bool Rnd100(const int& Check)
{
	return (Check>Rnd(99));
}

inline float Rndf(const float& Max)
{
	return ((rand()*Max)/(float)RAND_MAX);
}

inline float Rndf2(const float& Min, const float& Max)
{
	return Min+Rndf(Max-Min);
}

inline DWORD ErrorMsg(DWORD error)
{
	LPTSTR lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
		(LPTSTR) &lpMsgBuf,
		0,
		NULL );
	if( lpMsgBuf != NULL )
	{
		::OutputDebugString( lpMsgBuf );
		LocalFree( lpMsgBuf );
	}
	return error;
}

inline int PopupMsg( LPCTSTR Message )
{
	return ::MessageBox( NULL, Message, _T("メッセージ"), MB_OK | MB_ICONEXCLAMATION );
}

inline bool IsMemoryError( HRESULT hr )
{
	switch( hr )
	{
	case E_OUTOFMEMORY: case D3DERR_OUTOFVIDEOMEMORY:
		return true;
	}
	return false;
}

inline int MemoryErrorMsg( HRESULT hr )
{
	switch( hr )
	{
	case D3DERR_OUTOFVIDEOMEMORY:
		return PopupMsg( _T("ビデオメモリが不足しています") );
	case E_OUTOFMEMORY:
		return PopupMsg( _T("メインメモリが不足しています") );
	}
	return IDCANCEL;
}

#ifdef _DEBUG
#define FAILEDTRACE(x)	FailedMsg(x,_T(__FILE__),__LINE__)
#else
#define FAILEDTRACE(x)
#endif	// _DEBUG

#ifdef __HI_TRACE__
#include "String.h"
inline void HiTrace(const TCHAR* pMessage, ...)
{
	va_list args;
	va_start(args, pMessage);
	String Msg;
	Msg.Format( pMessage, args );
	OutputDebugString(Msg);
}
#undef ATLTRACE
#define ATLTRACE HiTrace
#undef ATLASSERT
#define ATLASSERT(expr) (expr)?(0):(OutputDebugString(#expr))
#endif

/*
#ifdef _DEBUG
#undef FAILED
#define FAILED(x)	FAILEDTRACE(x)
#endif	// _DEBUG
*/
inline bool FailedMsg( HRESULT hr, LPTSTR pFileName, DWORD Line )
{
	bool ret = ((HRESULT)(hr)<0);
	if( !ret )return ret;
	DXTrace( pFileName, Line, hr, _T("FAILED Message"), FALSE );
	return ret;
}

template<class T> T& AllZero( T& Data )
{
	ZeroMemory(&Data,sizeof(Data));
	return Data;
}

template<class T> T Sizing( const T& Dest, const T& Base, float Time, float MaxTime )
{
	if( MaxTime <= 0 || Time >= MaxTime )return Dest;
	if( Time <= 0 )return Base;
	T Vector = Dest - Base;
	Vector *= Time / MaxTime;
	return (Base + Vector);
}

template<class T> T Trim( const T& Taget, const T& Max, const T& Min )
{
	if( Taget < Min )return Min;
	if( Taget > Max )return Max;
	return Taget;
}

#endif	// MYMACROS

#endif	// __MYHEAD__