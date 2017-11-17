// SelectMsgWindow.cpp: SelectMsgWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "RbSound.h"
#include "RbGameData.h"
#include "RbEventSystem.h"
#include "SelectMsgWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SelectMsgWindow::SelectMsgWindow()
{
}

SelectMsgWindow::~SelectMsgWindow()
{
}

LPCTSTR SelectMsgWindow::WindowName() const
{
	return _T("SelectMsg");
}

bool SelectMsgWindow::SetXML( const XMLPaser& MsgXML )
{
	XMLList SelectXMLList;
	SelectXMLList.Query( MsgXML, _T("Com") );
	long Count = SelectXMLList.GetCount();
	bTextRead = (Count<=0);

	if( !MsgWindow::SetXML( MsgXML ) ) return false;

	SelectList.clear();
	SelectList.reserve( Count );
	for( long i = 0; i < Count; i++ )
	{
		AddSelectItem( ParamXML(SelectXMLList.GetItem(i)) );
	}
	Plane<> Face = GetFace();
	BoundRect = BoundCheck( Face );
	if( Count > 0 )GetSoundSystem().Play(_T("question"));

	return true;
}

bool SelectMsgWindow::AddSelectItem( const XMLPaser& AddXML )
{
	const RbGameData& UserData = GetGameData();
	String FlgOn = AddXML.AttributeString(TAG::ON);
	if( !UserData.CheckFlgs( FlgOn, true ) )return false;
	String FlgOff = AddXML.AttributeString(TAG::OFF);
	if( !UserData.CheckFlgs( FlgOff, false ) )return false;

	SelectData AddData;
	AddData.Msg = AddXML.AttributeString(TAG::MSG);
	if( AddData.Msg.empty() )return false;

	AddData.XML.XMLRead( AddXML );
	AddData.Flag = AddXML.AttributeString(TAG::FLG);

	const RbPoint& Chip = ChipSize();
	AddData.MsgRect = GetMsgRect( AddData.Msg );
	AddData.MsgRect.Move( Chip.x, Chip.y );

	SelectList.push_back( AddData );
	return true;
}

RbRect SelectMsgWindow::BoundCheck( const TexRect& RefFace )
{
	RbRect Bound = BaseMsgRect;
	int MaxWidth = Bound.Width();
	long Count = SelectList.size();
	for( long i = 0; i < Count; i++ )
	{
		MaxWidth = max( SelectList[i].MsgRect.Width(), MaxWidth );
	}

	long Trm = (Count>0)?(4):(0);
	BaseMsgRect.ReSize( MaxWidth, BaseMsgRect.Height()+Trm );
	int Height = BaseMsgRect.Height();
	for( i = 0; i < Count; i++ )
	{
		RbRect& Ref = SelectList[i].MsgRect;
		Ref.ReSize( MaxWidth, Ref.Height() );
		Ref.Move( 0, Height );
		Height += Ref.Height();
		Bound.OR( Ref );
	}

	if( RefFace.h > 0 )
	{
		RbRect FaceRect = GetFaceRect(RefFace);
		FaceRect.ReSize( FaceRect.Width()+2, FaceRect.Height() );

		long MoveX = RefFace.w+2;
		long MoveY = Bound.Center().y;
		Bound.OR( FaceRect );
		MoveY = Bound.Center().y - MoveY;

		BaseMsgRect.Move( MoveX, MoveY );
		Bound.OR( BaseMsgRect );
		for( i = 0; i < Count; i++ )
			SelectList[i].MsgRect.Move( MoveX, MoveY );
	}
	return Bound;
}

// レストア
HRESULT SelectMsgWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	if( Font.empty() )Font = MsgFont;

	const RbPoint& Chip = ChipSize();
	Position.Width = BoundRect.Width() + Chip.x*2;
	Position.Height = BoundRect.Height() + Chip.y*2;

	ExSprite Surface;
	HRESULT ret = RestoreMsgBase( pDevice, Surface, System );
	if( FAILED(ret) )return ret;
	DrawFace( Surface );

	DMsg( System, Message, BaseMsgRect );

	long Count = SelectList.size();
	for( long i = 0; i < Count; i++ )
	{
		const SelectData& Ref = SelectList[i];
		DMsg( System, Ref.Msg, Ref.MsgRect );
	}
	return S_OK;
}

// カーソルチェック
RbRect SelectMsgWindow::HitCursor( WindowSystem& System, int x, int y )
{
	long Count = SelectList.size();
	for( long i = 0; i < Count; i++ )
	{
		const RbRect& Ref = SelectList[i].MsgRect;
		if( Ref.Pin( x, y ) )return Ref;
	}
	return RbRect();
}

bool SelectMsgWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	if( SelectList.size() <= 0 )return MsgWindow::MouseJob( System, State );
	switch( State.Msg() )
	{
	case eLDown: case eLDblClick:
		if( ClickJob( State ) )Command( NULL, System, eDestroy );
	case eRDblClick: 
		return true;
	}

	return MsgWindow::MouseJob( System, State );
}

bool SelectMsgWindow::ClickJob( const GameMessage& State ) const
{
	int x = State.x - Position.x;
	int y = State.y - Position.y;
	long Count = SelectList.size();
	for( long i = 0; i < Count; i++ )
	{
		const SelectData& Ref = SelectList[i];
		if( !Ref.MsgRect.Pin( x, y ) )continue;
		if( !Ref.Flag.empty() )GetGameData().SetFlg( Ref.Flag );
		GetEventSystem().ChildEvent( Ref.XML );
		return true;
	}
	return false;
}
