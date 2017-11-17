
#ifndef __ANGLE_CHECKER__
#define __ANGLE_CHECKER__

struct AngleChecker
{
	struct Checker
	{
		bool Check( float Angle )
		{
			if( Angle >= Start && Angle <= End )return true;
			Angle -= 2;
			if( Angle >= Start && Angle <= End )return true;
			return false;
		}
		void Set( const float& S, const float& E )
		{
			if( S > E )
			{
				Start = E; End = S;
			}
			else
			{
				Start = S; End = E;
			}
			if( End > 2.0f )
			{
				Start -= 2.0f;
				End -= 2.0f;
			}
		}
		float Start;
		float End;
	};
	struct Counter : protected Checker
	{
		bool Check( const float& Angle )
		{
			if( !Checker::Check( Angle ) )return false;
			Count++;
			return true;
		}
		void Set( float Angle, float Width )
		{
			if( Angle >= 2 )Angle -= 2;
			MyAngle = Angle;
			Checker::Set( Angle-Width, Angle+Width );
			Count = 0;
		}
		float MyAngle;
		long Count;
	};
};

#endif //__ANGLE_CHECKER__

