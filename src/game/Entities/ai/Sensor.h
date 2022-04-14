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

#include "Brain.h"

namespace ct::ai
{
	class Sensor
	{
	public:
		enum class Type : uint8_t
		{
			SIGHT,
			SOUND,
			SMELL,
			TASTE,
			TOUCH,
			PAIN,
			TIME,

			MAX_TYPES
		};
		static constexpr uint8_t MAX_SENSOR_TYPES = ( uint8_t ) Type::MAX_TYPES;

		typedef void ( *SensorStateCallback )( Sensor *self );

		inline Sensor() {}
		inline Sensor( Type type, Brain *brain ) : myType( type ), myBrain( brain ) {}

		void Tick();

	private:
		Brain *myBrain{ nullptr };

		Type myType{ Type::SIGHT };
	};
}// namespace vc::ai
