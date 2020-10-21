/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "../shared.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Serializer.h"

std::map< std::string, vc::EntityManager::EntityConstructorFunction > vc::EntityManager::entityClasses __attribute__( ( init_priority( 2000 ) ) );

std::set< vc::Entity* > vc::EntityManager::entities;
std::vector< vc::Entity* > vc::EntityManager::destructionQueue;

vc::EntityManager::EntityManager() {
}

vc::EntityManager::~EntityManager() {
}

vc::Entity *vc::EntityManager::CreateEntity( const std::string &className ) {
	auto i = entityClasses.find( className );
	if ( i == entityClasses.end() ) {
		printf( "Failed to find entity class \"%s\"!\n", className.c_str() );
		return nullptr;
	}

	Entity *entity = i->second();
	entities.insert( entity );

	return entity;
}

void vc::EntityManager::DestroyEntity( Entity *entity ) {
	// Ensure it's not already queued for destruction
	if ( std::find( destructionQueue.begin(), destructionQueue.end(), entity ) != destructionQueue.end() ) {
		printf( "Attempted to queue entity for deletion twice, ignoring...\n" );
		return;
	}

	// Move it into a queue for destruction
	destructionQueue.push_back( entity );
}

void vc::EntityManager::DestroyEntities() {
	for ( auto &entity : entities ) {
		delete entity;
	}

	destructionQueue.clear();
	entities.clear();
}

void vc::EntityManager::Tick() {
	START_MEASURE();

	for ( const auto &entity : entities ) {
		entity->Tick();
	}

	// Now clean everything up that was marked for destruction
	for ( auto &entity : destructionQueue ) {
		entities.erase( entity );
		delete entity;
	}

	destructionQueue.clear();

	END_MEASURE();
}

void vc::EntityManager::Draw( const Camera &camera ) {
	START_MEASURE();

	for ( const auto &entity : entities ) {
		entity->Draw( camera );
	}

	END_MEASURE();
}

void vc::EntityManager::SerializeEntities( Serializer *write ) {
	write->WriteInteger( entities.size() );

	for ( auto &entity : entities ) {
		write->WriteString( entity->GetClassIdentifier() );

		entity->Serialize( write );
	}
}

void vc::EntityManager::DeserializeEntities( Serializer *read ) {
	unsigned int numEntities = read->ReadInteger();
	for ( unsigned int i = 0; i < numEntities; ++i ) {
		char className[ 64 ];
		read->ReadString( className, sizeof( className ) );

		Entity *entity = CreateEntity( className );
		if ( entity == nullptr ) {
			Error( "Failed to deserialize entity %d, unknown class %s!\n", i, className );
		}

		entity->Deserialize( read );
	}
}

void vc::EntityManager::SpawnEntities() {
	for ( auto &entity : entities ) {
		entity->Spawn();
	}
}

vc::EntityManager::EntityClassRegistration::EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction )
: myIdentifier( identifier ) {
	EntityManager::entityClasses[ myIdentifier ] = constructorFunction;
}

vc::EntityManager::EntityClassRegistration::~EntityClassRegistration() {
	EntityManager::entityClasses.erase( myIdentifier );
}
