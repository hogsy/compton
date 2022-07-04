// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

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
		inline Sensor( Type type, Brain *brain ) : type_( type ), brain_( brain ) {}

		void Tick();

	private:
		Brain *brain_{ nullptr };

		Type type_{ Type::SIGHT };
	};
}// namespace vc::ai
