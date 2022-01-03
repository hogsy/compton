// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "GameMode.h"
#include "World.h"

vc::World::World( const char *name, unsigned int seed ) : name_( name ),
														  seed_( seed )
{
}

vc::World::~World()
{
}

void vc::World::Tick()
{
	numSeconds_ += GetApp()->GetGameMode()->GetGameSpeed();
}

void vc::World::Deserialize( vc::Serializer *read )
{
	seed_       = read->ReadInteger();
	numSeconds_ = read->ReadInteger();
}

void vc::World::Serialize( Serializer *write )
{
	write->WriteInteger( name_.size() );
	write->WriteString( name_.c_str() );

	write->WriteInteger( seed_ );
	write->WriteInteger( numSeconds_ );
}
