// ShopSystem.h: ShopSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPSYSTEM_H__1EC4821B_33C6_467D_A520_3701A7236B8C__INCLUDED_)
#define AFX_SHOPSYSTEM_H__1EC4821B_33C6_467D_A520_3701A7236B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <list>
#include <vector>
#include "String.h"
#include "StringSplitter.h"
struct MemberStatus;
struct ItemStatus;
class XMLPaser;

class ShopSystem  
{
public:
	ShopSystem();
	virtual ~ShopSystem();
	bool LoadSystemXML( const XMLPaser& ReadXML );

	static bool EquipList( const MemberStatus& MemState, StringSplitter<>& Output );
	static bool EquipOK( const String& ItemType, const MemberStatus& MemState );
	static bool EquipOK( long ItemNum, const String& EquipType );
	static bool EquipOK( const String& ItemType, const String& EquipType );
	static bool EquipOK( const String& ItemType, const StringSplitter<>& EquipType );

	struct ShopData
	{
		bool SetData( long ItemNo, const String& EquipType=String() );
		bool SetData( long ItemNo, const StringSplitter<>& EquipType );
		long ItemID;
		String ItemName;
		String MagicType;
		long ItemType;
		long Price;
		bool bEquipOK;
	};
	struct ItemList : public std::list<ShopData>
	{
		iterator AddList( const ShopData& Source );
	};

	bool CheckShopItemType( short Type ) const;
	ItemList GetShopList( long ItemType, const String& EquipType=String() ) const;
	ItemList GetHaveItemList( long ItemType, const String& EquipType=String() ) const;
	long SetShopData( const String& ItemStr );
	bool SetEmployJob( const String& Jobs );

	bool EmployMember( MemberStatus& Output, long Level=0 );
	bool SetDefaultWepon( MemberStatus& State ) const;
	bool UnEquipAll( MemberStatus& State ) const;

	long AutoBuyMember( MemberStatus& Data, long& Money ) const;
protected:
	bool GetPowItem( LPCTSTR Name, long Pow ) const;

	bool HightPriceWepon( const ItemStatus& RefItem, const ItemStatus& NewItem ) const;
	bool HightPriceBody( const ItemStatus& RefItem, const ItemStatus& NewItem, const StringSplitter<>& EquipType ) const;
	MemberStatus CreateEmployMember( long PasonID, long Level ) const;
	bool SetItemInfo( const XMLPaser& ReadXML, LPCTSTR Name, std::vector<long>& Output ) const;
	std::list<long> ShopList;
	std::set<long> AddMinor;

	std::vector<long> DefaultWepon;
	std::vector<long> PowItem;
	String EmployJobs;
};

ShopSystem& GetShop();

#endif // !defined(AFX_SHOPSYSTEM_H__1EC4821B_33C6_467D_A520_3701A7236B8C__INCLUDED_)
