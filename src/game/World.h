// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Terrain.h"

namespace vc
{
	class World : public ISerializable
	{
	public:
		World( const char *name, unsigned int seed = 0 );
		~World();

		void Tick();
		void Draw( const Camera &camera );

		void Deserialize( Serializer *read ) override;
		void Serialize( Serializer *write ) override;

		inline unsigned int GetTotalSeconds() const { return numSeconds_; }
		inline unsigned int GetTotalMinutes() const { return numSeconds_ / 60; }
		inline unsigned int GetTotalHours() const { return GetTotalMinutes() / 60; }
		inline unsigned int GetTotalDays() const { return GetTotalHours() / 24; }

		inline unsigned int GetCurrentSecond() const
		{
			return ( GetTotalSeconds() - ( GetTotalMinutes() / 60 ) ) % 60;
		}
		inline unsigned int GetCurrentMinute() const
		{
			return ( GetTotalMinutes() - ( GetTotalHours() / 60 ) ) % 60;
		}
		inline unsigned int GetCurrentHour() const
		{
			return ( GetTotalHours() - ( GetTotalDays() / 24 ) ) % 24;
		}

		enum class TimeOfDay
		{
			DAWN,
			MORNING,
			AFTERNOON,
			EVENING,
			NIGHT
		};
		inline TimeOfDay GetTimeOfDay() const
		{
			unsigned int curHour = GetCurrentHour();
			if ( curHour > 17 )
			{
				return TimeOfDay::NIGHT;
			}
			else if ( curHour > 15 )
			{
				return TimeOfDay::EVENING;
			}
			else if ( curHour > 12 )
			{
				return TimeOfDay::AFTERNOON;
			}
			else if ( curHour > 9 )
			{
				return TimeOfDay::MORNING;
			}
			else if ( curHour > 5 )
			{
				return TimeOfDay::DAWN;
			}

			return TimeOfDay::NIGHT;
		}

	private:
		struct Territory
		{
			Territory( const hei::Vector2 &origin );

			std::string  name{ "unnamed" };
			hei::Vector2 origin{ 0.0f, 0.0f };
			PLGPolygon  *border{ nullptr };
		};
		std::vector< Territory > territories_;

		unsigned int numSeconds_{ 0 };// Number of total simulated seconds
		unsigned int seed_{ 0 };      // Randomisation seed

		std::string name_;
	};
}// namespace vc
