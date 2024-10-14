// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"
#include "../shared.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"

std::map< std::string, vc::EntityManager::EntityConstructorFunction > vc::EntityManager::entityClasses __attribute__( ( init_priority( 2000 ) ) );

vc::EntityManager::EntityVector vc::EntityManager::entities;
vc::EntityManager::EntityVector vc::EntityManager::destructionQueue;

vc::EntityManager::EntityManager()  = default;
vc::EntityManager::~EntityManager() = default;

vc::Entity *vc::EntityManager::CreateEntity( const std::string &className )
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

void vc::EntityManager::DestroyEntity( Entity *entity )
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

void vc::EntityManager::DestroyEntities()
{
	for ( auto &entity : entities )
	{
		delete entity;
	}

	destructionQueue.clear();
	entities.clear();
}

void vc::EntityManager::Tick()
{
	START_MEASURE();

	for ( const auto &entity : entities )
	{
		entity->Tick();
	}

	// Now clean everything up that was marked for destruction
	for ( auto &entity : destructionQueue )
	{
		entities.erase( std::remove( entities.begin(), entities.end(), entity ), entities.end() );
		delete entity;
	}

	destructionQueue.clear();

	END_MEASURE();
}

void vc::EntityManager::Draw( const Camera &camera )
{
	START_MEASURE();

	for ( const auto &entity : entities )
	{
		entity->Draw( camera );
	}

	END_MEASURE();
}

void vc::EntityManager::SerializeEntities( Serializer *write )
{
	write->WriteInteger( entities.size() );

	for ( auto &entity : entities )
	{
		write->WriteString( entity->GetClassIdentifier() );

		entity->Serialize( write );
	}
}

void vc::EntityManager::DeserializeEntities( Serializer *read )
{
	unsigned int numEntities = read->ReadInteger();
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

void vc::EntityManager::SpawnEntities()
{
	for ( auto &entity : entities )
	{
		entity->Spawn();
	}
}

vc::EntityManager::EntitySlot vc::EntityManager::FindEntityByClassName( const char *className, const vc::EntityManager::EntitySlot *curSlot ) const
{
	// Allow us to iterate from a previous position if desired
	unsigned int i = 0;
	if ( curSlot != nullptr )
	{
		i = curSlot->num;
	}

	for ( ; i < entities.size(); ++i )
	{
		if ( strcmp( entities[ i ]->GetClassIdentifier(), className ) != 0 )
		{
			continue;
		}

		return EntitySlot( entities[ i ], i );
	}

	return EntitySlot( nullptr, 0 );
}

vc::EntityManager::EntityClassRegistration::EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction )
    : myIdentifier( identifier )
{
	EntityManager::entityClasses[ myIdentifier ] = constructorFunction;
}

vc::EntityManager::EntityClassRegistration::~EntityClassRegistration()
{
	EntityManager::entityClasses.erase( myIdentifier );
}
