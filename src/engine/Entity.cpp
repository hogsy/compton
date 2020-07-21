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

void vc::Entity::Tick() {
}

void vc::Entity::Draw() {
	if( !isVisible ) {
		return;
	}
}

void vc::Entity::Deserialize( Serializer *read ) {
	origin = read->ReadCoordinate();
}

void vc::Entity::Serialize( Serializer *write ) {
	write->WriteCoordinate( origin );
}
