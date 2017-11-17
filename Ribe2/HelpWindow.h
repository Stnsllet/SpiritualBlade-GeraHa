// HelpWindow.h: HelpWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPWINDOW_H__6811945D_23AE_420B_96D1_93A73AEA2269__INCLUDED_)
#define AFX_HELPWINDOW_H__6811945D_23AE_420B_96D1_93A73AEA2269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "String.h"
#include "BaseWindow.h"

class HelpWindow : public BaseWindow  
{
public:
	HelpWindow();
	virtual ~HelpWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	bool SetHelp( LPCTSTR Infomation, LPCTSTR Help );
	bool AddSkill( LPCTSTR Name, long Level );
	bool AddSkill( long SkillID, long Level );
	bool SetHide( bool bSet=true ){return (bHide=bSet);}
	bool SetSourceRect( const RbRect& Ref ){HelpSourceRect = Ref;return true;}
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );

private:
	static LPCTSTR HelpFont;
	static LPCTSTR InfoFont;
	void SetStringSize( WindowSystem& System, RbRect& TextRect );
	double StartTime;
	String HelpString;
	String InfoString;
	RbRect SkillRect;
	struct SkillDef
	{
		SkillDef(){AllZero(*this);}
		SkillDef(long SkillID,long SkillLevel){ID=SkillID;Level=SkillLevel;}
		long ID;
		long Level;
	};
	RbRect HelpSourceRect;
	std::vector<SkillDef> SkillList;
};

#endif // !defined(AFX_HELPWINDOW_H__6811945D_23AE_420B_96D1_93A73AEA2269__INCLUDED_)
