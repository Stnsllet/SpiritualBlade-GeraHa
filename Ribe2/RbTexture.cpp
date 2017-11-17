// RbTexture.cpp: RbTexture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "XMLList.h"
#include "Path.h"
#include "RbCache.h"
#include "RbRect.h"
#include "RbPosition.h"
#include "RbSound.h"
#include "RbTexture.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbTexture::RbTexture()
{
	Hight = Size = 0;
	AllZero(BattleID);

	NEW_CHECK(this);
}

RbTexture::~RbTexture()
{
}

RefBord* RbTexture::TexturePlane::GetRefBord() const
{
	if( pRefBord != NULL )return pRefBord;
	const_cast<RefPtr<RefBord>&>(pRefBord) = new RefBord(Rect,Rect);
	return pRefBord;
}

// 左右反転
bool RbTexture::TexturePlane::Reverse()
{
	pRefBord = NULL;
	Rect.w = -Rect.w;
	Rect.angle = -Rect.angle;
	Rect.px = -Rect.px;
	return true;
}

RECT RbTexture::TexturePlane::BoundRect() const
{
	RbRect Source( Rect.px, Rect.py, Rect.w, Rect.h );
	Source.Move( -Rect.w/2, -Rect.h );

	RbPosition LT( Source.LT() );
	LT.Rotate( Rect.angle );
	RbRect Bound( LT.x, LT.y, 0,0 );

	RbPosition RT( Source.RT() );
	RT.Rotate( Rect.angle );
	Bound.AddPoint(RT.x, RT.y);

	RbPosition LB( Source.LB() );
	LB.Rotate( Rect.angle );
	Bound.AddPoint(LB.x, LB.y);

	RbPosition RB( Source.RB() );
	RB.Rotate( Rect.angle );
	Bound.AddPoint(RB.x, RB.y);

	return Bound;
}

// 左右反転
bool RbTexture::ActTime::Reverse()
{
	for( int i = 0; i < PlaneList.size(); i++ )
	{
		PlaneList[i].Reverse();
	}
	return true;
}

// 囲む RECT 取得
RECT RbTexture::ActTime::BoundRect() const
{
	if( PlaneList.size() == 0 )return RbRect();

	RbRect Bound = PlaneList[0].BoundRect();
	for( int i = 1; i < PlaneList.size(); i++ )
	{
		if( PlaneList[i].bItem )continue;
		Bound.OR( PlaneList[i].BoundRect() );
	}
	return Bound;
}

// 左右反転
bool RbTexture::Direction::Reverse()
{
	AtcInfo.AttackStart = -AtcInfo.AttackStart;
	AtcInfo.AttackEnd = -AtcInfo.AttackEnd;
	for( int i = 0; i < TimeList.size(); i++ )
	{
		TimeList[i].Reverse();
	}
	return true;
}

// 囲む RECT 取得
RECT RbTexture::Direction::BoundRect() const
{
	if( TimeList.size() == 0 )return RbRect();

	RbRect Bound = TimeList[0].BoundRect();
	for( int i = 1; i < TimeList.size(); i++ )
	{
		Bound.OR( TimeList[i].BoundRect() );
	}
	return Bound;
}

// 時間に入っているか？
bool RbTexture::ActTime::InTime( int Time ) const
{
	return (Min<=Time && Time<Max);
}

// 範囲に入っているか？
bool RbTexture::Direction::InDirection( const float& Dir ) const
{
	return (MinDir<=Dir && Dir<MaxDir);
}

// 登録されているか確認
bool RbTexture::IsRegister( LPCTSTR ActName ) const
{
	return IsRegister( GetCache().ActID.GetID(ActName) );
}

// 登録されているか確認
bool RbTexture::IsRegister( long ActID ) const
{
	if( ActionList.find( ActID ) == ActionList.end() )return false;
	return true;
}

// アクション ID 取得
long RbTexture::GetActID( BattleAction ActID ) const
{
	if( ActID < 0 || ActID >= count_of(BattleID) )return -1;
	return BattleID[ActID];
}

// 音を鳴らす
bool RbTexture::Sound( BattleAction ActID, const RbPosition& Pos )
{
	return Sound( GetActID(ActID), Pos );
}

