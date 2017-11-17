// ShopSystem.cpp: ShopSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Path.h"
#include "RbCache.h"
#include "RbMember.h"
#include "RbGameData.h"
#include "ShopSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ShopSystem::ShopSystem()
{
	EmployJobs = _T("default");
}

ShopSystem::~ShopSystem()
{
}

bool ShopSystem::LoadSystemXML( const XMLPaser& ReadXML )
{
	SetItemInfo( ReadXML, _T("wepon"), DefaultWepon );
	SetItemInfo( ReadXML, _T("pow"), PowItem );
	return true;
}

bool ShopSystem::SetItemInfo( const XMLPaser& ReadXML, LPCTSTR Name, std::vector<long>& Output ) const
{
	String Items = ReadXML.AttributeString( Name );
	Output.clear();
	StringSplitter<> Split;
	Split.SetString( Items );

	long Count = Split.Count();
	Output.resize( Count );
	for( int i = 0; i < Count; i++ )
	{
		Output[i] = Split[i].ToLong();
	}
	return true;
}

bool ShopSystem::EquipList( const MemberStatus& MemState, StringSplitter<>& Output )
{
	RbMember Mem(MemState);
	Output.SetString( Mem.GetEquip() );
	return true;
}

bool ShopSystem::EquipOK( const String& ItemType, const MemberStatus& MemState )
{
	StringSplitter<> Split;
	if( !EquipList( MemState, Split ) )return false;
	return (Split.Find( ItemType ) >= 0);
}

bool ShopSystem::EquipOK( long ItemNum, const String& EquipType )
{
	return EquipOK( GetCache().Item.Get(ItemNum).Type, EquipType );
}

bool ShopSystem::EquipOK( const String& ItemType, const String& EquipType )
{
	StringSplitter<> Split(EquipType);
	return EquipOK( ItemType, Split );
}

bool ShopSystem::EquipOK( const String& ItemType, const StringSplitter<>& EquipType )
{
	if( ItemType.empty() )return true;
	return (EquipType.Find( ItemType ) >= 0);
}

bool ShopSystem::ShopData::SetData( long ItemNo, const String& EquipType )
{
	StringSplitter<> Split(EquipType);
	return SetData( ItemNo, Split );
}

bool ShopSystem::ShopData::SetData( long ItemNo, const StringSplitter<>& EquipType )
{
	const ItemCache& Cache = GetCache().Item;
	const ItemStatus& Ref = Cache.Get( ItemNo );
	ItemID = ItemNo;
	ItemName = Ref.Name;
	Price = Ref.Price;
	ItemType = Ref.id.EquipType;
	MagicType = Ref.MagicType;
	if( EquipType.Count() <= 0 )bEquipOK = true;
	else bEquipOK = EquipOK( Ref.Type, EquipType );
	return true;
}

ShopSystem::ItemList::iterator ShopSystem::ItemList::AddList( const ShopData& Source )
{
	iterator itr = begin();
	while( itr != end() )
	{
		const ShopData& Ref = *itr;
		if( Ref.bEquipOK == Source.bEquipOK )
		{
			if( Source.Price >= Ref.Price )break;
		}
		else if( Ref.bEquipOK == false )break;
		itr++;
	}
	return insert( itr, Source );
}

long ShopSystem::SetShopData( const String& ItemStr )
{
	StringSplitter<> Split(ItemStr);
	long Count = Split.Count();
	for( int i = 0; i < Count; i++ )
	{
		const String& Ref = Split[i];
		if( Ref.empty() )
		{
			ShopList.clear();
			continue;
		}
		long ItemNum = ::atol( Ref );
		if( ItemNum == 0 )continue;
		ShopList.push_back( ItemNum );
	}
	return ShopList.size();
}

bool ShopSystem::CheckShopItemType( short Type ) const
{
	const ItemCache& Cache = GetCache().Item;
	std::list<long>::const_iterator itr = ShopList.begin();
	while( itr != ShopList.end() )
		if( Cache.Get(*itr++).id.EquipType == Type )return true;
	return false;
}

