// LandEdit.cpp: LandEdit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "NormalEditer.h"
#include "StringSplitter.h"
#include "SplashWindow.h"
#include "GameSystem.h"
#include "LandEdit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

bool LandEdit::EditInfo::IsEditMode() const
{
	return (Mode!=LandEdit::eNoneEdit);
}
bool LandEdit::EditInfo::EditMode()
{
	Mode = LandEdit::eEditView;
	return true;
}

bool LandEdit::EditInfo::SetChip( const MapID& No, const String& PlaneName )
{
	LandType& Cache = GetCache().Land;
	if( PlaneName.empty() )
	{
		SinplePlane = Cache.GetTypeData( No ).Type;
	}
	else SinplePlane = PlaneName;

	Chip = No;
	Mode = LandEdit::eChipSet;
	return true;
}

bool LandEdit::EditInfo::SetHightMode()
{
	Mode = LandEdit::eSetHight;
	LastHight = 0;
	return true;
}

bool LandEdit::EditInfo::SetPaintMode()
{
	Mode = LandEdit::ePaintSet;
	return true;
}

LandEdit::LandEdit()
{
	LandFilePath = _T("Land/Test.lnd");
	AllZero( LastSelectID );
	ChipHight = 6.0f;
	AlignHight = 48.0f;
	LastVertexNum = -1;
	CursorFace = -1;
	LastHight = 0;
	Edit.Mode = eNoneEdit;
}

LandEdit::~LandEdit()
{
}

HRESULT LandEdit::Draw( Display& Disp )
{
	HRESULT hr = LandScape::Draw( Disp );
	if( FAILED(hr) )return hr;
	if( !Edit.IsEditMode() )return S_OK;
	// カーソル描画
	if( CursorFace >= 0 && CursorFace < IndexData.Count() )
	{
		if( pCursorTexture == NULL )
			pCursorTexture = GetCache().PathTex.Get(_T("Tex/Poly/cursor.png"));
		LandBuffer CursorVer;
		CursorVer.Allocate(Disp,3);
		LandBuffer::Locker Cursor( CursorVer );
		{
			IndexBuffer::Locker Index( IndexData );
			LandBuffer::Locker Land( LandData );
			const VerIndex& TagetIndex = Index[CursorFace];
			Cursor[0] = Land[TagetIndex.a];
			Cursor[1] = Land[TagetIndex.b];
			Cursor[2] = Land[TagetIndex.c];
		}

		Disp->SetTexture( 0, pCursorTexture );
		HRESULT ret =  Disp->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			1,
			Cursor,
			CursorVer.StructSize() );
		if( FAILED(ret) )return ret;
	}

	return S_OK;
}

bool LandEdit::MouseJob( Display& Disp, GameMessage& State )
{
	if( !Edit.IsEditMode() )return LandScape::MouseJob( Disp, State );

	if( State == eMouseMove )SetCursor( State.ViewPos.x, State.ViewPos.y );

	bool bShiftKey = GetKeyState( VK_SHIFT )<0;
	switch( State.Msg() )
	{
	case eMouseMove:
		if( !State.LDown )break;
	case eLDown:
		if( !EditMouseJob( Disp, State, bShiftKey ) )break;
		return true;
	case eScrollUp:
	case eScrollDown:
		if( !bShiftKey )break;
		{
			long VertexNum = GetVertexPosition( State.ViewPos );
			float HightMove = (State==eScrollUp)?(ChipHight):(-ChipHight);
			ChangeAlignHight( VertexNum, HightMove );
			LastUpdateTime = RbTimer();
		}
		return true;
	}
	return LandScape::MouseJob( Disp, State );
}

