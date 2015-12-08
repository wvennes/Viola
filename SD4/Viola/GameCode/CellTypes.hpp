//---------------------------------------------------------------------------
// CellTypes.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CELLTYPES_
#define _INCLUDED_CELLTYPES_

#include "Cell.hpp"
#include "Engine/Math/Colors.hpp"

/////////////////////////////////////////////////////////////////////////////
namespace CellTypes
{
	static CellProperties	WALL	( TYPE_WALL,				Colors::WHITE,	'#',	'#' );
	static CellProperties	GRAVE	( TYPE_WALL,				Colors::PURPLE,	'#',	'G' );
	static CellProperties	FLOOR	( TYPE_AIR,					Colors::WHITE,	'.',	'.' );
	static CellProperties	ROOM	( TYPE_AIR,					Colors::RED,	'.',	'R' );
	static CellProperties	HALLWAY	( TYPE_AIR,					Colors::YELLOW,	'.',	'H' );
	static CellProperties	WATER	( TYPE_AIR | TYPE_WATER,	Colors::BLUE,	'-',	'-' );
	static CellProperties	LAVA	( TYPE_FIRE | TYPE_WATER,	Colors::RED,	'~',	'~' );
}

#endif