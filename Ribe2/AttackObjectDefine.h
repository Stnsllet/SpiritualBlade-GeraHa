// AttackObjectDefine.h: AttackObjectDefine クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKOBJECTDEFINE_H__479B4577_A739_4413_A785_255ED0BDFFBB__INCLUDED_)
#define AFX_ATTACKOBJECTDEFINE_H__479B4577_A739_4413_A785_255ED0BDFFBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
#include "ExCreator.h"
#include <map>
#include <vector>
class XMLPaser;

class AttackObjectDefine  
{
public:
	typedef ExCreator::BaseDef BaseDef;

	struct AtcDef
	{
		AtcDef(){AllZero(*this);}
		float UseMp;
	};
	AttackObjectDefine();
	virtual ~AttackObjectDefine();
	bool ReadDef();
	bool ReadDef( LPCTSTR FileName );
	const AtcDef& RefData( const String& Name ) const;
	const std::vector<RefPtr<BaseDef> >& RefAllEx() const{return ExList;}

protected:
	bool ReadAllExData( XMLPaser& ReadXML );
	bool ReadData( XMLPaser& ReadXML );
	bool ReadExData( XMLPaser& ReadXML );
	std::map<String,AtcDef> DefList;
	std::vector<RefPtr<BaseDef> > ExList; 
	AtcDef DefaultData;
};

#endif // !defined(AFX_ATTACKOBJECTDEFINE_H__479B4577_A739_4413_A785_255ED0BDFFBB__INCLUDED_)
