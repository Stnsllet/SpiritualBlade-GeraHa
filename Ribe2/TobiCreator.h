
#ifndef __TOBI_CREATOR__
#define __TOBI_CREATOR__

#include "String.h"
#include "TobiObjectBase.h"

class TobiCreator
{
public:
	TobiCreator(){MpData.Level=MpData.Base=0;}
	virtual ~TobiCreator(){}
	virtual TobiObjectBase* Create() const{return NULL;}
	// Žg—p MP
	float UseMp( long Level ) const
	{
		return (1.0f+(MpData.Level*Level)) * MpData.Base;
	}
protected:
	struct MpDef
	{
		float Level;
		float Base;
	}MpData;

};

template<class T> class TobiCreatorT : public TobiCreator
{
public:
	TobiCreatorT(){}
	TobiCreatorT( float MpBase ){MpData.Base=MpBase;MpData.Level=0.05f;}
	virtual ~TobiCreatorT(){}
	virtual TobiObjectBase* Create() const
	{
		TobiObjectBase* pNew = new T;
		pNew->SetResource();
		return pNew;
	}
};

#endif // __TOBI_CREATOR__