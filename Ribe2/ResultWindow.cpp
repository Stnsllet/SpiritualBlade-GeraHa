// ResultWindow.cpp: ResultWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "BattleSystem.h"
#include "RbCache.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "ResultWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ResultWindow::ResultWindow()
{
	Result.SetResult();
}

ResultWindow::~ResultWindow()
{
}

bool ResultWindow::SetXML( const XMLPaser& MsgXML )
{
	if( !TitleWindow::SetXML( MsgXML ) )return false;

	long OldLength = TitleMsg.length();
	XMLList ResultMsgList;
	ResultMsgList.Query( MsgXML, _T("Result") );
	long Count = ResultMsgList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		AddResultMsg( ParamXML(ResultMsgList.GetItem(i)) );
	}
	if( Count > 0 && OldLength == TitleMsg.length() )return false;

	return true;
}

String ResultWindow::FormatMoney( LPCTSTR Default, const XMLPaser& MsgXML, long Money ) const
{
	String Msg = MsgXML.AttributeString(TAG::MSG);
	if( Msg.empty() )Msg = Default;
	LPCTSTR Unit = GetGameData().RefSystemData().MoneyUnit;

	String Ret;
	Ret.Format( _T("%s %6d %s "), (LPCTSTR)Msg, Money, Unit );

	return Ret;
}

bool ResultWindow::AddResultMsg( const XMLPaser& MsgXML )
{
	String AddMsg;
	String Type = MsgXML.AttributeString(_T("type"));
	if( Type == _T("medical") )
	{
		AddMsg = FormatMoney( _T("Medical "), MsgXML, Result.Medical );
	}
	else if( Type == _T("ko") )
	{
		AddMsg = FormatMoney( _T("KO Bounus"), MsgXML, Result.Kill );
	}
	else if( Type == _T("bonus") )
	{
		if( Result.Bonus == 0 )return false;
		AddMsg = FormatMoney( _T("Bounus"), MsgXML, Result.Bonus );
	}
	else if( Type == _T("combo") )
	{
		String Msg = MsgXML.AttributeString(TAG::MSG);
		if( Msg.empty() )Msg = _T("MaxCombo ");
		AddMsg.Format(_T("%s%4d "), (LPCTSTR)Msg,Result.MaxCombo);
	}
	else if( Type == _T("msg") )
	{
		AddMsg = MsgXML.AttributeString(TAG::MSG);
	}
	else if( Type == _T("items") )
	{
		ItemCache& Cache = GetCache().Item;
		for( int i = 0; i < Result.Items.Count(); i++ )
		{
			if( !AddMsg.empty() )AddMsg += _T("\n");
			AddMsg += Cache.Get(Result.Items[i]).Name;
			AddMsg += _T(" ");
		}
	}

	if( AddMsg.empty() )return false;
	TitleMsg += _T("\n");
	TitleMsg += AddMsg;
	return true;
}

bool ResultWindow::ResultDef::SetResult()
{
	BattleSystem& System = GetBattleSystem();
	const BattleSystemStatus::ResultData& BResult = System.BStatus.RefResult();

	Medical = BResult.GetMedical( System );

	Kill = BResult.GetKill( System );

	Bonus = BResult.Bonus;
	MaxCombo = BResult.MaxCombo;

	Items = BResult.ItemList;
	return true;
}