bool LandEdit::EditMouseJob( Display& Disp, GameMessage& State, bool bShiftKey )
{
	const RbPosition& Pos = State.ViewPos;
	bool bPress = (State.Msg() != eLDown);
	long FaceNum = GetFacePosition( Pos.x, Pos.y );
	if( FaceNum < 0 )return false;

	LastUpdateTime = RbTimer();
	switch( Edit.Mode )
	{
	case eChipSet:
		if( Edit.Chip.Move <= 0 )break;
		ChangeMap( Disp, FaceNum, Edit.Chip );
		return true;
	case eSetHight:
		{
			long VertexNum = GetVertexPosition( Pos );
			if( VertexNum < 0 )break;
			float Hight = LandData.Get(VertexNum).z;
			if( !bPress )Edit.LastHight = Hight;
			else if( bShiftKey )LandBuffer::Locker(LandData)[VertexNum].z = Edit.LastHight;
			else ChangeAlignHight( VertexNum, Edit.LastHight-Hight );
		}
		return true;
	case ePaintSet:
		if( bPress )return false;
		PaintMap( Disp, FaceNum, Edit.Chip );
		return true;
	}
	return false;
}

// ランドスケープロード
HRESULT LandEdit::LoadLandScape( LPCTSTR FileName )
{
	if( FileName == NULL )return E_INVALIDARG;

	String Name = FileName;
	if( (long)Name.find(_T(".lnd"))>=0 )return LandScape::LoadLandScape( FileName );

	StringSplitter<> Split(FileName);
	if( Split.Count() < 3 )return E_INVALIDARG;

	RbPoint Face( Split[1].ToLong(), Split[2].ToLong() );
	RbPointF ChipSize( Split[3].ToFloat(), Split[4].ToFloat() );
	if( Face.x < 1 )Face.x = 1;
	if( Face.y < 1 )Face.y = 1;
	if( ChipSize.x < 1 )ChipSize.x = 48;
	if( ChipSize.y < 1 )ChipSize.y = 48;

	HRESULT ret = CreatePlane( Face.x, Face.y, ChipSize.x, ChipSize.y );
	if( FAILED(ret) )return ret;

	LandFilePath = _T("Land/");
	LandFilePath += Split[0];
	LandFilePath += _T(".lnd");

	LastUpdateTime = RbTimer();
	return S_OK;
}


HRESULT LandEdit::CreatePlane( int x, int y, float ChipX, float ChipY )
{
	if( !IsInit() )return E_FAIL;

	SplashMsg(_T("LandScape 作成中・・・"));
	ClearData();

	GData.Face.x = x*2; GData.Face.y = y;
	GData.ChipSize.x = ChipX; GData.ChipSize.y = ChipY;

	Display& Disp = GetRbDisplay();

	LandData.Allocate( Disp, (x+1)*(y+1) );
	IndexData.Allocate( Disp, x*y * 2 );

	LandBuffer::Locker LandLock(LandData);
	for( int j = 0; j < y+1; j++ )for( int i = 0; i < x+1; i++ )
	{
		int Pos = i+j*(x+1);
		ATLASSERT( Pos < LandData.Count() );
		LandVertex& Data = LandLock[Pos];
		Data.x = GData.ChipSize.x  * i + (j&1)*(GData.ChipSize.y/2.0f);
		Data.y = GData.ChipSize.y * j;
		Data.z = 0;
		Data.nx = 0;
		Data.ny = 0;
		Data.nz = 1.0f;
		Data.tu = (float)i+(j&1)*0.5f;
		Data.tv = (float)j;
	//	LandData[Pos].tu2 = LandData[Pos].tu;
	//	LandData[Pos].tv2 = LandData[Pos].tv;
	}
	IndexBuffer::Locker IndexLock(IndexData);
	for( j = 0; j < y; j++ )for( int i = 0; i < x; i++ )
	{
		int Pos = (i+j*x)*2;
		ATLASSERT( Pos < IndexData.Count() );

		VerIndex& Pos0 = IndexLock[Pos];
		VerIndex& Pos1 = IndexLock[Pos+1];
		int jy = x+1;
		if( !(j&1) )
		{
			Pos0.a   = i+   j*jy;
			Pos0.b   = i+1+ j*jy;
			Pos0.c   = i+   (j+1)*jy;
			Pos1.a = i+   (j+1)*jy;
			Pos1.b = i+1+ j*jy;
			Pos1.c = i+1+ (j+1)*jy;
		}
		else
		{
			Pos0.a   = i+   j*jy;
			Pos0.b   = i+1+ j*jy;
			Pos0.c   = i+1+ (j+1)*jy;
			Pos1.a = i+   (j+1)*jy;
			Pos1.b = i+   j*jy;
			Pos1.c = i+1+ (j+1)*jy;
		}
	}
	CreateFaceNormal();
	SetNormal();

	MapID NewID(1);

	MapData.Allocate( IndexData.Count() );
	for( int i = 0; i < MapData.Count(); i++ )MapData[i] = NewID;

	Map.SetMapData( GData.Face.x, GData.Face.y,
		GData.ChipSize.y, GData.ChipSize.y );

	String PlaneName = GetCache().Land.GetTypeData(NewID).Type;
	CreateSinplePlane( PlaneName );
	return S_OK;
}

