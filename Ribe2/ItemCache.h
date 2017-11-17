// ItemCache.h: ItemCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ItemCache_H__1ACE32B5_A625_4F4B_9A62_7F39BF10C3D6__INCLUDED_)
#define AFX_ItemCache_H__1ACE32B5_A625_4F4B_9A62_7F39BF10C3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "Texture.h"
#include "Statusstruct.h"
#include "BillbordPlane.h"
#include "DefaultPlanes.h"

class XMLPaser;

class ItemCache  
{
public:
	struct ItemType
	{
		String Name;
		String NameID;
		short EquipType;
	};
	ItemCache();
	virtual ~ItemCache();

	HRESULT ReadItemData( DISPDEVICE* pDevice );
	short GetEquipType( const String& TypeName ) const;
	const ItemStatus& Get( long ItemNum ) const;
	const ItemType& GetTypeData( const String& Type ) const;
	RefBord* GetTexture( long ItemNum );
	Plane<HandTexRect> GetPlane( long ItemNum ) const;

	static LPCTSTR BaseTypeNames[];
	static long BaseTypeCount;
	static short GetBaseType( const String& Name );
private:
	long GetPrice( const ItemStatus& Item ) const;
	HRESULT ReadItemFiles( DISPDEVICE* pDevice, LPCTSTR FileName );
	HRESULT ReadItemFile( LPCTSTR FileName );
	HRESULT ReadItemData( const XMLPaser& ItemData );

	HRESULT ReadItemType();
	HRESULT ReadItemType( LPCTSTR FileName );
	HRESULT ReadItemType( short Type, const XMLPaser& TypeData );
	HRESULT ReadNamedPlane( DISPDEVICE* pDevice, const XMLPaser& PlaneXML );
	typedef std::map<String,ItemType> TypeMap;
	TypeMap TypeList;
	typedef std::map<long,ItemStatus> ItemMap;
	ItemMap ItemList;
	std::map<long,RefPtr<RefBord> > BillBordList;

	ItemStatus DefaultItem;

	HandPlanes NamedPlane;
};

#endif // !defined(AFX_ItemCache_H__1ACE32B5_A625_4F4B_9A62_7F39BF10C3D6__INCLUDED_)
