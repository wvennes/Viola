//---------------------------------------------------------------------------
// MovementProperties.hpp
//---------------------------------------------------------------------------

#ifndef _INCLUDED_MOVEMENTPROPERTIES_
#define _INCLUDED_MOVEMENTPROPERTIES_

#define Bit( x ) ( 1 << x )

/////////////////////////////////////////////////////////////////////////////
enum MovementProperties
{
	IS_BLOCKED_BY_WALLS		= Bit( 0 ),
	IS_BLOCKED_BY_AIR		= Bit( 1 ),
	IS_BLOCKED_BY_WATER		= Bit( 2 ),
	IS_BLOCKED_BY_DOORS		= Bit( 3 ),
	IS_BLOCKED_BY_FIRE		= Bit( 4 ),
	IS_SLOWED_BY_FIRE		= Bit( 5 ),
	IS_SLOWED_BY_WATER		= Bit( 6 ),
	IS_IMMUNE_TO_FIRE		= Bit( 7 ),
};

#endif