/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "../shared.h"

namespace vc {
	struct Camera {
		PLVector2 oldPosition, position;
		PLVector2 oldVelocity, velocity;
	};

	class GameMode {
	public:
		GameMode();
		~GameMode();

	protected:
	private:
		Camera playerCamera;
	};
}
