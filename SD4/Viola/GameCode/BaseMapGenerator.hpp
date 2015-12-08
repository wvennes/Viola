//---------------------------------------------------------------------------
// BaseMapGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BASEMAPGENERATOR_
#define _INCLUDED_BASEMAPGENERATOR_

#include <string>
#include <map>
#include "Feature.hpp"

class Map;
class Player;
class BaseMapGenerator;
class MapGeneratorRegistration;
class TiXmlNode;

typedef BaseMapGenerator* ( MapRegistrationFunc ) ( const std::string& name );
typedef std::map< std::string, MapGeneratorRegistration* > MapGeneratorRegistrationMap;
typedef std::map< std::string, BaseMapGenerator* > MapGeneratorMap;


/////////////////////////////////////////////////////////////////////////////
enum MapGeneratorDirection
{
	MAP_NORTH = 0,
	MAP_SOUTH = 1,
	MAP_EAST = 2,
	MAP_WEST = 3,
	NUM_GENERATOR_DIRECTIONS = 4,
};


/////////////////////////////////////////////////////////////////////////////
class MapGeneratorRegistration
{
public:
	MapGeneratorRegistration( const std::string& name, MapRegistrationFunc* registrationFunc )
		: m_name( name ),
		m_registrationFunc( registrationFunc )
	{
		if ( !s_generatorRegistrationMap ) s_generatorRegistrationMap = new MapGeneratorRegistrationMap();

		s_generatorRegistrationMap->insert( std::pair< std::string, MapGeneratorRegistration* >( name, this ) );
	}

	static MapGeneratorRegistrationMap*		GetGeneratorRegistrationMap() { return s_generatorRegistrationMap; }
	std::string								GetName() { return m_name; }
	BaseMapGenerator*						CreateMapGenerator() { return m_registrationFunc( m_name ); }

protected:
	std::string								m_name;
	MapRegistrationFunc*					m_registrationFunc;
	static MapGeneratorRegistrationMap*		s_generatorRegistrationMap;

};


/////////////////////////////////////////////////////////////////////////////
class BaseMapGenerator
{
public:
	std::string					m_name;

public:
	BaseMapGenerator( const std::string& name ) : m_name ( name ) {}
	~BaseMapGenerator() {};

	static void					CreateGenerators();
	static BaseMapGenerator*	GetGenerator( const std::string& name );
	static void					DestroyGenerators();

	virtual Map*				GenerateMap() = 0;
	virtual void				FinishGeneration( Map& map ) = 0;
	virtual void				Startup() = 0;
	virtual void				Shutdown() = 0;
	virtual void				PlacePlayer( Map& map, Player& player ) = 0;
	virtual void				LoadFromFile( TiXmlNode* node, Map& map, Player& player );
	virtual void				PlaceFeature( Map& map, const Vec2i& location, const std::string& featureName );
	virtual void				PlaceRandomItem( Map& map );
	virtual void				PlaceRandomItemAtLocation( const Vec2i& location, Map& map );
	virtual void				PlaceNPCAtRandomLocation( const std::string& npcName, Map& map );
	virtual void				PlaceNPCAtLocation( const Vec2i& location, const std::string& npcName, Map& map );
	const std::string			GetName() const { return m_name; }
	virtual void				ProcessOneStep( Map& map ) = 0;
	void						InitializeRandomData( Map& map );

protected:
	static MapGeneratorMap		s_mapGeneratorMap;

};

#endif