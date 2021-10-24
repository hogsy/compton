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
		void		DestroyEntity( Entity *entity );
		void		DestroyEntities();

		void Tick();
		void Draw( const Camera &camera );

		void SerializeEntities( Serializer *write );
		void DeserializeEntities( Serializer *read );

		void SpawnEntities();

		struct EntitySlot
		{
			EntitySlot( vc::Entity *entity, unsigned int index ) : entity( entity ), num( index ) {}
			vc::Entity * entity;
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
		static EntityVector				entities;
		static EntityVector				destructionQueue;
	};
}// namespace vc

#define REGISTER_ENTITY( NAME, CLASS )                                                           \
	static vc::Entity *NAME##_make() { return new CLASS(); }                                     \
	static vc::EntityManager::EntityClassRegistration __attribute__( ( init_priority( 2100 ) ) ) \
	_reg_actor_##NAME##_name( ( #NAME ), NAME##_make );// NOLINT(cert-err58-cpp)
