
#ifndef __MEMBER_CALLBACKS__
#define __MEMBER_CALLBACKS__

class RbActStatus;

interface MemCallBack
{
	virtual bool Call( RbActStatus& State ) = 0;
};

#endif //__MEMBER_CALLBACKS__
