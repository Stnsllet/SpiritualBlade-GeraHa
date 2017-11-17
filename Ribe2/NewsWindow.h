// NewsWindow.h: NewsWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWSWINDOW_H__A5CC8329_8C67_409A_BF40_68FDD2E70551__INCLUDED_)
#define AFX_NEWSWINDOW_H__A5CC8329_8C67_409A_BF40_68FDD2E70551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class NewsWindow : public BaseWindow  
{
public:
	NewsWindow();
	virtual ~NewsWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetDefaultPosition( WindowSystem& System );
	bool SetParameter( const CComVariant& Value );
	bool CheckParameter( const CComVariant& Value ) const;

	String ReadTitle( const String& NewsName ) const;
	const String& NewsTitle() const{return Title.Msg;}
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	bool ReadNews( WindowSystem& System, LPCTSTR FilePath );
	String GetFilePath( const String& name ) const;

	struct MsgData
	{
		bool SetXML( const XMLPaser& BaseXML, LPCTSTR Name );
		bool SetMSG( WindowSystem& System, const String& NewMsg );
		RbRect Area, MsgRect;
		String Msg;
		String Font;
	};
	Plane<> ImagePlane;
	RbRect ImageRect;
	MsgData Msg, Title;
	String NewsName;
};

#endif // !defined(AFX_NEWSWINDOW_H__A5CC8329_8C67_409A_BF40_68FDD2E70551__INCLUDED_)
