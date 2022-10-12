// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "../shared.h"

#include "Terrain.h"

namespace ct
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

		void Generate( unsigned int seed = 0 );

		[[nodiscard]] inline unsigned int GetTotalSeconds() const { return numSeconds_; }
		[[nodiscard]] inline unsigned int GetTotalMinutes() const { return numSeconds_ / 60; }
		[[nodiscard]] inline unsigned int GetTotalHours() const { return GetTotalMinutes() / 60; }
		[[nodiscard]] inline unsigned int GetTotalDays() const { return GetTotalHours() / 24; }

		[[nodiscard]] inline unsigned int GetCurrentSecond() const
		{
			return ( GetTotalSeconds() - ( GetTotalMinutes() / 60 ) ) % 60;
		}
		[[nodiscard]] inline unsigned int GetCurrentMinute() const
		{
			return ( GetTotalMinutes() - ( GetTotalHours() / 60 ) ) % 60;
		}
		[[nodiscard]] inline unsigned int GetCurrentHour() const
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
		[[nodiscard]] inline TimeOfDay GetTimeOfDay() const
		{
			unsigned int curHour = GetCurrentHour();
			if ( curHour > 17 )
				return TimeOfDay::NIGHT;
			else if ( curHour > 15 )
				return TimeOfDay::EVENING;
			else if ( curHour > 12 )
				return TimeOfDay::AFTERNOON;
			else if ( curHour > 9 )
				return TimeOfDay::MORNING;
			else if ( curHour > 5 )
				return TimeOfDay::DAWN;

			return TimeOfDay::NIGHT;
		}

	private:
		////////////////////////////////////////////
		// Territory

		struct Territory
		{
			std::string   name{ "unnamed" };
			math::Vector2 origin;
			PLGPolygon   *border{ nullptr };
		};
		std::vector< Territory > territories_;

		////////////////////////////////////////////

		unsigned int numSeconds_{ 0 };// Number of total simulated seconds
		unsigned int seed_{ 0 };      // Randomisation seed

		std::string name_;

		////////////////////////////////////////////
		// Terrain

		Terrain terrain_;

	public:
		[[nodiscard]] inline const Terrain *GetTerrain() const { return &terrain_; }

		////////////////////////////////////////////
		// Quadrant

	public:
		struct Quadrant
		{
			std::vector< Entity * > occupiers{};
		};

		static constexpr unsigned int QUADRANT_WIDTH = 256;
		static constexpr unsigned int QUADRANT_HEIGHT = 256;

		static constexpr unsigned int NUM_QUADRANTS_ROW = Terrain::PIXEL_WIDTH / QUADRANT_WIDTH;
		static constexpr unsigned int NUM_QUADRANTS_COL = Terrain::PIXEL_HEIGHT / QUADRANT_HEIGHT;
		static constexpr unsigned int NUM_QUADRANTS = NUM_QUADRANTS_ROW * NUM_QUADRANTS_COL;

	private:
		Quadrant quadrants_[ NUM_QUADRANTS ];

	public:
		/**
		 * Fetches a quadrant explicitly against the provided
		 * X and Y values. If the X and Y are out of scope for
		 * the grid then it returns NULL.
		 */
		[[nodiscard]] const Quadrant *GetQuadrant( int x, int y ) const
		{
			if ( x < 0 || y < 0 || x >= QUADRANT_WIDTH * NUM_QUADRANTS_COL || y >= QUADRANT_HEIGHT * NUM_QUADRANTS_ROW )
				return nullptr;

			unsigned int xw = x / QUADRANT_WIDTH;
			unsigned int yh = y / QUADRANT_HEIGHT;
			unsigned int i = xw + yh * NUM_QUADRANTS_ROW;
			assert( i >= 0 && i < NUM_QUADRANTS );

			return &quadrants_[ i ];
		}

		////////////////////////////////////////////
	};
}// namespace ct
