/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "agent.h"

class SpriteAgent : public Agent {
public:
	IMPLEMENT_FACTORY( SpriteAgent )

	void SetupProperties( const AgentDefinitionLoader &adf_loader ) override;

	void Tick() override;
	void Draw() override;

protected:
	Sprite *sprite_{ nullptr };
};
