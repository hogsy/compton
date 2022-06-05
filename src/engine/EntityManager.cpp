// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "EntityManager.h"
#include "Entity.h"
#include "BitmapFont.h"
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
		entityDrawOrder.emplace( entity->z_ ? entity->z_ : entity->origin_.y, entity );

	unsigned int i = 0;
	for ( const auto &entity : entityDrawOrder )
	{
		entity.second->Draw( camera );

		BitmapFont *font = GetApp()->GetDefaultFont();
		std::string s = "z: " + std::to_string( i );
		int x = (entity.second->origin_.x - camera.position.x) - ( ( font->GetCharacterWidth() * s.length() ) / 2 );
		int y = entity.second->origin_.y - camera.position.y;
		font->DrawString( &x, &y, s.c_str() );
		++i;
	}

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
		char className[ 64 ];
		read->ReadString( className, sizeof( className ) );

		Entity *entity = CreateEntity( className );
		if ( entity == nullptr )
		{
			Error( "Failed to deserialize entity %d, unknown class %s!\n", i, className );
		}

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

ct::EntityManager::EntityClassRegistration::EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction )
	: myIdentifier( identifier )
{
	EntityManager::entityClasses[ myIdentifier ] = constructorFunction;
}

ct::EntityManager::EntityClassRegistration::~EntityClassRegistration()
{
	EntityManager::entityClasses.erase( myIdentifier );
}
