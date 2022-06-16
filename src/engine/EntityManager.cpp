// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Renderer/BitmapFont.h"
#include "Serializer.h"

std::map< std::string, ct::EntityManager::EntityConstructorFunction > ct::EntityManager::entityClasses __attribute__( ( init_priority( 2000 ) ) );

ct::EntityManager::EntityVector ct::EntityManager::entities;
ct::EntityManager::EntityVector ct::EntityManager::destructionQueue;

ct::EntityManager::EntityManager() = default;
ct::EntityManager::~EntityManager() = default;

ct::Entity *ct::EntityManager::CreateEntity( const std::string &className )
{
	auto i = entityClasses.find( className );
	if ( i == entityClasses.end() )
	{
		Warning( "Failed to find entity class \"%s\"!\n", className.c_str() );
		return nullptr;
	}

	Entity *entity = i->second();
	entities.push_back( entity );

	return entity;
}

void ct::EntityManager::DestroyEntity( Entity *entity )
{
	// Ensure it's not already queued for destruction
	if ( std::find( destructionQueue.begin(), destructionQueue.end(), entity ) != destructionQueue.end() )
	{
		Warning( "Attempted to queue entity for deletion twice, ignoring...\n" );
		return;
	}

	// Move it into a queue for destruction
	destructionQueue.push_back( entity );
}

void ct::EntityManager::DestroyEntities()
{
	for ( auto &entity : entities )
		delete entity;

	destructionQueue.clear();
	entities.clear();
}

void ct::EntityManager::Tick()
{
	START_MEASURE();

	for ( const auto &entity : entities )
		entity->Tick();

	// Now clean everything up that was marked for destruction
	for ( auto &entity : destructionQueue )
	{
		entities.erase( std::remove( entities.begin(), entities.end(), entity ), entities.end() );
		delete entity;
	}

	destructionQueue.clear();

	END_MEASURE();
}

void ct::EntityManager::Draw( const Camera &camera )
{
	START_MEASURE();

	std::map< float, Entity * > entityDrawOrder;
	for ( const auto &entity : entities )
	{
		if ( !entity->ShouldDraw( camera ) )
			continue;

		// Allow the entity to override the z order if z member is set
		float z;
		if ( entity->z_ != 0.0f )
			z = entity->z_;
		else
			z = entity->origin_.y;

		entityDrawOrder.emplace( z, entity );
	}

	// And now this has the benefit of drawing everything in order *and* only if it's visible!
	for ( const auto &entity : entityDrawOrder )
		entity.second->Draw( camera );

	END_MEASURE();
}

void ct::EntityManager::SerializeEntities( Serializer *write )
{
	write->WriteI32( entities.size() );

	for ( auto &entity : entities )
	{
		write->WriteString( entity->GetClassIdentifier() );

		entity->Serialize( write );
	}
}

void ct::EntityManager::DeserializeEntities( Serializer *read )
{
	unsigned int numEntities = read->ReadI32();
	for ( unsigned int i = 0; i < numEntities; ++i )
	{
		std::string className = read->ReadString();

		Entity *entity = CreateEntity( className );
		if ( entity == nullptr )
			Error( "Failed to deserialize entity %d, unknown class %s!\n", i, className.c_str() );

		entity->Deserialize( read );
	}
}

void ct::EntityManager::SpawnEntities()
{
	for ( auto &entity : entities )
		entity->Spawn();
}

/**
 * Interate over each entity, create it and
 * then precache it before then deleting it.
 */
void ct::EntityManager::PrecacheEntities()
{
	Print( "Precaching for %lu entities...\n", entityClasses.size() );
	for ( const auto& i : entityClasses )
	{
		Entity *entity = CreateEntity( i.first );
		entity->Precache();
		DestroyEntity( entity );
	}
}

ct::EntityManager::EntitySlot ct::EntityManager::FindEntityByClassName( const char *className, const ct::EntityManager::EntitySlot *curSlot ) const
{
	// Allow us to iterate from a previous position if desired
	unsigned int i = 0;
	if ( curSlot != nullptr && curSlot->entity != nullptr )
		i = curSlot->num;

	for ( ; i < entities.size(); ++i )
	{
		if ( strcmp( entities[ i ]->GetClassIdentifier(), className ) != 0 )
			continue;

		return { entities[ i ], i };
	}

	return { nullptr, 0 };
}

std::vector< ct::Entity * > ct::EntityManager::GetEntitiesInRange( const hei::Vector2 &origin, float range )
{
	std::vector< Entity * > out;
	for ( auto i : entities )
	{
		hei::Vector2 sv = i->origin_ - origin;
		float dv = sv.Length();
		if ( dv > range )
			continue;

		out.push_back( i );
	}

	return out;
}

ct::EntityManager::EntityClassRegistration::EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction )
	: myIdentifier( identifier )
{
	EntityManager::entityClasses[ myIdentifier ] = constructorFunction;
}

ct::EntityManager::EntityClassRegistration::~EntityClassRegistration()
{
	EntityManager::entityClasses.erase( myIdentifier );
}
