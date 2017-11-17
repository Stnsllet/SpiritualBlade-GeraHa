
#ifndef __LEARNING_SKILL__
#define __LEARNING_SKILL__

#include <vector>
#include "Statusstruct.h"
class XMLPaser;

struct LearningSkill
{
	struct Data
	{
		NamedID name;
		long Level;
	};
	bool SetXML( const XMLPaser& ReadXML );
	bool AddSkill( const XMLPaser& ReadXML );
	long Count() const{return SkillList.size();}
	const Data& operator []( int Index ) const{return SkillList[Index];}
	std::vector<Data> SkillList;
};

#endif //__LEARNING_SKILL__
