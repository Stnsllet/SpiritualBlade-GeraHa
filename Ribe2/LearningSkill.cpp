
#include "stdafx.h"
#include "XMLList.h"
#include "LearningSkill.h"

bool LearningSkill::SetXML( const XMLPaser& ReadXML )
{
	XMLList SkillXMLList;
	SkillXMLList.Query( ReadXML, _T("Skill") );
	long Count = SkillXMLList.GetCount();
	if( Count <= 0 )return false;

	SkillList.reserve( SkillList.size() + Count );
	for( long i = 0; i < Count; i++ )
	{
		AddSkill( SkillXMLList.GetItem(i) );
	}
	return true;
}

bool LearningSkill::AddSkill( const XMLPaser& ReadXML )
{
	Data AddData;
	AddData.name = String(ReadXML.AttributeString(TAG::NAME));
	if( AddData.name.IsNone() )return false;

	AddData.Level = ReadXML.AttributeLong(TAG::LEV);
	if( AddData.Level <= 0 )AddData.Level = 1;
	SkillList.push_back( AddData );

	return true;
}
