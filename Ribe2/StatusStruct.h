
#ifndef __STATUS_STRUCT__
#define __STATUS_STRUCT__

#include <set>
#include "String.h"

#ifndef count_of
#define count_of(x) (sizeof(x)/sizeof(x[0]))
#endif //count_of

enum ItemTypeEnum
{
	eNoneTypeItem = 0,
	eWeponTypeItem = 1,
	eBodyTypeItem = 2,
	eAcsTypeItem = 3,
	ePowTypeItem = 4,
	eEventTypeItem = 5,

	eTypeItemMax,
};

struct NamedID
{
	char name[16];
	operator LPCTSTR() const{return name;}
	operator String() const{return name;}
	NamedID& operator = ( LPCSTR NameID )
	{
		::strncpy( name, NameID, count_of(name) );
		name[15] = 0;
		return *this;
	}
	bool operator == ( LPCSTR NameID ) const
	{
		if( NameID == NULL )return IsNone();
		return (0==::strncmp(name, NameID, count_of(name)) );
	}
	bool IsNone() const{return (name[0] == 0);}
};

typedef std::set<NamedID> NameIDSet;

struct TexRect
{
	int x, y, w, h;
};

struct HandTexRect : public TexRect
{
	int px, py;
	float angle;
};

struct DefaultStatus
{
	float Hp;
	float Mp;

	float Attack;
	float Defence;
	float Magic;
	float Speed;
	float Move;

	DefaultStatus& operator *= (float x )
	{
		Hp *= x;
		Mp *= x;
		Attack *= x;
		Defence *= x;
		Magic *= x;
		Speed *= x;
		Move *= x;
		return *this;
	}

	DefaultStatus& operator *= (const DefaultStatus& Add)
	{
		Hp *= Add.Hp;
		Mp *= Add.Mp;
		Attack *= Add.Attack;
		Defence *= Add.Defence;
		Magic *= Add.Magic;
		Speed *= Add.Speed;
		Move *= Add.Move;
		return *this;
	}

	DefaultStatus& operator += (const DefaultStatus& Add)
	{
		Hp += Add.Hp;
		Mp += Add.Mp;
		Attack += Add.Attack;
		Defence += Add.Defence;
		Magic += Add.Magic;
		Speed += Add.Speed;
		Move += Add.Move;
		return *this;
	}
	DefaultStatus& operator -= (const DefaultStatus& Add)
	{
		Hp -= Add.Hp;
		Mp -= Add.Mp;
		Attack -= Add.Attack;
		Defence -= Add.Defence;
		Magic -= Add.Magic;
		Speed -= Add.Speed;
		Move -= Add.Move;
		return *this;
	}
	DefaultStatus operator + (const DefaultStatus& Add) const
	{
		DefaultStatus Ret = *this;
		return (Ret += Add);
	}
};

struct SkillID
{
	NamedID name;
	char Level;
};

struct ItemID
{
	short EquipType;
	short Number;
	operator long() const{return Number;}
	long MakeLong() const{return ((long)EquipType<<16)+Number;}
	void SetLong( long Value )
	{
		Number = Value&((1<<16)-1);
		EquipType = Value>>16;
	}
};

struct SkillEffect
{
	SkillEffect(){AllZero(*this);}
	float Pow;
	float Range;
	long Count;
	operator float() const{return Pow;}
	operator long() const{return Count;}
};

struct ItemStatus
{
	ItemStatus(){AllZero(*this);}
	String Name;
	String Help;
	String Type;
	String MagicType;
	String Ex;
	DefaultStatus Status;

	HandTexRect TexInfo;
	CComPtr<MYTEXTURE> pTexture;
	SkillEffect Effect;

	ItemID id;
	bool Rare;
	long Price;

	SkillID Skill[4];
};

struct AppendItem
{
	long Level;
	DefaultStatus Status;
};

struct GrowthStatus
{
	DefaultStatus Status;
//	String GrowType;
};

enum BattleAlgoType
{
	eBatDefault = 0,
	eBatDontMove,
	eBatNotBat,
	eBatRaging,
	eBatEscape,

	eBatMax,
};

struct JobStatus
{
	String NameID;
	String Name;
	String Help;
	GrowthStatus Grow;

	String Texture;
	String EquipType;
	BattleAlgoType BattleType;
	long WeponRandom;
	String MoveType;
	String NextJob;
};

struct PasonStatus
{
	String NameID;
	String Name;
	String Help;
	String News;

	bool bMain;
	String BaseJob;
	String EquipType;
	String Texture;
	String MoveType;

	DefaultStatus FirstStatus;
	GrowthStatus Grow;
};

struct MemberStatus
{
	DefaultStatus Status;

	NamedID PasonName;
	NamedID JobName;

	long AllLevel;
	long Level;
	long Exp;
	long Kill;

	ItemID Wepon;
	ItemID Body;
	ItemID Acs;

	AppendItem Append;

	SkillID Skill[8];

};

enum BattleAction
{
	eActNone = -1,
	eActStop = 0,
	eActWait,
	eActAttack,
	eActMove,

	eActDamege,
	eActSpecial,
	eActRecover ,
	eActDead ,

	eActMax
};

enum TacticsAlgoType
{
	eTacticsRndMove = -1,
	eTacticsNone = 0,
	eTacticsAttack,
	eTacticsSyncAttack,
	eTacticsHelp,

	eTacticsMax,
};

struct HaveItems
{
	long id;
	long Count;
};

struct UserStatus
{
	NamedID NowPos;
	long Reserve;
	long Money;
	double BattleTime;
	long MaxCombo;

	BYTE EventFlag[2048/8];	//256

};

struct PatyStatus
{
	TacticsAlgoType Algo;
	float EscapeHp;
	struct PatyMember
	{
		NamedID id;
		float x, y;
	}Member[8];
};

#endif // __STATUS_STRUCT__

