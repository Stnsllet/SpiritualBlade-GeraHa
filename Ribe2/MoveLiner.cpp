// MoveLiner.cpp: MoveLiner クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PaformanceChecker.h"
#include "RbPosition.h"
#include "LandScape.h"
#include "BattleSystem.h"
#include "MoveLiner.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MoveLiner::MoveLiner( const LandScape& RefLand, float CheckSize )
: Land( RefLand ), Size( CheckSize ), MinLength( 1<<30 )
{
}

MoveLiner::~MoveLiner()
{

}

MoveLiner::MoveStop MoveLiner::MovingPos( const RbPosition& DestPos, const RbPosition& SrcPos, float Size ) const
{
	float CheckLength = Size/2;

	RbPosition Vector = DestPos - SrcPos;
	float Length = Vector.Length();
	if( Length <= 0 )return MoveStop(DestPos,NULL);

	Vector.Normalize( CheckLength );

	MoveStop RetStop;
	RetStop.Pos = SrcPos;
	RetStop.pStation = NULL;
	MoveStop CheckPos = RetStop;
	do
	{
		CheckPos.Pos += Vector;
		RetStop.pStation = Land.BlockCheck( CheckPos.Pos, Size );
		if( RetStop.pStation != NULL )
		{
			if( NULL != Land.BlockCheck( RetStop.Pos, Size*1.2f ) && RetStop.Pos != SrcPos )
			{	// ぎりぎりのときはちょっと戻る
				Vector /= 2;
				RetStop.Pos -= Vector;
			}
			return RetStop;
		}
		RetStop = CheckPos;
		Length -= CheckLength;
	}while( Length > 0 );

	return MoveStop(DestPos,NULL);
}

RbPosition MoveLiner::UnBlockPos( const RbPosition& BasePos, const PolyStation& Block ) const
{
	static float dips = 2/12.0f; 
	const RbPosition& BlockPos = Block.RefPos();
	RbPosition BaseVector = BlockPos - BasePos;
	BaseVector.Normalize( Size );
	for( int i = 1; i < 6; i++ )
	{
		RbPosition Pos = BaseVector;
		Pos.Rotate( dips*i );
		Pos += BasePos;
		if( NULL == Land.BlockCheck( Pos, Size ) )return Pos;
		Pos = BaseVector;
		Pos.Rotate( -dips*i );
		Pos += BasePos;
		if( NULL == Land.BlockCheck( Pos, Size ) )return Pos;
	}
	return BasePos;
}

bool MoveLiner::GetMoveList( Move::List& Output, const RbPosition& DestPos, const RbPosition& SrcPos )
{
	if( !Land.Map.MoveOK( DestPos.x, DestPos.y, Size ) )return false;
	TagetPos = DestPos;

	Output.Clear();
	bool bRet = _GetMoveList( Output, SrcPos );
	if( !Output )return false;
	return (DestPos.NiarXY2( Output.last(), Size/2.0f ));
}

bool MoveLiner::_GetMoveList( Move::List& Output, const RbPosition& SrcPos )
{
	Move::List NewList;
//	if( !Land.MoveOK( SrcPos, Size/2 ) )
//	{
//		NewList.AddNext( TagetPos );
//		Output.Insert( NewList );
//		return true;
//	}
	MoveStop StopPos = MovingPos( TagetPos, SrcPos, Size );
	if( StopPos.pStation == NULL )
		//|| !Land.MoveOK( TagetPos, Size ) )
	{
		NewList.AddNext( StopPos.Pos );
		Output.Insert( NewList );
		return true;
	}
	PEFORMANCE("Move Liner Check");

	Checker.SetCheckRange( Size/2.1f );

	RefPtr<Move::BlockChain> pBase( new Move::BlockChain );
	pBase->AddBlock( SrcPos, NULL, StopPos.pStation );
	Checker.CheckIn( pBase );

	RefPtr<Move::BlockChain> pFirstChain( new Move::BlockChain );
	pFirstChain->AddBlock( StopPos.Pos, pBase, StopPos.pStation );

	if( StopPos.Pos == SrcPos )
	{
	//	RbPosition Vector = StopPos.pStation->UnBlockPos( SrcPos, Size*1.2f );
		RbPosition Vector = UnBlockPos( SrcPos, *StopPos.pStation );

		RefPtr<Move::BlockChain> pChain( new Move::BlockChain );
		pChain->AddBlock( Vector, pBase, StopPos.pStation );
		Checker.CheckIn( pChain );
		pFirstChain = pChain;
	}
	Checker.CheckIn( pFirstChain );

	bool bGet = _GetMoveList( Output, pFirstChain );
	Output.pick(); // Drop Base

	return bGet;
}

