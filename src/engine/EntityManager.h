/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "Camera.h"
namespace vc {
	class Serializer;
	class Entity;
	class EntityManager {
	protected:
		typedef Entity *( *EntityConstructorFunction )();
		static std::map< std::string, EntityConstructorFunction > entityClasses;

	public:
		EntityManager();
		~EntityManager();

		vc::Entity *CreateEntity( const std::string &className );
		void DestroyEntity( Entity *entity );
		void DestroyEntities();

		void Tick();
		void Draw( const Camera &camera );

		void SerializeEntities( Serializer *write );
		void DeserializeEntities( Serializer *read );

		void SpawnEntities();

		class EntityClassRegistration {
		public:
			const std::string myIdentifier;

			EntityClassRegistration( const std::string &identifier, EntityConstructorFunction constructorFunction );
			~EntityClassRegistration();
		};

	private:
		static std::set< Entity * > entities;
		static std::vector< Entity * > destructionQueue;
	};
}// namespace vc

#define REGISTER_ENTITY( NAME, CLASS )                                                       \
	static vc::Entity *NAME##_make() { return new CLASS(); }                                     \
	static vc::EntityManager::EntityClassRegistration __attribute__( ( init_priority( 2100 ) ) ) \
	        _reg_actor_##NAME##_name( ( #NAME ), NAME##_make );// NOLINT(cert-err58-cpp)
