/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "EntityManager.h"
#include "Entity.h"

std::map< std::string, vc::EntityManager::EntityConstructorFunction > vc::EntityManager::entityClasses __attribute__( ( init_priority( 2000 ) ) );

std::set< vc::Entity* > vc::EntityManager::entities;
std::vector< vc::Entity* > vc::EntityManager::destructionQueue;

vc::EntityManager::EntityManager() {
}

vc::EntityManager::~EntityManager() {
}

vc::Entity *vc::EntityManager::CreateEntity( const std::string &className, const std::stringstream &spawnData ) {
	auto i = entityClasses.find( className );
	if ( i == entityClasses.end() ) {
		printf( "Failed to find entity class \"%s\"!\n", className.c_str() );
		return nullptr;
	}

	Entity *entity = i->second();
	entities.insert( entity );

	entity->Deserialize( nullptr );

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
	for ( const auto &entity : entities ) {
		entity->Tick();
	}

	// Now clean everything up that was marked for destruction
	for ( auto &entity : destructionQueue ) {
		entities.erase( entity );
		delete entity;
	}

	destructionQueue.clear();
}

void vc::EntityManager::Draw() {
	for ( const auto &entity : entities ) {
		entity->Draw();
	}
}

void vc::EntityManager::SerializeEntities( Serializer *write ) {
	for ( auto &entity : entities ) {
		entity->Serialize( write );
	}
}

void vc::EntityManager::DeserializeEntities( Serializer *read ) {
	for ( auto &entity : entities ) {
		entity->Deserialize( read );
	}
}

vc::EntityManager::EntityClassRegistration::EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction )
: myIdentifier( identifier ) {
	EntityManager::entityClasses[ myIdentifier ] = constructorFunction;
}

vc::EntityManager::EntityClassRegistration::~EntityClassRegistration() {
	EntityManager::entityClasses.erase( myIdentifier );
}