// 音を鳴らす
bool RbTexture::Sound( long ActID, const RbPosition& Pos )
{
	std::map<long,Action>::iterator itr = ActionList.find( ActID );
	if( itr == ActionList.end() )return false;
	Action& Ref = itr->second;
	RbSound& SoundSys = GetSoundSystem();
	if( Ref.WaveID == 0 )Ref.WaveID = SoundSys.GetID(Ref.Sound);
	if( Ref.WaveID <= 0 )return false;
	SoundSys.Play( Ref.WaveID, Pos );
	return true;
}

// バトルアクション時間取得
int RbTexture::GetBattleActionTime( BattleAction ActID, float Direct ) const
{
	return GetActionTime( GetActID(ActID), Direct );
}

// アクション時間取得
int RbTexture::GetActionTime( LPCTSTR ActName, float Direct ) const
{
	return GetActionTime( GetCache().ActID.GetID(ActName) , Direct );
}

// アクション時間取得
int RbTexture::GetActionTime( long ActID, float Direct ) const
{
	return GetActionInfo( ActID, Direct ).MaxTime;
}

// アクション情報取得
RbTexture::AttackInfo RbTexture::GetActionInfo( long ActID, float Direct ) const
{
	const Direction* pDir = GetDirection( ActID, Direct );
	if( pDir == NULL )return AllZero(AttackInfo());

	return pDir->AtcInfo;
}

// バトルのIDで取得
const RbTexture::ActTime& RbTexture::GetBattleAction( BattleAction ActID, float Direct, DWORD Time ) const
{
	return GetAction( GetActID(ActID), Direct, Time );
}

// アクションテクスチャ取得
const RbTexture::ActTime& RbTexture::GetAction( LPCTSTR ActName, float Direct, DWORD Time ) const
{
	return GetAction( GetCache().ActID.GetID(ActName) , Direct, Time );
}

// アクションテクスチャ取得
const RbTexture::ActTime& RbTexture::GetAction( long ActID, float Direct, DWORD Time ) const
{
	const Direction* pDir = GetDirection( ActID, Direct );

	if( pDir == NULL )return Dummy;
	int Count = pDir->TimeList.size();
	if( Count <= 0 )return Dummy;

	const int& MaxTime = pDir->AtcInfo.MaxTime;
	if( MaxTime > 0 ) Time %= MaxTime;

	for( int i = 0; i < Count; i++ )
	{
		const RbTexture::ActTime& Ref = pDir->TimeList[i];
		if( Ref.InTime( Time ) )return Ref;
	}
	return pDir->TimeList[0];
}

// 方向情報取得
const RbTexture::Direction* RbTexture::GetDirection( long ActID, float Direct ) const
{
	std::map<long,Action>::const_iterator itr = ActionList.find( ActID );
	if( itr == ActionList.end() )return NULL;

	int Count = itr->second.DirList.size();
	for( int i = 0; i < Count; i++ )
	{
		const RbTexture::Direction& Ref = itr->second.DirList[i];
		if( Ref.InDirection( Direct ) )return &Ref;
	}
	return NULL;
}

// バトル ID 設定
bool RbTexture::SetBattleID( BattleAction ActID, LPCTSTR ActName )
{
	if( ActID < 0 || ActID >= count_of(BattleID) )return false;
	long ID = GetCache().ActID.GetID(ActName);
	if( !IsRegister( ID ) )return false;
	BattleID[ActID] = ID;
	return true;
}

HRESULT RbTexture::LoadRbTexture( DISPDEVICE* pDevice, LPCTSTR NameID )
{
	Path FilePath = _T("Tex/Chara");
	String FileName = NameID;
	FileName += _T(".xml");
	FilePath += FileName;

	HRESULT hr = LoadActionTexture( pDevice, FilePath );
	if( FAILED(hr) )return hr;

	MyName = NameID;

	return S_OK;
}

