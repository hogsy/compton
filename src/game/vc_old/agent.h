/*
Compton, 2D Game Engine
Copyright (C) 2016-2021 Mark E Sowden <hogsy@oldtimes-software.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <utility>

#include "../engine/Sprite.h"
#include "AgentDefinitionLoader.h"

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void Activate() { is_activated_ = true; }
	virtual void Deactivate() { is_activated_ = false; }

	virtual void Tick() {}
	virtual void Draw() {}

	virtual void SetupProperties( const AgentDefinitionLoader &adf_loader );

	virtual void Restore();
	virtual void Save();

	virtual void SetPosition( PLVector2 pos )
	{
		old_position_ = position_;
		position_     = pos;
	}

	PLVector2 GetPosition() { return position_; }

	void SetDescriptor( const std::string &desc )
	{
		description_ = desc;
	}

protected:
	PLVector2 position_, old_position_;
	float     angle{ 0.0f };

private:
	std::string description_{ "none" };

	bool is_activated_{ false };
};

class AgentFactory
{
private:
	AgentFactory();
	AgentFactory( const AgentFactory & ) {}
	AgentFactory &operator=( const AgentFactory & ) { return *this; }

	static void RegisterScript( const char *path, void *userData );

	typedef Agent *( *AgentSpawnFunction )();
	struct AgentClassData
	{
		AgentClassData( AgentSpawnFunction func, AgentDefinitionLoader data ) : spawn_function( func ),
																				spawn_data( std::move( data ) ) {}
		AgentSpawnFunction    spawn_function{ nullptr };
		AgentDefinitionLoader spawn_data;
	};
	typedef std::map< std::string, AgentClassData > AgentMap;
	AgentMap                                        registered_;

	std::vector< Agent * > agents_;

public:
	~AgentFactory();

	static AgentFactory *Get()
	{
		static AgentFactory instance;
		return &instance;
	}

	void Tick();
	void Draw();

	void   RegisterScripts();
	void   Register( const std::string &name, AgentSpawnFunction func );
	void   Register( const std::string &name, const std::string &baseclass, const AgentDefinitionLoader &data );
	Agent *Create( const std::string &name );

	void Clear();

	unsigned int GetNumOfAgents() { return agents_.size(); }
};

#define IMPLEMENT_FACTORY( a ) \
	static Agent *Create() { return new ( a )(); }
