// DataUpdater.h: DataUpdater クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAUPDATER_H__51A9358E_4172_41A9_BA39_5A0A50497CAC__INCLUDED_)
#define AFX_DATAUPDATER_H__51A9358E_4172_41A9_BA39_5A0A50497CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Statusstruct.h"
class RbGameData;

class DataUpdater  
{
public:
	DataUpdater();
	DataUpdater( const String& PasonName );
	virtual ~DataUpdater();

	bool SetData( const String& PasonName );
	bool ClassChange( const String& NewClass );
	bool AddSkill( const String& Skill, long Level );
protected:
	MemberStatus Mem;
	RbGameData& UserData;
};

#endif // !defined(AFX_DATAUPDATER_H__51A9358E_4172_41A9_BA39_5A0A50497CAC__INCLUDED_)
