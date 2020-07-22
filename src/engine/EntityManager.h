/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

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

		Entity *CreateEntity( const std::string &className, const std::stringstream &spawnData );
		void DestroyEntity( Entity *entity );
		void DestroyEntities();

		void Tick();
		void Draw();

		void SerializeEntities( Serializer *write );
		void DeserializeEntities( Serializer *read );

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
	static Entity *NAME##_make() { return new CLASS(); }                                     \
	static EntityManager::EntityClassRegistration __attribute__( ( init_priority( 2000 ) ) ) \
	        _reg_actor_##NAME##_name( ( #NAME ), NAME##_make );// NOLINT(cert-err58-cpp)
#define REGISTER_ENTITY_BASIC( CLASS ) \
	REGISTER_ACTOR( CLASS, CLASS )
