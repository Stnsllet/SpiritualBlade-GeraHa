// Plane.h: Plane クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANE_H__C8956671_ED72_46FF_83B7_7B5823B64EB2__INCLUDED_)
#define AFX_PLANE_H__C8956671_ED72_46FF_83B7_7B5823B64EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusStruct.h"
#include "Texture.h"
#include "TAG.h"
#include "XMLPaser.h"

template <class T=TexRect> struct Plane : public T
{
public:
	Plane(){}
	Plane( const T& Src ){(*(T*)this) = Src;}
	~Plane(){}

	operator const T&() const{return *this;}
	operator T&(){return *this;}

	Plane<T>& ReadXML( const XMLPaser& XML )
	{
		ReadXML( *this, XML );
		return *this;
	}
	Plane<T>& ReadHand( const XMLPaser& XML )
	{
		px = XML.AttributeLong(TAG::PX);
		py = XML.AttributeLong(TAG::PY);
		angle = XML.AttributeFloat(TAG::A);
		return *this;
	}
	CComPtr<MYTEXTURE> pTexture;
	operator MYTEXTURE*() const{return pTexture;}
private:
	TexRect& ReadXML( TexRect& Dest, const XMLPaser& XML )
	{
		Dest.x = XML.AttributeLong(TAG::X);
		Dest.y = XML.AttributeLong(TAG::Y);
		Dest.w = XML.AttributeLong(TAG::W);
		Dest.h = XML.AttributeLong(TAG::H);
		return Dest;
	}
	HandTexRect& ReadXML( HandTexRect& Dest, const XMLPaser& XML )
	{
		ReadXML( (TexRect&)Dest, XML );
		return ReadHand( XML );
	}

};

template <class T=DefaultStatus> struct StatusEdit
{
public:
	StatusEdit( T& RefData ):Ref(RefData){}
	~StatusEdit(){}

	operator const T&() const{return Ref;}
	operator T&(){return Ref;}

	StatusEdit<T>& ReadXML( const XMLPaser& XML )
	{
		ReadXML( *this, XML );
		return *this;
	}
	StatusEdit<T>& SkipReadXML( const XMLPaser& XML )
	{
		ReadSkip( Ref.Hp, XML, HP );
		ReadSkip( Ref.Mp, XML, MP );
		ReadSkip( Ref.Attack, XML, ATC );
		ReadSkip( Ref.Defence, XML, DEF );
		ReadSkip( Ref.Magic, XML, MAG );
		ReadSkip( Ref.Speed, XML, SPD );
		ReadSkip( Ref.Move, XML, MOV );
		return *this;
	}
	float& RefState( const String& Name )
	{
		if( Name == HP )return Ref.Hp;
		if( Name == MP )return Ref.Mp;

		if( Name == ATC )return Ref.Attack;
		if( Name == DEF )return Ref.Defence;
		if( Name == MAG )return Ref.Magic;
		if( Name == SPD )return Ref.Speed;
		if( Name == MOV )return Ref.Move;

		return DumVal;
	}
private:
	T& ReadXML( T& Dest, const XMLPaser& XML ) const
	{
		Dest.Hp = XML.AttributeFloat(TAG::HP);
		Dest.Mp = XML.AttributeFloat(TAG::MP);

		Dest.Attack = XML.AttributeFloat(TAG::ATC);
		Dest.Defence = XML.AttributeFloat(TAG::DEF);
		Dest.Magic = XML.AttributeFloat(TAG::MAG);
		Dest.Speed = XML.AttributeFloat(TAG::SPD);
		Dest.Move = XML.AttributeFloat(TAG::MOV);
		return Dest;
	}
	bool ReadSkip( float& Dest, const XMLPaser& XML, LPCTSTR Name ) const
	{
		CComBSTR BName(Name);
		if( !XML.HasAttribute(BName) )return false;
		Dest = XML.AttributeFloat(BName);
		return true;
	}
	T& Ref;

	static float DumVal;
	static LPCTSTR HP;
	static LPCTSTR MP;
	static LPCTSTR ATC;
	static LPCTSTR DEF;
	static LPCTSTR MAG;
	static LPCTSTR SPD;
	static LPCTSTR MOV;
};

template <class T> float StatusEdit<T>::DumVal = 0;
template <class T> LPCTSTR StatusEdit<T>::HP = _T("hp");
template <class T> LPCTSTR StatusEdit<T>::MP = _T("mp");
template <class T> LPCTSTR StatusEdit<T>::ATC = _T("atc");
template <class T> LPCTSTR StatusEdit<T>::DEF = _T("def");
template <class T> LPCTSTR StatusEdit<T>::MAG = _T("mag");
template <class T> LPCTSTR StatusEdit<T>::SPD = _T("spd");
template <class T> LPCTSTR StatusEdit<T>::MOV = _T("mov");

#endif // !defined(AFX_PLANE_H__C8956671_ED72_46FF_83B7_7B5823B64EB2__INCLUDED_)
