//---------------------------------------------------------------------------
// GameEntity.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_GAMEENTITY_
#define _INCLUDED_GAMEENTITY_

#include <string>
#include <set>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Colors.hpp"


class Map;
class GameEntity;
class TiXmlNode;
class TiXmlElement;
class Cell;
class MessageBar;

typedef std::set< Vec2i > CellSet;
typedef std::set< GameEntity* > EntitySet;

/////////////////////////////////////////////////////////////////////////////
struct FieldOfViewResult
{
	std::set< GameEntity* >	m_entitiesHit;
	//CellSet		m_cellsHit;
};


/////////////////////////////////////////////////////////////////////////////
enum MovementDirections
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST,
	NUM_DIRECTIONS
};


/////////////////////////////////////////////////////////////////////////////
class GameEntity
{
public:

public:
	GameEntity();
	GameEntity( const GameEntity& otherEntity );
	GameEntity( const std::string& name, char glyph );
	GameEntity( const std::string& name, const Vec2i& position, char glyph, const Rgba& color, float maxHealth, int debugLevel );
	virtual ~GameEntity();

	virtual Vec2i						GetPosition() const { return m_position; }
	virtual std::string					GetName() const { return m_name; }
	virtual char						GetGlyph() const { return m_glyph; }
	virtual Rgba						GetColor() const { return m_color; }
	virtual float						GetMaxHealth() const { return m_maxHealth; }
	virtual float						GetCurrentHealth() const { return m_currentHealth; }
	virtual float						GetBaseAttackDamage() const { return m_baseAttackDamage; }
	virtual float						GetFieldOfViewRadius() const { return m_fieldOfViewRadius; }
	virtual int							GetDebugLevel() const { return m_debugLevel; }
	virtual int							GetID() const { return m_id; }
	virtual bool						IsDead() const { return m_currentHealth <= 0.f; }
	virtual const FieldOfViewResult*	GetFieldOfViewResult() const { return &m_fieldOfViewResult; }

	virtual void						SetPosition( const Vec2i& position ) { m_position = position; }
	virtual void						SetName( const std::string& name ) { m_name = name; }
	virtual void						SetGlyph( char glyph ) { m_glyph = glyph; }
	virtual void						SetColor( const Rgba& color ) { m_color = color; }
	virtual void						SetDebugLevel( int debugLevel ) { m_debugLevel = debugLevel; }
	virtual void						SetMaxHealth( float health ) { m_maxHealth = health; }
	virtual void						SetCurrentHealth( float health ) { m_currentHealth = health; }
	virtual void						SetBaseAttackDamage( float baseAttackDamage ) { m_baseAttackDamage = baseAttackDamage; }
	virtual void						SetFieldOfViewRadius( float fieldOfViewRadius ) { m_fieldOfViewRadius = fieldOfViewRadius; }
	virtual float						ApplyDamage( float damageToInflict );
	virtual void						AddToMap( Map* map ) { m_map = map; }
	virtual void						RemoveFromMap() { m_map = nullptr; }
	virtual void						SaveToXML( TiXmlNode* node );
	virtual void						LoadFromXML( TiXmlElement* node );

	virtual bool						Update() { return false; }
	virtual void						Render() {}

	
protected:
	Vec2i								m_position;
	std::string							m_name;
	char								m_glyph;
	Rgba								m_color;
	float								m_maxHealth;
	float								m_currentHealth;
	float								m_baseAttackDamage;
	float								m_fieldOfViewRadius;
	int									m_debugLevel;
	Map*								m_map;
	int									m_id;
	FieldOfViewResult					m_fieldOfViewResult;
	static int							s_nextGameEntityID;

protected:
	virtual void						Initialize() {}
	virtual void						CastRay( Map* map, float radius, const Vec2f& unitVector, bool shouldLightCells, FieldOfViewResult& outResult );
	virtual FieldOfViewResult			GetFieldOfViewResult( Map* map, float radius, bool shouldLightCells );
	virtual void						GetFieldOfViewResult( Map* map, float radius, bool shouldLightCells, FieldOfViewResult& result_out );

};

extern MessageBar* g_gameMessageLog;


#endif