// テクスチャロード
HRESULT RbTexture::LoadActionTexture( DISPDEVICE* pDevice, LPCTSTR FileName )
{
	ActionList.clear();

	XMLPaser File;
	HRESULT ret = File.Load( FileName );
	ATLASSERT( !FAILED(ret) );
	if( FAILED(ret) )return ret;

	ret = ReadActionTexture( pDevice, File );
	ATLASSERT( !FAILED(ret) );

	const ActTime& Ref = GetAction( _T("image"), 1.25f, 0 );
	if( &Ref != &Dummy )Dummy = Ref;
	else
	{
		const ActTime& Ref = GetAction( _T("stop"), 1.25f, 0 );
		if( &Ref != &Dummy )Dummy = Ref;
		else
		{
			const ActTime& Ref = GetAction( _T("move"), 1.25f, 0 );
			if( &Ref != &Dummy )Dummy = Ref;
		}
	}

	NamedPlane.Clear();
	return ret;
}

// テクスチャロード
HRESULT RbTexture::ReadActionTexture( DISPDEVICE* pDevice, XMLPaser& File )
{
//	MyName = File.AttributeString( TAG::NAME );
//	if( MyName.empty() )return E_FAIL;

	Size = File.AttributeFloat( _T("size") );
	Hight = File.AttributeFloat( _T("hight") );

	HRESULT ret = NamedPlane.LoadPlane( pDevice, File );
	if( FAILED(ret) )return ret;

	XMLList ActionList;
	ret = ActionList.Query( File, _T("Action") );
	if( FAILED(ret) )return ret;

	int Count = ActionList.GetCount();
	for( int i = 0; i< Count; i++ )
	{
		ReadAction( ActionList.GetItem(i) );
	}

	return S_OK;
}

// アクションデータ読み取り
HRESULT RbTexture::ReadAction( const XMLPaser& ActionXML )
{
	Action ReadData;

	String Name = ActionXML.AttributeString( TAG::NAME );
	if( Name.empty() )return E_FAIL;

	String Source = ActionXML.AttributeString(TAG::SRC);
	if( !Source.empty() )
	{
		long ActID = GetCache().ActID.GetID( Source );
		std::map<long,Action>::const_iterator itr = ActionList.find( ActID );
		if( itr == ActionList.end() )return E_FAIL;
		ReadData = itr->second;
	}

	ReadData.Name = Name;
	ReadData.ActNumber = GetCache().ActID.GetID( Name );
	String Sound = ActionXML.AttributeString( "sound" );
	if( !Sound.empty() )ReadData.Sound = Sound;
	ReadData.WaveID = 0;

	XMLList DirList;
	HRESULT ret = DirList.Query( ActionXML, _T("Direction") );
	if( FAILED(ret) )return ret;

	int Count = DirList.GetCount();
	ReadData.DirList.reserve( Count );
	for( int i= 0; i< Count; i++ )
	{
		HRESULT ret = ReadDirection( DirList.GetItem(i), ReadData );
		if( FAILED(ret) )continue;
	}

	ActionList[ReadData.ActNumber] = ReadData;
	return S_OK;
}

// 方向情報を読み取り
HRESULT RbTexture::ReadDirection( const XMLPaser& DirXML, Action& Output ) const
{
	Direction ReadData;
	String Reverse = DirXML.AttributeString( _T("rev") );
	if( !Reverse.empty() )
	{
		float Rev = Reverse.ToFloat();
		for( int i = 0; i < Output.DirList.size(); i++ )
			if( Output.DirList[i].InDirection( Rev ) )
			{
				ReadData = Output.DirList[i];
				ReadData.Reverse();
			}
	}

	ReadData.MinDir = DirXML.AttributeFloat( _T("min") );
	ReadData.MaxDir = DirXML.AttributeFloat( _T("max") );

	ReadData.AtcInfo.AttackStart = DirXML.AttributeFloat( _T("as") );
	ReadData.AtcInfo.AttackEnd = DirXML.AttributeFloat( _T("ae") );
	ReadData.AtcInfo.bSlash = true;

	if( ReadData.AtcInfo.AttackStart == ReadData.AtcInfo.AttackEnd )
	{
		ReadData.AtcInfo.bSlash = false;
		ReadData.AtcInfo.AttackStart = ReadData.MinDir;
		ReadData.AtcInfo.AttackEnd = ReadData.MaxDir;
	}

	XMLList TimeList;
	HRESULT ret = TimeList.Query( DirXML, _T("Time") );
	if( FAILED(ret) )return ret;

	int Count = TimeList.GetCount();
	ReadData.TimeList.reserve( Count );
	for( int i= 0; i< Count; i++ )
	{
		ReadTimeData( TimeList.GetItem(i), ReadData );
	}
	Output.DirList.push_back( ReadData );
	return S_OK;
}

