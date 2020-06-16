/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../../shared.h"
#include "brain.h"

vc::Drive::Drive( const std::string &name ) : description_( name ) {
}
vc::Drive::~Drive() = default;

vc::Brain::Brain() = default;
vc::Brain::~Brain() = default;

void vc::Brain::Tick() {
}
