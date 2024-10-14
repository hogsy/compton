// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "agent.h"

class SpriteAgent : public Agent
{
public:
	IMPLEMENT_FACTORY( SpriteAgent )

	void SetupProperties( const AgentDefinitionLoader &adf_loader ) override;

	void Tick() override;
	void Draw() override;

protected:
	Sprite *sprite_{ nullptr };
};