ShopSystem::ItemList ShopSystem::GetShopList( long ItemType, const String& EquipType ) const
{
	ItemList RetList;
	StringSplitter<> Split(EquipType);

	std::list<long>::const_iterator itr = ShopList.begin();
	while( itr != ShopList.end() )
	{
		ShopData AddData;
		if( !AddData.SetData(*itr++,Split) )continue;
		if( AddData.ItemType != ItemType )continue;
		RetList.AddList( AddData );
	}
	return RetList;
}

ShopSystem::ItemList ShopSystem::GetHaveItemList( long ItemType, const String& EquipType ) const
{
	ItemList RetList;
	StringSplitter<> Split(EquipType);

	typedef std::map<long,HaveItems> HaveList;
	const HaveList& Items = GetGameData().RefHaveItems();
	HaveList::const_iterator itr = Items.begin();
	while( itr != Items.end() )
	{
		ShopData AddData;
		long ItemNum = (itr++)->second.id;
		if( !AddData.SetData(ItemNum,Split) )continue;
		if( AddData.ItemType != ItemType )continue;
		RetList.AddList( AddData );
	}
	return RetList;
}

bool ShopSystem::SetEmployJob( const String& Jobs )
{
	const PasonCache& Pason = GetCache().Pason;
	StringSplitter<> Split( Jobs );
	if( Split.Count() <= 0 )return true;
	for( int i = 0; i < Split.Count(); i++ )
	{
		const String& Ref = Split[i];
		if( Ref.empty() )
		{
			AddMinor.clear();
			EmployJobs.erase();
		}
		else
		{
			long PasonID = Pason.GetPasonID( Ref );
			if( 0 < PasonID )AddMinor.insert( PasonID );
			else
			{
				if( !EmployJobs.empty() )EmployJobs += _T("/");
				EmployJobs += Ref;
			}
		}
	}
	return true;
}

bool ShopSystem::EmployMember( MemberStatus& Output, long Level )
{
	RbGameData& UserData = GetGameData();
	if( Level <= 0 )Level = UserData.RefSystemData().MonsterLevel;

	const PasonCache::MinorMap& RefMinor = GetCache().Pason.RefMinor();

	std::vector<long> EmployList;
	EmployList.reserve( RefMinor.size() + AddMinor.size() );

	PasonCache::MinorMap::const_iterator itr = RefMinor.begin();
	while( itr != RefMinor.end() )
	{
		const long& PasonID = *itr++;
		if( !UserData.InMember(PasonID) )EmployList.push_back(PasonID);
	}
	std::set<long>::const_iterator itr2 = AddMinor.begin();
	while( itr2 != AddMinor.end() )
	{
		const long& PasonID = *itr2++;
		if( RefMinor.find(PasonID)==RefMinor.end() )
			if( !UserData.InMember(PasonID) )EmployList.push_back(PasonID);
	}

	long Count = EmployList.size();
	if( Count <= 0 )return false;

	long PasonID = EmployList[Rnd(Count-1)];

	if( PasonID <= 0 )return false;
	Output = CreateEmployMember( PasonID, Level );
	return true;
}

MemberStatus ShopSystem::CreateEmployMember( long PasonID, long Level ) const
{
	const RbCache& Cache = GetCache();
	const PasonCache::PasonST& Pason = Cache.Pason.Get( PasonID );
	StringSplitter<> JobList( Pason.BaseJob );

	if( JobList.Count() <= 0 )JobList.SetString( EmployJobs );

	String Job = JobList[Rnd(JobList.Count()-1)];

	MemberStatus RetState;
	AllZero( RetState );
	RbMember NewMem( RetState );
	NewMem.Create( PasonID, Job, Level );

	for( int i = 0; i < Pason.EmployEquip.size(); i++ )
	{
		const ItemStatus& Item = Cache.Item.Get( Pason.EmployEquip[i] );
		NewMem.SetItem( Item.id );
	}
	SetDefaultWepon( RetState );
	return RetState;
}

bool ShopSystem::SetDefaultWepon( MemberStatus& State ) const
{
	if( State.Wepon.Number != 0 )return true;

	StringSplitter<> EquipType;
	if( !EquipList( State, EquipType ) )return false;

	const ItemCache& Cache = GetCache().Item;
	for( int i = 0; i < DefaultWepon.size(); i++ )
	{
		const ItemStatus& RefItem = Cache.Get( DefaultWepon[i] );
		if( RefItem.id.EquipType != 1 )continue;
		if( !EquipOK( RefItem.Type, EquipType ) )continue;
		State.Wepon = RefItem.id;
		return true;
	}
	return false;
}

