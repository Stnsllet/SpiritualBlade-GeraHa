// MoveLiner.h: MoveLiner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVELINER_H__190E10AF_2496_4EA9_848F_C540CA2414AC__INCLUDED_)
#define AFX_MOVELINER_H__190E10AF_2496_4EA9_848F_C540CA2414AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MoveList.h"
class LandScape;
class PolyStation;

class MoveLiner
{
public:
	MoveLiner( const LandScape& RefLand, float CheckSize );
	virtual ~MoveLiner();
//	RbPosition MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const;

	bool GetMoveList( Move::List& Output, const RbPosition& DestPos, const RbPosition& SrcPos );
protected:
	struct MoveStop
	{
		MoveStop(){pStation=NULL;}
		MoveStop(const RbPosition& Ref, const PolyStation* pBlock)
		{Pos = Ref;pStation=pBlock;}
		RbPosition Pos;
		const PolyStation* pStation;
	};
	typedef std::list<RefPtr<Move::BlockChain> > ChainList;

	RbPosition UnBlockPos( const RbPosition& BasePos, const PolyStation& Block ) const;
	MoveStop MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const;

	bool _GetMoveList( Move::List& Output, const RbPosition& SrcPos );
	bool _GetMoveList( Move::List& Output, Move::BlockChain* pFirstChain );
	bool CheckBlock( const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain );
	bool PointEntry( const RbPosition& DestPos, const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain );
	bool _PointEntry( const RbPosition& DestPos, const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain );

	bool ShortCut( Move::BlockChain& ChainPos ) const;
	bool ShortCut( Move::Chain& ChainPos ) const;
	bool ShortCutAll( Move::Chain* pChain ) const;
	const float Size;
	const LandScape& Land;
	Move::CheckPoint Checker;
	RbPosition TagetPos;
	float MinLength;
};

#endif // !defined(AFX_MOVELINER_H__190E10AF_2496_4EA9_848F_C540CA2414AC__INCLUDED_)
