// ItemCache.cpp: ItemCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "Plane.h"
#include "ItemCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ItemCache::ItemCache()
{

}

ItemCache::~ItemCache()
{

}

// データ取得
const ItemStatus& ItemCache::Get( long ItemNum ) const
{
	ItemMap::const_iterator itr = ItemList.find(ItemNum);
	ATLASSERT( itr != ItemList.end() );
	if( itr == ItemList.end() )return DefaultItem;
	return itr->second;
}

// テクスチャ取得
RefBord* ItemCache::GetTexture( long ItemNum )
{
	RefPtr<RefBord>& pRef = BillBordList[ItemNum];
	if( pRef != NULL )return pRef;
	const ItemStatus& Data = Get( ItemNum );
	pRef = new RefBord( Data.TexInfo, Data.pTexture );
	return pRef;
}

// プレーンテクスチャ取得
Plane<HandTexRect> ItemCache::GetPlane( long ItemNum ) const
{
	const ItemStatus& Data = Get( ItemNum );
	Plane<HandTexRect> Ret( Data.TexInfo );
	Ret.pTexture = Data.pTexture;
	return Ret;
}

// 装備タイプ取得
short ItemCache::GetEquipType( const String& TypeName ) const
{
	return GetTypeData(TypeName).EquipType;
}

// 装備タイプ取得
const ItemCache::ItemType& ItemCache::GetTypeData( const String& TypeName ) const
{
	static ItemType DumType;
	TypeMap::const_iterator itr = TypeList.find(TypeName);
	ATLASSERT( TypeName == NULL || itr != TypeList.end() );
	if( itr == TypeList.end() )return AllZero(DumType);
	return itr->second;
}

// 名前テクスチャを読む
HRESULT ItemCache::ReadNamedPlane( DISPDEVICE* pDevice, const XMLPaser& PlaneXML )
{
	HRESULT hr = NamedPlane.LoadPlane( pDevice, PlaneXML );
	if( FAILED(hr) )return hr;
	return S_OK;
}

// Type と Item データを読む
HRESULT ItemCache::ReadItemData( DISPDEVICE* pDevice )
{
	HRESULT ret = ReadItemType();
	if( FAILED(ret) )return ret;

	return ReadItemFiles( pDevice, _T("Data/Item.xml") );
}

HRESULT ItemCache::ReadItemFiles( DISPDEVICE* pDevice, LPCTSTR FileName )
{
	ItemList.clear();

	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( FileName );
	if( FAILED(ret) )return ret;

	ret = ReadNamedPlane( pDevice, XMLFile );
	if( FAILED(ret) )return ret;

	XMLList FileList;
	ret = FileList.Query( XMLFile, _T("ItemFile") );

	int Count = FileList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		String Path = FileList.GetItem(i).AttributeString(TAG::PATH);
		ReadItemFile( Path );
	}

	XMLList ItemList;
	ret = ItemList.Query( XMLFile, _T("Item") );

	Count = ItemList.GetCount();
	for( i = 0; i < Count; i++ )
	{
		ReadItemData( ItemList.GetItem(i) );
	}

	return S_OK;
}

// ItemFile を読み込み
HRESULT ItemCache::ReadItemFile( LPCTSTR FileName )
{
	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList ItemList;
	ret = ItemList.Query( XMLFile, _T("Item") );
	if( FAILED(ret) )return ret;

	int Count = ItemList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadItemData( ItemList.GetItem(i) );
	}

	return S_OK;
}

// Type データを読む
HRESULT ItemCache::ReadItemType()
{
	return ReadItemType( _T("Data/TypeName.xml") );
}

LPCTSTR ItemCache::BaseTypeNames[] = {_T("All"),_T("Wepon"),_T("Body"),_T("Acs"),_T("Pow"),_T("Event")};
long ItemCache::BaseTypeCount = count_of(BaseTypeNames);

// タイプ取得
short ItemCache::GetBaseType( const String& Name )
{
	for( int i = 0; i < BaseTypeCount; i++ )
		if( Name == BaseTypeNames[i] )return i;
	return -1;
}

// Type データを読む
HRESULT ItemCache::ReadItemType( LPCTSTR FileName )
{
	TypeList.clear();

	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( FileName );
	if( FAILED(ret) )return ret;

	for( int j = 0; j < BaseTypeCount; j++ )
	{
		XMLPaser SingleType;
		ret = XMLFile.Find( BaseTypeNames[j], SingleType );
		if( FAILED(ret) )continue;

		XMLList TypeList;
		ret = TypeList.Child( SingleType );
		if( FAILED(ret) )continue;

		int Count = TypeList.GetCount();
		for( int i = 0; i < Count; i++ )
		{
			ReadItemType( (short)j , TypeList.GetItem(i) );
		}
	}
	return S_OK;
}

