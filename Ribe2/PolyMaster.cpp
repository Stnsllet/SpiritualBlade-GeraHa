// PolyMaster.cpp: PolyMaster クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolyMaster.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PolyMaster::PolyMaster()
{

}

PolyMaster::~PolyMaster()
{
}

void PolyMaster::Clear()
{
	PolyList.clear();
}

HRESULT PolyMaster::DrawPoly( Display& Disp )
{
	PolyData::iterator itr = PolyList.begin();
	while( itr != PolyList.end() )
	{
		(*itr++)->Draw( Disp );
	}
	return S_OK;
}

PolyStation* PolyMaster::AddXML( const XMLPaser& PolyXML, const LandScape& Land )
{
	RefPtr<PolyStation> pStation(new PolyStation );
	if( !pStation->SetXML( PolyXML, Land ) )return NULL;
	PolyList.push_back( pStation );
	return pStation;
}

PolyStation* PolyMaster::AddStation( const String& PolyName, const LandScape& Land,
	const RbPosition& Pos, float Direction, float Scale, float Angle, bool bRevDraw )
{
	RefPtr<PolyStation> pStation(new PolyStation );
	if( !pStation->SetStation( PolyName, Land, Pos, Direction, Scale, Angle, bRevDraw ) )return NULL;
	PolyList.push_back( pStation );
	return pStation;
}

bool PolyMaster::Pin( const RbPosition& Pos, float Size ) const
{
	int Count = PolyList.size();
	for( int i = 0; i < Count; i++ )
	{
		if( PolyList[i]->Pin( Pos, Size ) )return true;
	}
	return false;
}

bool PolyMaster::Pout( const RbPosition& Pos, float Size ) const
{
	return (BlockCheck(Pos,Size)==NULL);
}

const PolyStation* PolyMaster::BlockCheck( const RbPosition& Pos, float Size ) const
{
	int Count = PolyList.size();
	for( int i = 0; i < Count; i++ )
	{
		if( PolyList[i]->Pout( Pos, Size ) )return PolyList[i];
	}
	return NULL;
}

long PolyMaster::Delete( const RbPosition& Pos )
{
	long Count = 0;
	PolyData::iterator itr = PolyList.begin();
	while( itr != PolyList.end() )
	{
		if( (*itr)->Pout( Pos, 0 ) )
		{
			PolyList.erase(itr);
			++Count;
		}
		else ++itr;
	}
	return Count;
}

long PolyMaster::Delete( const PolyStation* pStation )
{
	PolyData::iterator itr = PolyList.begin();
	while( itr != PolyList.end() )
	{
		if( (*itr) != pStation )++itr;
		else
		{
			PolyList.erase(itr);
			return 1;
		}
	}
	return 0;
}

float PolyMaster::CheckHight( float x, float y, float z ) const
{
	if( PolyList.size() <= 0 )return z;

	PolyData::const_iterator itr = PolyList.begin();
	RbPosition Pos(x,y,z);
	while( itr != PolyList.end() )
	{
		const PolyStation& Ref = *(*itr++);
		if( !Ref.Pout( Pos, 0 ) )continue;
		z = max(z,Ref.Hight(x,y));
	}
	return z;
}
