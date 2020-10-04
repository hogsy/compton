/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "SimGame.h"
#include "Entity.h"
#include "Serializer.h"

vc::Entity::Entity() {
}

vc::Entity::~Entity() {
}

/**
 * Called if the entity is spawning anew.
 */
void vc::Entity::Spawn() {
}

void vc::Entity::Tick() {
}

void vc::Entity::Draw() {
}

/**
 * Loading from pre-existing dataset.
 */
void vc::Entity::Deserialize( Serializer *read ) {
	origin = read->ReadCoordinate();
}

/**
 * Saving.
 */
void vc::Entity::Serialize( Serializer *write ) {
	write->WriteCoordinate( origin );
}