// Type データを読む
HRESULT ItemCache::ReadItemType( short Type, const XMLPaser& TypeData )
{
	ItemType ReadData;
	ReadData.EquipType = Type;

	ReadData.NameID = TypeData.AttributeString( TAG::NAME );
	if( ReadData.NameID.length() <= 0 )return E_FAIL;

	ReadData.Name = TypeData.AttributeString( TAG::STR );
	if( ReadData.Name.length() <= 0 )return E_FAIL;

	ATLASSERT( TypeList.find(ReadData.NameID) == TypeList.end() );

	TypeList[ReadData.NameID] = ReadData;
	return S_OK;
}

// Item データを読む
HRESULT ItemCache::ReadItemData( const XMLPaser& ItemData )
{
	ItemStatus ReadData;
	ReadData.id.Number = ItemData.AttributeLong( _T("id") );
	ReadData.Type = ItemData.AttributeString(TAG::TYPE );
	ReadData.id.EquipType = GetEquipType( ReadData.Type );

	ReadData.Name = ItemData.AttributeString( TAG::STR );
	ReadData.Help = ItemData.AttributeString( TAG::HELP );

	String Tex;
	XMLPaser Data;
	HRESULT ret = ItemData.Find( _T("Data"), Data );
	if( SUCCEEDED(ret) )
	{
		StatusEdit<> Read(ReadData.Status);
		Read.ReadXML( Data );
		ReadData.Price = Data.AttributeLong(_T("price"));
		ReadData.Rare = Data.AttributeBool(_T("rare"));
		ReadData.Effect.Range = Data.AttributeFloat(_T("len"));
		ReadData.Effect.Pow = Data.AttributeFloat(TAG::POW);
		if( ReadData.Effect.Pow <= 0 )ReadData.Effect.Pow = 1.0f;
		ReadData.Effect.Count = Data.AttributeFloat(TAG::COUNT);
		if( ReadData.Effect.Count <= 0 )ReadData.Effect.Count = 1;
		ReadData.MagicType = Data.AttributeString(_T("mtype"));
		ReadData.Ex = Data.AttributeString(_T("ex") );

		Tex = Data.AttributeString(_T("tex"));
	}
	if( Tex.empty() )Tex = ReadData.Type;
	Plane<HandTexRect> FindPlane = NamedPlane.GetPlane(Tex);
	ReadData.TexInfo = FindPlane;
	ReadData.pTexture = FindPlane;
	if( ReadData.id.Number == 0 )DefaultItem = ReadData;
	else
	{
		if( ReadData.Help.empty() )ReadData.Help = DefaultItem.Help;
	}

	XMLList SkillListXML;
	SkillListXML.Query( ItemData, _T("Skill") );
	long Count = min(SkillListXML.GetCount(), (long)count_of(ReadData.Skill) );
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser SkillXML = SkillListXML.GetItem(i);
		ReadData.Skill[i].name = String(SkillXML.AttributeString(TAG::NAME));
		ReadData.Skill[i].Level = SkillXML.AttributeLong(_T("level"));
	}

	if( ReadData.id.Number > 0 && ReadData.Price <= 0 )
		ReadData.Price = GetPrice( ReadData );
	ATLASSERT( ItemList.find(ReadData.id.Number) == ItemList.end() );
	ItemList[ReadData.id.Number] = ReadData;
	return S_OK;
}

// 標準金額取得
long ItemCache::GetPrice( const ItemStatus& Item ) const
{
	DefaultStatus State = Item.Status;
	if( !Item.MagicType.empty() )State.Magic *= 2;
	State *= State;
	long Price = -Item.Price;
	Price += State.Attack+0.5f;
	Price += State.Defence+0.5f;
	Price += State.Magic+0.5f;
	Price += State.Speed+0.5f;
	Price += State.Hp+0.5f;
	Price += State.Mp+0.5f;

	for( int i = 0; i < count_of(Item.Skill); i++ )
	{
		const SkillID& Ref = Item.Skill[i];
		if( Ref.name.IsNone() )break;
		Price += (Ref.Level*Ref.Level)*2000;
	}

	long Count = 0;
	do
	{
		Price = (Price+5)/10;
		++Count;
	}while( Price >= 1000 );
	while( (Count--)>0 )Price *= 10;
	Price = (Price/20)*20;
	if( Price <= 0 )Price = 20;
	return Price;
}
