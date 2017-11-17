// ResultWindow.h: ResultWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTWINDOW_H__51CCEED0_C797_47DE_A4C8_B45A0E504B7F__INCLUDED_)
#define AFX_RESULTWINDOW_H__51CCEED0_C797_47DE_A4C8_B45A0E504B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbBuffer.h"
#include "TitleWindow.h"

class ResultWindow : public TitleWindow  
{
public:
	ResultWindow();
	virtual ~ResultWindow();

	virtual bool SetXML( const XMLPaser& MsgXML );
protected:
	bool AddResultMsg( const XMLPaser& MsgXML );
	String FormatMoney( LPCTSTR Default, const XMLPaser& MsgXML, long Money ) const;
	struct ResultDef
	{
		bool SetResult();
		long Medical;
		long Kill;
		long Bonus;
		long MaxCombo;
		RbBuffer<long> Items;
	}Result;
};

#endif // !defined(AFX_RESULTWINDOW_H__51CCEED0_C797_47DE_A4C8_B45A0E504B7F__INCLUDED_)
