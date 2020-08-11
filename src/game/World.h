/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc {
	class Serializer;

	struct WorldSection {

	};

	class World {
	public:
		World();
		~World();

		PL_INLINE uint64_t GetTotalSeconds() { return numSeconds; }
		PL_INLINE uint64_t GetTotalMinutes() { return numSeconds / 60; }
		PL_INLINE uint64_t GetTotalHours() { return GetTotalMinutes() / 60; }
		PL_INLINE uint64_t GetTotalDays() { return GetTotalHours() / 25; }

		void Deserialize( Serializer *read );
		void Serialize( Serializer *write );

		void Tick();
		void Draw();

		PL_INLINE const char *GetName() const { return name; }

	private:
#define MAX_WORLD_NAME 64
		char name[ MAX_WORLD_NAME ];

		uint64_t numSeconds{ 0 };
	};
}// namespace vc
