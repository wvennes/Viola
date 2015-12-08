//---------------------------------------------------------------------------
// PathFinder.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Cell.hpp"
#include "MovementProperties.hpp"
#include "PathFinder.hpp"


//---------------------------------------------------------------------------
PathFinder::PathFinder()
{

}


//---------------------------------------------------------------------------
PathFinder::~PathFinder()
{

}


//---------------------------------------------------------------------------
void PathFinder::FindPath( const Vec2i& start, const Vec2i& end, Map& map, int movementProperties, std::vector< Vec2i >& outPathCoordinates )
{
	std::vector< Vec2i > surroundingCoordinates;
	surroundingCoordinates.reserve( 8 );
	//std::vector< Vec2i > outPathList;
	Vec2i pathGoal = end;
	PathFinderNode* startNode = new PathFinderNode();
	PathFinderNode* currentNode = nullptr;
	startNode->m_position = start;
	startNode->m_gCost = 0.f;
	startNode->m_hCost = CalculateHeuristic( start, end );
	startNode->m_parent = nullptr;
	m_openList.push_back( startNode );

	while ( !m_openList.empty() )
	{
		currentNode = m_openList.back();
		m_openList.pop_back();

		if ( currentNode->m_position == pathGoal )
		{
			delete currentNode;
			break;
		}

		m_closedList.push_back( currentNode );
		map.GetSurroundingNonBlockedCoordinates( currentNode->m_position, movementProperties, surroundingCoordinates );

		for ( unsigned int index = 0; index < ( unsigned int ) surroundingCoordinates.size(); ++ index )
		{
			Vec2i& currentPosition = surroundingCoordinates.at( index );
			if ( IsPositionOnClosedList( currentPosition ) )
			{
				continue;
			}
			
			if ( IsPositionOnOpenList( currentPosition ) )
			{
				PathFinderNode* existingNode = GetNodeFromOpenList( currentPosition );
				float newGCost = currentNode->m_gCost + CalculateDirectionalGCost( currentPosition - currentNode->m_position );
				newGCost += CalculateMovementPropertyGCost( map.GetCell( currentPosition ), movementProperties );
				if ( newGCost > existingNode->m_gCost )
				{
					continue;
				}
				else
				{
					existingNode->m_gCost = newGCost;
					existingNode->m_parent = currentNode;
					continue;
				}
			}
			PathFinderNode* newNode = new PathFinderNode();
			newNode->m_position = currentPosition;
			newNode->m_gCost = CalculateDirectionalGCost( currentPosition - currentNode->m_position );
			newNode->m_gCost += CalculateMovementPropertyGCost( map.GetCell( currentPosition ), movementProperties );
			newNode->m_hCost = CalculateHeuristic( currentPosition, end );
			newNode->m_parent = currentNode;
			m_openList.push_back( newNode );

			//m_openList.push_back( new PathFinderNode())
		}
		m_openList.sort( ComparePathFinderNodePointer() );
		surroundingCoordinates.clear();
	}

	outPathCoordinates.push_back( end );
	if ( m_closedList.empty() ) return;
	PathFinderNode* finalPathNode = m_closedList.back();
	while ( finalPathNode != nullptr )
	{
		outPathCoordinates.push_back( finalPathNode->m_position );
		finalPathNode = finalPathNode->m_parent;
	}

// 	for ( unsigned int index = 0; index < ( unsigned int ) outPathCoordinates.size(); ++ index )
// 	{
// 		map.GetCell( outPathCoordinates[ index ] ).m_properties.m_color = Colors::BLUE;
// 	}
// 	map.GetCell( start ).m_properties.m_color = Colors::GREEN;
// 	map.GetCell( end ).m_properties.m_color = Colors::PURPLE;

	//delete startNode;

	DestroyLists();
}


//---------------------------------------------------------------------------
bool PathFinder::ListSort( PathFinderNode*& first, PathFinderNode*& second )
{
	return first->GetFCost() > second->GetFCost();
}


//---------------------------------------------------------------------------
float PathFinder::CalculateHeuristic( const Vec2i& start, const Vec2i& end )
{
	Vec2i difference = end - start;
	return ( float ) ( abs( difference.x ) + abs( difference.y ) );
}


//---------------------------------------------------------------------------
float PathFinder::CalculateDirectionalGCost( const Vec2i& difference )
{
	if ( difference.x == 0 || difference.y == 0 )
	{
		return 1.f;
	}
	else
	{
		return 1.45f;
	}
}


//---------------------------------------------------------------------------
float PathFinder::CalculateMovementPropertyGCost( Cell* cell, int movementProperties )
{
	if ( cell == nullptr ) return 100.f;

	if ( ( ( movementProperties & IS_BLOCKED_BY_FIRE	) == IS_BLOCKED_BY_FIRE	 )	&&	cell->IsType( TYPE_FIRE  ) )	return 100.f;
	if ( ( ( movementProperties & IS_BLOCKED_BY_AIR		) == IS_BLOCKED_BY_AIR	 )	&&	cell->IsType( TYPE_AIR	 ) )	return 100.f;
	if ( ( ( movementProperties & IS_BLOCKED_BY_WALLS	) == IS_BLOCKED_BY_WALLS )	&&	cell->IsType( TYPE_WALL  ) )	return 100.f;
	if ( ( ( movementProperties & IS_BLOCKED_BY_DOORS	) == IS_BLOCKED_BY_DOORS )	&&	cell->IsType( TYPE_DOOR  ) )	return 100.f;
	if ( ( ( movementProperties & IS_SLOWED_BY_FIRE		) == IS_SLOWED_BY_FIRE	 )	&&	cell->IsType( TYPE_FIRE  ) )	return 2.0;
	if ( ( ( movementProperties & IS_SLOWED_BY_WATER	) == IS_SLOWED_BY_WATER	 )	&&	cell->IsType( TYPE_WATER ) )	return 1.0;

	return 0.f;
}


//---------------------------------------------------------------------------
bool PathFinder::IsPositionOnClosedList( const Vec2i& position )
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_closedList.size(); ++ index )
	{
		if ( position == m_closedList.at( index )->m_position ) return true;
	}
	return false;
}


//---------------------------------------------------------------------------
bool PathFinder::IsPositionOnOpenList( const Vec2i& position )
{
	std::list< PathFinderNode* >::iterator iter;
	for ( iter = m_openList.begin(); iter != m_openList.end(); ++ iter )
	{
		if ( position == ( *iter )->m_position ) return true;
	}
	return false;
}


//---------------------------------------------------------------------------
PathFinderNode* PathFinder::GetNodeFromOpenList( const Vec2i& position )
{
	std::list< PathFinderNode* >::iterator iter;
	for ( iter = m_openList.begin(); iter != m_openList.end(); ++ iter )
	{
		if ( position == ( *iter )->m_position ) return *iter;
	}
	return nullptr;
}


//---------------------------------------------------------------------------
void PathFinder::DestroyLists()
{
// 	for ( unsigned int index = 0; index < ( unsigned int ) m_closedList.size(); ++ index )
// 	{
// 		delete m_closedList.at( index );
// 	}
	DestroyVector( m_closedList );

// 	OpenList::iterator iter;
// 	for ( iter = m_openList.begin(); iter != m_openList.end(); ++ iter )
// 	{
// 		delete *iter;
// 	}
	DestroyList( m_openList );
}