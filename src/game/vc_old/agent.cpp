/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"

#include "agents/PhysicsAgent.h"
#include "agent.h"

Agent::Agent() = default;
Agent::~Agent() = default;

void Agent::SetupProperties( const AgentDefinitionLoader &adf_loader ) {
}

void Agent::Restore() {
}

void Agent::Save() {
}

/////////////////////////////////////////////////

AgentFactory::AgentFactory() {
	// register all the base classes
	Register( "Sprite", SpriteAgent::Create );
	Register( "Physics", PhysicsAgent::Create );
}

AgentFactory::~AgentFactory() {
	registered_.clear();
}

void AgentFactory::Register( const std::string &name, AgentSpawnFunction func ) {
	registered_.insert( std::pair< std::string, AgentClassData >( name, AgentClassData( func, AgentDefinitionLoader() ) ) );
}

void AgentFactory::Register( const std::string &name, const std::string &baseclass, const AgentDefinitionLoader &data ) {
	auto it = registered_.find( baseclass );
	if ( it == registered_.end() ) {
		Warning( "Failed to find baseclass, \"%s\"!\n", baseclass.c_str() );
		return;
	}

	registered_.insert( std::pair< std::string, AgentClassData >( name, AgentClassData( it->second.spawn_function, data ) ) );
}

Agent *AgentFactory::Create( const std::string &name ) {
	auto it = registered_.find( name );
	if ( it == registered_.end() ) {
		return nullptr;
	}

	Agent *agent = it->second.spawn_function();
	agent->SetupProperties( it->second.spawn_data );
	agents_.push_back( agent );
	return agent;
}

void AgentFactory::RegisterScript( const char *path, void *userData ) {
	AgentDefinitionLoader adf_loader( path );

	std::string classname = adf_loader.GetProperty( "classname" );
	if ( classname.empty() ) {
		Warning( "Invalid classname, unable to register \"%s\"!\n", path );
		return;
	}

	std::string baseclass = adf_loader.GetProperty( "baseclass" );
	if ( baseclass.empty() ) {
		Warning( "Invalid baseclass, unable to register \"%s\"!\n", path );
		return;
	}

	Get()->Register( classname, baseclass, adf_loader );
}

void AgentFactory::Tick() {
	for ( const auto &agent : agents_ ) {
		agent->Tick();
	}
}

void AgentFactory::Draw() {
	for ( const auto &agent : agents_ ) {
		agent->Draw();
	}
}

void AgentFactory::RegisterScripts() {
	// now load in all of our agents (these all depend on the base classes)
	plScanDirectory( "scripts", "adf", AgentFactory::RegisterScript, false, nullptr );
}

void AgentFactory::Clear() {
	for ( auto agent : agents_ ) {
		delete agent;
		agent = nullptr;
	}

	agents_.clear();
}