bool LandEdit::SetNormal( long IndexNum ) const
{
	if( IndexNum >= IndexData.Count()
		|| IndexNum >= FaceNormal.Count() )return false;

	LandBuffer::Locker LandLock(LandData);
	IndexBuffer::Locker IndexLock(IndexData);
	const VerIndex& Source = IndexLock[IndexNum];

	LandVertex Data = LandLock[Source.a];
	D3DXVECTOR3 Ver1(Data.x, Data.y, Data.z);
	Data = LandLock[Source.b];
	D3DXVECTOR3 Ver2(Data.x, Data.y, Data.z);
	Data = LandLock[Source.c];
	D3DXVECTOR3 Ver3(Data.x, Data.y, Data.z);

	D3DXPLANE Plane;
	D3DXPlaneFromPoints(&Plane, &Ver1, &Ver2, &Ver3 );
//	D3DXPlaneNormalize(&Normal, &Plane);
	FaceNormal[IndexNum] = Plane;
	return true;
}

LandEdit::VertexFaces LandEdit::GetFaces( long VertexNum ) const
{
	VertexFaces RetFaces;
	IndexBuffer::Locker IndexLock(IndexData);
	int IndexCount = IndexData.Count();
	for( int i = 0; i < IndexCount; i++ )
	{
		const VerIndex& Ref = IndexLock[i];
		if( VertexNum== Ref.a
			|| VertexNum== Ref.b
			|| VertexNum== Ref.c )RetFaces += i;
		if( RetFaces.Count >= count_of(RetFaces.FaceNum) )break;
	}
	return RetFaces;
}

RbPosition LandEdit::GetNormalize( VertexFaces& Faces ) const
{
	RbPosition Normal(0,0,0);
	int NomalDataCount = FaceNormal.Count();
	int Count = 0;
	for( int i = 0; i < Faces.Count; i++ )
	{
		int FaceNum = Faces.FaceNum[i];
		if( FaceNum < NomalDataCount )
		{
			const PanelVector& Ref = FaceNormal[FaceNum];
			Normal.x += Ref.a;
			Normal.y += Ref.b;
			Normal.z += Ref.c;
			Count++;
		}
	}
	if( Count <= 0 )return RbPosition(0,-1,0);
	return (Normal /= Count );
}

long LandEdit::GetVertexes( long VertexNum, std::set<int>& Output ) const
{
	VertexFaces Faces = GetFaces( VertexNum );
	IndexBuffer::Locker IndexLock(IndexData);
	for( int i = 0; i < Faces.Count; i++ )
	{
		if( VertexNum == Faces.FaceNum[i] )continue;
		const VerIndex& Face = IndexLock[Faces.FaceNum[i]];
		Output.insert( Face.a );
		Output.insert( Face.b );
		Output.insert( Face.c );
	}
	return Output.size();
}

int LandEdit::GetVertexPosition( const D3DVECTOR& Pos ) const
{
	int FaceNum = GetFacePosition( Pos.x, Pos.y );
	if( FaceNum < 0 || FaceNum >= IndexData.Count()  )return -1;
	RbPosition LandPos = Pos;
	LandPos.z = GetHight( Pos.x, Pos.y );

	const VerIndex& Index = IndexData.Get(FaceNum);
	LandBuffer::Locker LandLock(LandData);

	float Length = LandPos.Length( LandLock[Index.a] );
	int RetNum = Index.a;
	if( Length >  LandPos.Length( LandLock[Index.b] ) )
	{
		RetNum = Index.b;
		Length = LandPos.Length( LandLock[Index.b] );
	}
	if( Length >  LandPos.Length( LandLock[Index.c] ) )RetNum = Index.c;
	return RetNum;
}

