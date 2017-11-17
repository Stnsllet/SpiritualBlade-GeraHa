// ExCreator.h: ExCreator クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCREATOR_H__643E243A_3E7E_4A01_96C9_DF4BD4F119A0__INCLUDED_)
#define AFX_EXCREATOR_H__643E243A_3E7E_4A01_96C9_DF4BD4F119A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include "TobiCreator.h"
class XMLPaser;

struct ExCreator  
{
	class BaseDef;
	static TobiCreator* CreateCreator( BaseDef* pDef );
	struct DefBaseData
	{
		DefBaseData();
		String Name;
		String Type;
		String Poly;
		String Wave;
		float Speed;
		float Range;
		float UseMp;
		float KnockPow;
		float Shake;
		long PolyID, WaveID;
		bool bMagic;
		bool bAllHit;
		bool bInit;
	};
	class BaseDef : public RefAuto, public DefBaseData
	{
	public:
		BaseDef();
		virtual ~BaseDef();
		bool ReadXML( XMLPaser& ReadXML );
		bool Init();
		RefPtr<BaseDef> pChiled;
	};

	class Base : public TobiCreator
	{
	public:
		Base();
		virtual ~Base();
		virtual bool InitCreator() const;
		virtual bool SetDef( BaseDef* pNew );
	protected:
		RefPtr<BaseDef> pDef;
	};

	class Simple : public Base
	{
	public:
		Simple(){}
		virtual ~Simple(){}
		virtual TobiObjectBase* Create() const;
	};
	class Arrow : public Base
	{
	public:
		Arrow(){}
		virtual ~Arrow(){}
		virtual TobiObjectBase* Create() const;
	};
	class Burst : public Base
	{
	public:
		Burst(){}
		virtual ~Burst(){}
		virtual TobiObjectBase* Create() const;
	};
	class Homing : public Base
	{
	public:
		Homing(){}
		virtual ~Homing(){}
		virtual TobiObjectBase* Create() const;
	};
	class Spread : public Base
	{
	public:
		Spread(){}
		virtual ~Spread(){}
		virtual TobiObjectBase* Create() const;
	};
	class Liner : public Base
	{
	public:
		Liner(){}
		virtual ~Liner(){}
		virtual TobiObjectBase* Create() const;
	};
	class Circle : public Base
	{
	public:
		Circle(){}
		virtual ~Circle(){}
		virtual TobiObjectBase* Create() const;
	};
	class RndHit : public Base
	{
	public:
		RndHit(){}
		virtual ~RndHit(){}
		virtual TobiObjectBase* Create() const;
	};
	class AllHit : public Base
	{
	public:
		AllHit(){}
		virtual ~AllHit(){}
		virtual TobiObjectBase* Create() const;
	};
	class Around : public Base
	{
	public:
		Around(){}
		virtual ~Around(){}
		virtual TobiObjectBase* Create() const;
	};
};

#endif // !defined(AFX_EXCREATOR_H__643E243A_3E7E_4A01_96C9_DF4BD4F119A0__INCLUDED_)