bool ShopSystem::UnEquipAll( MemberStatus& State ) const
{
	ItemID NoItem = {0,0};
	RbGameData& UserData = GetGameData();

	UserData.AddItem( State.Wepon );
	State.Wepon = NoItem;
	UserData.AddItem( State.Body );
	State.Body = NoItem;
	UserData.AddItem( State.Acs );
	State.Acs = NoItem;

	GetPowItem( _T("lev"), State.Append.Level );
	GetPowItem( _T("hp"), State.Append.Status.Hp );
	GetPowItem( _T("mp"), State.Append.Status.Mp );
	GetPowItem( _T("atc"), State.Append.Status.Attack );
	GetPowItem( _T("def"), State.Append.Status.Defence );
	GetPowItem( _T("mag"), State.Append.Status.Magic );
	GetPowItem( _T("spd"), State.Append.Status.Speed );
	GetPowItem( _T("mov"), State.Append.Status.Move );
	AllZero( State.Append );

	return true;
}

bool ShopSystem::GetPowItem( LPCTSTR Name, long Pow ) const
{
	if( Pow <= 0 )return false;

	String ExName( _T("pow/") );
	ExName += Name;
	const ItemCache& Cache = GetCache().Item;
	std::vector<long>::const_iterator itr = PowItem.begin();
	while( itr != PowItem.end() )
	{
		const ItemStatus& Item = Cache.Get( *(itr++) );
		if( !Item.Ex.IsFirst(ExName) )continue;
		long ItemPow = StringSplitter<>(Item.Ex)[2].ToLong();
		if( ItemPow <= 0 )ItemPow = 1;
		Pow /= ItemPow;
		if( Pow <= 0 )Pow = 1;
		GetGameData().AddItem( Item.id, Pow );
		return true;
	}
	return false;
}

long ShopSystem::AutoBuyMember( MemberStatus& Data, long& Money ) const
{
	const ItemCache& Cache = GetCache().Item;
	RbMember State(Data);
	const ItemStatus* pWep = &(Cache.Get( Data.Wepon ));
	const ItemStatus* pBody = &(Cache.Get( Data.Body ));
	StringSplitter<> EquipType = State.GetEquip();

	const ItemStatus* pNewWep = pWep;
	const ItemStatus* pNewBody = pBody;
	std::list<long>::const_iterator itr = ShopList.begin();
	while( itr != ShopList.end() )
	{
		const ItemStatus& Item = Cache.Get(*itr++);
		if( HightPriceWepon( *pNewWep,Item ) )pNewWep = &Item;
		if( HightPriceBody( *pNewBody,Item,EquipType ) )pNewBody = &Item;
	}

	long RetCount = 0;
	if( pNewBody != pBody )
	{
		long Price = pNewBody->Price - (pBody->Price/2);
		if( Money >= Price )
		{
			Money -= Price;
			Data.Body = pNewBody->id;
			++RetCount;
		}
	}
	if( pNewWep != pWep )
	{
		long Price = pNewWep->Price - (pWep->Price/2);
		if( Money >= Price )
		{
			Money -= Price;
			Data.Wepon = pNewWep->id;
			++RetCount;
		}
	}
	return RetCount;
}

bool ShopSystem::HightPriceWepon( const ItemStatus& RefItem, const ItemStatus& NewItem ) const
{
	if( NewItem.Price <= RefItem.Price )return false;
	if( RefItem.Rare )return false;
	if( NewItem.Type != RefItem.Type )return false;
	if( NewItem.MagicType != RefItem.MagicType )return false;
	return true;
}

bool ShopSystem::HightPriceBody( const ItemStatus& RefItem, const ItemStatus& NewItem, const StringSplitter<>& EquipType ) const
{
	if( NewItem.Price <= RefItem.Price )return false;
	if( RefItem.Rare )return false;
	if( eBodyTypeItem != NewItem.id.EquipType )return false;
	if( !EquipOK(NewItem.Type,EquipType) )return false;
	return true;
}
