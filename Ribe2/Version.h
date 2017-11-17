
#ifndef __RB_VERSION__
#define __RB_VERSION__

struct Version
{
	Version( char M, char M2, short D )
	{
		Main = M;
		Minor = M2;
		Debug = D;
	};
	char Main;
	char Minor;
	short Debug;
	bool operator == (const Version& Ver ) const
	{
		if( Main != Ver.Main )return false;
		if( Minor != Ver.Minor )return false;
		return true;
	}
	bool operator != (const Version& Ver ) const
	{
		return !(*this==Ver);
	}
	long ToMinor() const
	{
		return Main*256 + Minor;
	}
};

#endif //__RB_MATERIAL__