bool MoveLiner::_GetMoveList( Move::List& Output, Move::BlockChain* pFirstChain )
{
	ChainList List1;
	ChainList List2;
	List1.push_back( pFirstChain );

	bool bFlip=false;
	do
	{
		bFlip = !bFlip;
		ChainList& Edit = (bFlip)?(List2):(List1);
		const ChainList& Source = (bFlip)?(List1):(List2);
		Edit.clear();
		ChainList::const_iterator itr = Source.begin();
		while( itr != Source.end() )
		{
			RefPtr<Move::BlockChain> Ref = *itr++;
			CheckBlock( Ref->Pos, *Ref->pBlock, Edit, Ref );
		}

	}while( ((bFlip)?(List2):(List1)).size() > 0 );

	RefPtr<Move::Chain> pGetChain = Checker.GetTagetChain(TagetPos);
	if( pGetChain == NULL )pGetChain = pFirstChain;
	ShortCutAll( pGetChain );
	if( pGetChain != NULL )pGetChain->AddList( Output );

	return true;
}

bool MoveLiner::CheckBlock( const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain )
{
	if( pBaseChain->RefLength() > MinLength )return false;
	float TSize = (Size*1.2f) / Block.RefScale();
	RbPosition LT = Block.RefRect().LT()+RbPointF(-TSize,-TSize);
	RbPosition LB = Block.RefRect().LB()+RbPointF(-TSize,TSize);
	RbPosition RT = Block.RefRect().RT()+RbPointF(TSize,-TSize);
	RbPosition RB = Block.RefRect().RB()+RbPointF(TSize,TSize);

	PointEntry( LT, SourcePos, Block, Output, pBaseChain );
	PointEntry( LB, SourcePos, Block, Output, pBaseChain );
	PointEntry( RT, SourcePos, Block, Output, pBaseChain );
	PointEntry( RB, SourcePos, Block, Output, pBaseChain );
	if( _PointEntry( TagetPos, SourcePos, Block, Output, pBaseChain ) )
	{
		RefPtr<Move::BlockChain> pChain = Output.back();
		if( pChain->RefLength() < MinLength &&
			pChain->Pos.NiarXY2(TagetPos,Size/2.0f) )MinLength = pChain->RefLength();
	}
	return true;
}

bool MoveLiner::PointEntry( const RbPosition& DestPos, const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain )
{
	return _PointEntry( Block.RefTrans().Trans(DestPos), SourcePos, Block, Output, pBaseChain );
}

bool MoveLiner::_PointEntry( const RbPosition& DestPos, const RbPosition& SourcePos, const PolyStation& Block, ChainList& Output, Move::Chain* pBaseChain )
{
	RefPtr<Move::BlockChain> pChain( new Move::BlockChain );
	pChain->AddBlock( DestPos, pBaseChain, &Block );
	if( !Checker.Check(pChain) )return false;

	MoveStop StopPos = MovingPos( DestPos, SourcePos, Size );
	if( !Land.Map.MoveOK( StopPos.Pos.x, StopPos.Pos.y, Size ) )return false;
	if( StopPos.pStation == NULL )StopPos.pStation = &Block;

	pChain->AddBlock( StopPos.Pos, pBaseChain, StopPos.pStation );
//	ShortCut( *pChain );
//	GetBattleSystem().BStatus.AddPoint( StopPos.Pos );
	if( !Checker.CheckIn(pChain) )return false;

	if( TagetPos == StopPos.Pos )return false;
	Output.push_back( pChain );
	return true;
}

bool MoveLiner::ShortCut( Move::BlockChain& ChainPos ) const
{
	if( ChainPos.pSource == NULL )return false;
	RefPtr<Move::Chain> pBase = ChainPos.pSource->pSource;
	if( pBase == NULL )return false;

	MoveStop StopPos = MovingPos( ChainPos.Pos, pBase->Pos, Size+2.0f );
	if( StopPos.pStation != NULL )return false;

	ChainPos.AddBlock( ChainPos.Pos, pBase, ChainPos.pBlock );
	return true;
}

bool MoveLiner::ShortCut( Move::Chain& ChainPos ) const
{
	if( ChainPos.pSource == NULL )return false;
	RefPtr<Move::Chain> pBase = ChainPos.pSource->pSource;
	if( pBase == NULL )return false;

	MoveStop StopPos = MovingPos( ChainPos.Pos, pBase->Pos, Size);//+2.0f );
	if( StopPos.pStation != NULL )return false;

	ChainPos.AddChain( ChainPos.Pos, pBase );
	return true;
}

bool MoveLiner::ShortCutAll( Move::Chain* pChain ) const
{
	RefPtr<Move::Chain> pTaget = pChain;
	while( pTaget != NULL )
	{
		if( !ShortCut( *pTaget ) )pTaget = pTaget->pSource;
	}
	return true;
}
