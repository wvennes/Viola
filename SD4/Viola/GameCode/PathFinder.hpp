//---------------------------------------------------------------------------
// PathFinder.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PATHFINDER_
#define _INCLUDED_PATHFINDER_

#include <list>
#include "Engine/Math/Vec2.hpp"
#include "Map.hpp"

/////////////////////////////////////////////////////////////////////////////
struct PathFinderNode
{
public:
	Vec2i				m_position;
	float				m_gCost;
	float				m_hCost;
	PathFinderNode*		m_parent;

public:
	float				GetFCost() { return m_gCost + m_hCost; }

};

/////////////////////////////////////////////////////////////////////////////
struct ComparePathFinderNodePointer
{
	bool operator()( PathFinderNode* leftHandSide, PathFinderNode* rightHandSide )
	{
		return leftHandSide->GetFCost() > rightHandSide->GetFCost();
	}
};

typedef std::list< PathFinderNode* >	OpenList;
typedef std::vector< PathFinderNode* >	ClosedList;

/////////////////////////////////////////////////////////////////////////////
class PathFinder
{
public:
	PathFinder();
	~PathFinder();

	void			FindPath( const Vec2i& start, const Vec2i& end, Map& map, int movementProperties, std::vector< Vec2i >& outPathList );

private:
	OpenList		m_openList;
	ClosedList		m_closedList;

private:
	float			CalculateHeuristic( const Vec2i& start, const Vec2i& end );
	float			CalculateDirectionalGCost( const Vec2i& difference );
	float			CalculateMovementPropertyGCost( Cell* cell, int movementProperties );
	bool			ListSort( PathFinderNode*& first, PathFinderNode*& second );
	bool			IsPositionOnClosedList( const Vec2i& position );
	bool			IsPositionOnOpenList( const Vec2i& position );
	PathFinderNode*	GetNodeFromOpenList( const Vec2i& position );
	void			DestroyLists();

};

#endif