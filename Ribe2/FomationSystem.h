// FomationSystem.h: FomationSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOMATIONSYSTEM_H__1AA20F94_2F42_494F_B0DC_0303CD2C53A3__INCLUDED_)
#define AFX_FOMATIONSYSTEM_H__1AA20F94_2F42_494F_B0DC_0303CD2C53A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RbPaty;
#include <list>

class FomationSystem
{
public:
	struct FormationData
	{
		float x,y;
		bool operator ==( const FormationData& Ref ) const
		{
			return (x==Ref.x && y==Ref.y);
		}
		bool operator < ( const FormationData& Ref ) const
		{
			return (x<Ref.x || y<Ref.y);
		}
	};

	FomationSystem();
	~FomationSystem();
	void SetPosition( const RbPaty& Ref );
	void SetPosition( float x, float y );

	FormationData GetPosition( float Range );

	static const FormationData AllForm[];
	static long GetAllFormCount();
private:
	std::list<FormationData> NowDataSet;
	bool CheckFormation( const FormationData& Ref ) const;
	static const FormationData LongDefault[];
	static const FormationData ShortDefault[];
};

#endif // !defined(AFX_FOMATIONSYSTEM_H__1AA20F94_2F42_494F_B0DC_0303CD2C53A3__INCLUDED_)
