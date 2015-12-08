//---------------------------------------------------------------------------
// GameCommon.hpp
//---------------------------------------------------------------------------


#pragma once
#ifndef _INCLUDED_GAMECOMMON_
#define _INCLUDED_GAMECOMMON_

// window dimensions
// const static float			GAME_WINDOW_X = 1600.f;
// const static float			GAME_WINDOW_Y = 900.f;
// 
// const static Vec2f			MIDDLE_OF_WINDOW = Vec2f( 50.f + GAME_WINDOW_X * 0.5f, 50.f + GAME_WINDOW_Y * 0.5f );
// 
// const static float			GAME_ASPECT_X = 16.f;
// const static float			GAME_ASPECT_Y = 9.f;

const static int GAME_CELL_MULTIPLIER = 3; // higher number = more cells -- 3 = 42 x 21

// aspect ratio * cell multiplier for square cells
const static int GAME_WIDTH_CELLS = 70;//16 * GAME_CELL_MULTIPLIER;
const static int GAME_HEIGHT_CELLS = 38;//9 * GAME_CELL_MULTIPLIER;

// offset to fit in smaller area on the screen
const static int MAP_WIDTH_CELLS = GAME_WIDTH_CELLS - GAME_CELL_MULTIPLIER * 2;
const static int MAP_HEIGHT_CELLS = GAME_HEIGHT_CELLS - GAME_CELL_MULTIPLIER * 2;
// 64 x 32

#endif