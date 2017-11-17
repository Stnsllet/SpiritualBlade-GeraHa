
#ifndef __MOVE_LIST__
#define __MOVE_LIST__

#include <list>
#include "RefObject.h"
#include "RbPoint.h"
#include "RbPosition.h"
class PolyStation;


struct Move
{
	typedef RbPosition MovePointer;
	class List
	{
	public:
		void Clear()
		{
			PosList.clear();
		}
		MovePointer next() const
		{
			if( !(*this) )return AllZero(MovePointer());
			return PosList.front();
		}
		MovePointer last() const
		{
			if( !(*this) )return AllZero(MovePointer());
			return PosList.back();
		}
		MovePointer pick()
		{
			MovePointer NextPos = next();
			PosList.pop_front();
			return NextPos;
		}
		List& AddNext( const MovePointer& Data )
		{
			PosList.push_back( Data );
			return *this;
		}
		List& Append( const List& Data )
		{
			if( !Data )return *this;
			if( !(*this) )
			{
				PosList = Data.PosList;
				return *this;
			}
			std::list<MovePointer>::const_iterator itr = Data.PosList.begin();
			while( itr != Data.PosList.end() )
				if( last().NiarXY2(*itr,4) )itr++;
				else break;
			while( itr != Data.PosList.end() )
				PosList.push_back( *itr++ );
			return *this;
		}
		List& Insert( const List& Data )
		{
			if( !Data )return *this;
			if( !(*this) )
			{
				PosList = Data.PosList;
				return *this;
			}

			std::list<MovePointer>::iterator additr = PosList.begin();
			std::list<MovePointer>::const_iterator itr = Data.PosList.begin();
			while( itr != Data.PosList.end() )
			{
				MovePointer& NextPos = *additr;
				const MovePointer& AddPos = *itr++;
				if( NextPos == AddPos )break;
				additr = PosList.insert( additr, AddPos );
				additr++;
			}
			return *this;
		}
		bool operator !() const
		{
			return ( PosList.size() <= 0 );
		}
	private:
		std::list<MovePointer> PosList;
	};

	class Chain : public RefAuto
	{
	public:
		Chain(){NEW_CHECK(this);}
		void AddChain(const MovePointer& AddPos, Chain* pSrc )
		{
			Pos = AddPos;
			if( pSrc != NULL )
			{
				pSource = pSrc;
				Length = pSrc->Length + AddPos.LenXY( pSrc->Pos );
			}
			else Length = 0;
		}
		bool operator < ( const Chain& Ref ) const
		{
			return (Length < Ref.Length);
		}
		void AddList( List& Data ) const
		{
			if( pSource != NULL )pSource->AddList( Data );
			Data.AddNext( Pos );
		}
		MovePointer Pos;
		RefPtr<Chain> pSource;
		const float& RefLength() const{return Length;}
	private:
		float Length;
	};

	class BlockChain : public Chain
	{
	public:
		BlockChain(){ pBlock = NULL; }
		void AddBlock(const MovePointer& AddPos, Chain* pSrc, const PolyStation* pSetBlock )
		{
			AddChain( AddPos, pSrc );
			pBlock = pSetBlock;
		}
		const PolyStation* pBlock;
	};

	class CheckPoint
	{
	private:
		typedef std::list<RefPtr<BlockChain> > ListData;
	public:
		CheckPoint():Range2(16*16){}
		CheckPoint( float NewRange ):Range2(NewRange*NewRange){}
		void SetCheckRange( float NewRange )
		{
			Range2 = NewRange*NewRange;
		}
		bool CheckIn( BlockChain* pChain )
		{
			return Check( pChain, true );
		}
		bool Check( BlockChain* pChain, bool bInput=false )
		{
			if( pChain == NULL )return false;
			ListData::iterator itr = PosList.begin();
			while( itr != PosList.end() )
			{
				RefPtr<BlockChain>& Ref = *itr++;
				if( pChain->pBlock != Ref->pBlock )continue;
				if( pChain->Pos.NiarXY2(Ref->Pos,Range2) )
				{
					if( !(*pChain < *Ref) )
						return false;
					if( bInput )Ref = pChain;
					return true;
				}
			}
			if( bInput )PosList.push_back( pChain );
			return true;
		}
		Chain* GetTagetChain( const MovePointer& Taget ) const
		{
			if( PosList.size() <= 0 )return NULL;
			ListData::const_iterator itr = PosList.begin();
			RefPtr<Chain> pRet = *itr++;
			float Length2 = Taget.LenXY2(pRet->Pos);
			while( itr != PosList.end() )
			{
				RefPtr<Chain> pCheck = *itr++;
				const float CheckLength2 = Taget.LenXY2(pCheck->Pos);
				if( CheckLength2 >= Length2 )continue;
				pRet = pCheck;
				Length2 = CheckLength2;
			}
			return pRet;
		}
	private:
		ListData PosList;
		float Range2;
	};
};
#endif // __MOVE_LIST__