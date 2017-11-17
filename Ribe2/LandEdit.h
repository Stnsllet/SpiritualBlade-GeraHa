// LandEdit.h: LandEdit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDEDIT_H__4D0256EF_8460_4A13_BE3D_86481EBC561F__INCLUDED_)
#define AFX_LANDEDIT_H__4D0256EF_8460_4A13_BE3D_86481EBC561F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include "LandScape.h"

class LandEdit  : public LandScape
{
public:
	enum eEditMode
	{
		eNoneEdit = 0,
		eEditView,
		eChipSet,
		eSetHight,
		ePaintSet,
	};
	struct EditInfo
	{
		eEditMode Mode;
		MapID Chip;
		String SinplePlane;
		float LastHight;
		bool IsEditMode() const;
		bool EditMode();
		bool SetChip( const MapID& No, const String& PlaneName );
		bool SetPaintMode();
		bool SetHightMode();
	}Edit;
	LandEdit();
	virtual ~LandEdit();
	virtual HRESULT Draw( Display& Disp );
	virtual bool MouseJob( Display& Disp, GameMessage& State );
	bool EditMouseJob( Display& Disp, GameMessage& State, bool bShiftKey );

	HRESULT CreatePlane( int x, int y, float ChipX, float ChipY );
	bool ChangeMap( Display& Disp, long FaceNum, const MapID& New );
	long PaintMap( Display& Disp, long FaceNum, const MapID& New );
	HRESULT LoadLandData(){return LoadLandScape(String(LandFilePath));}
	HRESULT SaveLandData(){return SaveLandData(LandFilePath);}
	HRESULT SaveLandData( LPCTSTR FileName );
	virtual HRESULT LoadLandScape( LPCTSTR FileName );

	void SetCursor( float x, float y ){CursorFace=GetFacePosition(x,y);}
	bool SetEditMapID( long ID ){LastSelectID=MapID(ID);return true;}
	bool RandomHight(int min=10, int max=200);
private:
	struct VertexFaces
	{
		VertexFaces(){AllZero(*this);}
		VertexFaces& operator +=( int NewFaceNum )
		{
			ATLASSERT( Count < count_of(FaceNum) );
			FaceNum[Count++] = NewFaceNum;
			return *this;
		}
		long Count;
		int FaceNum[6];
	};

	void ChangeAlignHight( long VertexNum, float AddHight );
	long ChangeHight( long VertexNum, float AddHight );
	bool SetNormal( long IndexNum ) const;
	long GetVertexes( long VertexNum, std::set<int>& Output ) const;
	int GetVertexPosition( const D3DVECTOR& Pos ) const;
	VertexFaces GetFaces( long VertexNum ) const;
	RbPosition GetNormalize( VertexFaces& Faces ) const;
	void CreateSinplePlane( const String& PlaneName );
	void CreateFaceNormal();
	void SetNormal();

	HRESULT SaveGlobalData( File& fp ) const;
	HRESULT SaveSinplePlane( File& fp ) const;
	HRESULT SaveBasePlane( File& fp ) const;

	MapID LastSelectID;
	float ChipHight;
	float AlignHight;
	long LastVertexNum;
	float LastHight;

	CComPtr<MYTEXTURE> pCursorTexture;
	int CursorFace;
};

LandEdit& GetLandEdit();

#endif // !defined(AFX_LANDEDIT_H__4D0256EF_8460_4A13_BE3D_86481EBC561F__INCLUDED_)