bool LandEdit::RandomHight(int min, int max)
{
	int VerCount = LandData.Count()-1;
	int UpCount = Rnd2(min,max);
	LandBuffer::Locker LandLock(LandData);
	for( int i = 0; i < UpCount; i++ )
	{
		LandLock[Rnd(VerCount)].z += Rnd(4)*ChipHight;
	}
	SetNormal();
	return true;
}

void LandEdit::ChangeAlignHight( long VertexNum, float AddHight )
{
	if( 0 >= ChangeHight( VertexNum, AddHight ) )return;
	LandBuffer::Locker LandLock(LandData);

	std::set<int> VertexNumSet;
	GetVertexes( VertexNum, VertexNumSet );
	float BaseZ = LandLock[ VertexNum ].z;

	std::set<int>::const_iterator itr = VertexNumSet.begin();
	for( ; itr != VertexNumSet.end(); itr++ )
	{
		LandVertex& VerData = LandLock[*itr];
		float DipZ = VerData.z - BaseZ;
		if( DipZ > AlignHight )
			ChangeAlignHight( *itr, AlignHight-DipZ );
		if( DipZ < -AlignHight )
			ChangeAlignHight( *itr, -AlignHight-DipZ );
	}
}

long LandEdit::ChangeHight( long VertexNum, float AddHight )
{
	if( AddHight == 0 )return 0;
	if( VertexNum >= LandData.Count() )return 0;
	LandBuffer::Locker LandLock(LandData);
	IndexBuffer::Locker IndexLock(IndexData);

	LandLock[ VertexNum ].z += AddHight;
	VertexFaces Faces = GetFaces( VertexNum );
	std::set<int> VertexNumSet;
	for( int i = 0; i < Faces.Count; i++ )
	{
		SetNormal( Faces.FaceNum[i] );
		const VerIndex& Face = IndexLock[Faces.FaceNum[i]];
		VertexNumSet.insert( Face.a );
		VertexNumSet.insert( Face.b );
		VertexNumSet.insert( Face.c );
	}
	std::set<int>::const_iterator itr = VertexNumSet.begin();
	while( itr != VertexNumSet.end() )
	{
		const int& Num = *(itr++);
		RbPosition Normal = GetNormalize( GetFaces(Num) );
		LandLock[Num].nx = Normal.x;
		LandLock[Num].ny = Normal.y;
		LandLock[Num].nz = Normal.z;
	}
	return VertexNumSet.size();
}

void LandEdit::CreateFaceNormal()
{
	SplashMsg(_T("平面情報を作成しています"));
	int Count = IndexData.Count();
	FaceNormal.Allocate(Count);
	for( int i = 0; i < Count; i++ ) SetNormal( i );
}

// 頂点を参照している面の平均を法線とする
void LandEdit::SetNormal()
{
	SplashMsg(_T("法線を再計算中です"));
	int Count = LandData.Count();
	int IndexCount = IndexData.Count();

	LandBuffer::Locker VData(LandData);
	IndexBuffer::Locker IData(IndexData);
	NormalEditer<LandVertex> Editer( VData, Count, true );
	for( int i = 0; i < IndexCount; i++ )
	{
		const VerIndex& Ref = IData[i];
		Editer.SetNormal( Ref.a, Ref.b, Ref.c );

		const LandVertex& Ver1 = VData[Ref.a];
		const LandVertex& Ver2 = VData[Ref.b];
		const LandVertex& Ver3 = VData[Ref.c];
		D3DXPLANE Plane;
		D3DXPlaneFromPoints(&Plane, &Ver1, &Ver2, &Ver3 );
	//	D3DXPlaneNormalize(&Normal, &Plane);
		FaceNormal[i] = Plane;
	}
	Editer.End();
}

void LandEdit::CreateSinplePlane( const String& PlaneName )
{
	SinplePlane& NewPlane = GetSinplePlane( PlaneName );
	NewPlane.IndexData = IndexData;
}

