// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Camera.h"

namespace ct
{
	class Serializer;
	class Entity;
	class EntityManager
	{
	protected:
		typedef Entity *( *EntityConstructorFunction )();
		static std::map< std::string, EntityConstructorFunction > entityClasses;

	public:
		EntityManager();
		~EntityManager();

		static ct::Entity *CreateEntity( const std::string &className );
		static void               DestroyEntity( Entity *entity );
		static void               DestroyEntities();

		static void        Tick();
		static void Draw( const Camera &camera );

		static void SerializeEntities( Serializer *write );
		void        DeserializeEntities( Serializer *read );

		static void SpawnEntities();
		static void        PrecacheEntities();

		struct EntitySlot
		{
			EntitySlot() : entity( nullptr ), num( 0 ) {}
			EntitySlot( ct::Entity *entity, unsigned int index ) : entity( entity ), num( index ) {}
			ct::Entity  *entity;
			unsigned int num;
		};
		static EntitySlot FindEntityByClassName( const char *className, const EntitySlot *curSlot = nullptr ) ;

		static std::vector< Entity * > GetEntitiesInRange( const hei::Vector2 &origin, float range );

		class EntityClassRegistration
		{
		public:
			const std::string myIdentifier;

			EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction );
			~EntityClassRegistration();
		};

	private:
		static std::vector< Entity * > entities;
		static std::vector< Entity * > destructionQueue;
	};
}// namespace ct

#define REGISTER_ENTITY( NAME, CLASS )                                                           \
	static ct::Entity *NAME##_make()                                                             \
	{                                                                                            \
		return new CLASS();                                                                      \
	}                                                                                            \
	static ct::EntityManager::EntityClassRegistration __attribute__( ( init_priority( 2100 ) ) ) \
	_reg_actor_##NAME##_name( ( #NAME ), NAME##_make );// NOLINT(cert-err58-cpp)
