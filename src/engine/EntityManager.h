// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "Camera.h"

namespace vc
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

		vc::Entity *CreateEntity( const std::string &className );
		void        DestroyEntity( Entity *entity );
		void        DestroyEntities();

		void Tick();
		void Draw( const Camera &camera );

		void SerializeEntities( Serializer *write );
		void DeserializeEntities( Serializer *read );

		void SpawnEntities();

		struct EntitySlot
		{
			EntitySlot( vc::Entity *entity, unsigned int index ) : entity( entity ), num( index ) {}
			vc::Entity  *entity;
			unsigned int num;
		};
		EntitySlot FindEntityByClassName( const char *className, const EntitySlot *curSlot = nullptr ) const;

		class EntityClassRegistration
		{
		public:
			const std::string myIdentifier;

			EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction );
			~EntityClassRegistration();
		};

	private:
		typedef std::vector< Entity * > EntityVector;
		static EntityVector             entities;
		static EntityVector             destructionQueue;
	};
}// namespace vc

#define REGISTER_ENTITY( NAME, CLASS )                                                           \
	static vc::Entity *NAME##_make() { return new CLASS(); }                                     \
	static vc::EntityManager::EntityClassRegistration __attribute__( ( init_priority( 2100 ) ) ) \
	_reg_actor_##NAME##_name( ( #NAME ), NAME##_make );// NOLINT(cert-err58-cpp)