bool LandEdit::ChangeMap( Display& Disp, long FaceNum, const MapID& New )
{
	if( FaceNum < 0 )return false;

	if( FaceNum >= MapData.Count() )return false;

	ATLASSERT( FaceNum < IndexData.Count() );
	VerIndex Index = IndexData.Get(FaceNum);

	if( !Edit.SinplePlane.empty() )
	{
		std::map<String,SinplePlane>::iterator Itr = SinpleList.begin();
		for( ; Itr != SinpleList.end(); Itr++ )
		{
			SinplePlane& Ref = Itr->second;
			if( !Ref.IndexData.Delete( Index ) )continue;
			if( Ref.IndexData.Count() > 0 )break;
			SinpleList.erase( Itr );
			break;
		}
	//	ATLASSERT( Itr != SinpleList.end() );
		bool bTest = GetSinplePlane( Edit.SinplePlane ).IndexData.Add( Disp, Index );
		ATLASSERT( bTest );
	}

	MapData[FaceNum] = New;
	return true;
}

long LandEdit::PaintMap( Display& Disp, long FaceNum, const MapID& New )
{
	long Count = 0;
	if( New.Move == 0 )return 0;
	if( FaceNum < 0 )return Count;
	long Size = MapData.Count();
	if( FaceNum >= Size )return Count;

	MapID Source = MapData[FaceNum];
	if( Source.Move == New.Move )return Count;

	std::set<long> FaceNumSet;
	FaceNumSet.insert( FaceNum );
	do
	{
		long FaceNum = *(FaceNumSet.begin());
		FaceNumSet.erase( FaceNumSet.begin() );
		if( Source.Move != MapData[FaceNum].Move )continue;
		if( ChangeMap( Disp, FaceNum, New ) )
		{
			int X = FaceNum % GData.Face.x;
			int Y = FaceNum / GData.Face.x;
			if( X > 0 )FaceNumSet.insert( FaceNum-1 );
			if( X < GData.Face.x-1 )FaceNumSet.insert( FaceNum+1 );
			if( !((Y&1)^(X&1)) )
			{
				if( Y > 0 )FaceNumSet.insert( FaceNum-GData.Face.x );
			}else
			{
				if( Y < GData.Face.y-1 )FaceNumSet.insert( FaceNum+GData.Face.x );
			}
			Count++;
		}
	}while( FaceNumSet.begin() != FaceNumSet.end() );

	return Count;
}

// save
HRESULT LandEdit::SaveLandData( LPCTSTR FileName )
{
	File fp;
	HRESULT ret = fp.Open( FileName, true );
	if( FAILED(ret) )return ret;

	ret = SaveGlobalData( fp );
	if( FAILED(ret) )return ret;

	ret = SaveBasePlane( fp );
	if( FAILED(ret) )return ret;

	ret = SaveSinplePlane( fp );
	if( FAILED(ret) )return ret;

	SystemPopup(_T("save"));

	return S_OK;
}

// save
HRESULT LandEdit::SaveGlobalData( File& fp ) const
{
	HRESULT ret = fp.Write( eLandTypeGlobal );
	if( FAILED(ret) )return ret;

	ret = fp.Write( GData );
	if( FAILED(ret) )return ret;

	ret = MapData.Save( fp );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// save
HRESULT LandEdit::SaveBasePlane( File& fp ) const
{
	HRESULT ret = fp.Write( eLandTypeBasePlane );
	if( FAILED(ret) )return ret;

	ret = LandData.Save( fp );
	if( FAILED(ret) )return ret;

	ret = IndexData.Save( fp );
	if( FAILED(ret) )return ret;

	ret = FaceNormal.Save( fp );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// save
HRESULT LandEdit::SaveSinplePlane( File& fp ) const
{
	HRESULT ret = fp.Write( eLandTypeSinplePlane );
	if( FAILED(ret) )return ret;

	ret = fp.Write( (long)SinpleList.size() );
	if( FAILED(ret) )return ret;

	std::map<String,SinplePlane>::const_iterator itr = SinpleList.begin();
	while( itr != SinpleList.end() )
	{
		const SinplePlane& Ref = itr->second;

		ret = fp.WriteStr( itr->first );
		if( FAILED(ret) )return ret;

		ret = Ref.IndexData.Save( fp );
		if( FAILED(ret) )return ret;

		itr++;
	}

	return S_OK;
}