// 時間設定を読み取る
HRESULT RbTexture::ReadTimeData( const XMLPaser& TimeXML, Direction& Output ) const
{
	ActTime ReadData;
	ReadData.Min = TimeXML.AttributeLong( _T("min") );
	ReadData.Max = TimeXML.AttributeLong( _T("max") );

	if( ReadData.Max > Output.AtcInfo.MaxTime )
		Output.AtcInfo.MaxTime = ReadData.Max;

	XMLList PlaneList;
	HRESULT ret = PlaneList.Query( TimeXML, TAG::PLANE );
	if( FAILED(ret) )return ret;

	int Count = PlaneList.GetCount();
	for( int i= 0; i< Count; i++ )
	{
		ReadPlaneData( PlaneList.GetItem(i), ReadData );
	}
	long Sweep = TimeXML.AttributeLong( _T("sweep") );
	if( Sweep > 0 && Output.TimeList.size() > 0 )
	{
		long Last = Output.TimeList.size()-1;
		ActTime StartData = Output.TimeList[Last];
		CreateSweep( StartData, ReadData, Output, Sweep );
	}

	Output.TimeList.push_back( ReadData );
	return S_OK;
}

// 間を補完する
HRESULT RbTexture::CreateSweep( const ActTime& Start,  const ActTime& End, Direction& Output, long Sweep ) const
{
	long MinPlane = Start.PlaneList.size();
	if( MinPlane < End.PlaneList.size() )MinPlane = End.PlaneList.size();
	if( MinPlane <= 0 )return S_FALSE;

	if( Sweep <= 0 )return S_FALSE;
	long Dip = End.Min - Start.Max;
	if( Dip <= Sweep )return S_FALSE;

	for( long NowTime = 0; NowTime < Dip; NowTime += Sweep )
	{
		float Scale = (Sweep / (float)Dip )*(NowTime/Sweep);
		ActTime AddData;
		AddData = Start;
		for( int i = 0; i < MinPlane; i++ )
		{
			const TexturePlane& TexStart = Start.PlaneList[i];
			const TexturePlane& TexEnd = End.PlaneList[i];
			TexturePlane& TexAdd = AddData.PlaneList[i];
			TexAdd.Rect.angle += (TexEnd.Rect.angle - TexStart.Rect.angle)*Scale;
			TexAdd.Rect.px += (TexEnd.Rect.px - TexStart.Rect.px)*Scale;
			TexAdd.Rect.py += (TexEnd.Rect.py - TexStart.Rect.py)*Scale;
		}
		AddData.Min += NowTime;
		AddData.Max += NowTime+Sweep;
		if( AddData.Max > End.Min )AddData.Max = End.Min;
		Output.TimeList.push_back( AddData );
	}

	return S_OK;
}

// プレーンデータ読み取り
HRESULT RbTexture::ReadPlaneData( const XMLPaser& PlaneXML, ActTime& Output ) const
{
	TexturePlane ReadData;

	String Name = PlaneXML.AttributeString(TAG::NAME);
	if( !Name.empty() )
	{
		Plane<HandTexRect> HRect = NamedPlane.GetPlane( Name );
		ATLASSERT( HRect.pTexture != NULL );
		if( HRect.pTexture != NULL )
		{
			ReadData.Rect = HRect;
		}
		HRect.ReadHand( PlaneXML );
		ReadData.Rect.px += HRect.px;
		ReadData.Rect.py += HRect.py;
		ReadData.Rect.angle += HRect.angle;
	}
	else{
		ReadData.bItem = true;
		ReadData.Rect = Plane<HandTexRect>().ReadXML( PlaneXML );
	}

	Output.PlaneList.push_back( ReadData );

	return S_OK;
}
