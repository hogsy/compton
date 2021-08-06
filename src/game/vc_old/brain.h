/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

namespace vc
{
	class Drive
	{
	public:
		Drive( const std::string &name );
		~Drive();

	protected:
	private:
		const std::string &description_;
		double			   weight_{ 0 };
	};

	class MemoryStorage
	{
	public:
		void Wipe()
		{
			identifiers_.clear();
		}

	protected:
	private:
		std::map< std::string, double > identifiers_;
		// the above is used for carrying identifications for
		// each agent in the world. each agent carries a unique
		// descriptor for it's "classification", e.g. honey, water, ice, gremlin
		// which we use to fetch our disposition
	};

	class Brain
	{
	public:
		Brain();
		~Brain();

		void Tick();

		void Serialise();
		void Deserialise();

	protected:
	private:
		std::vector< Drive > drives_{
				Drive( "thirst" ),
				Drive( "hunger" ),
				Drive( "boredom" ),
		};

		MemoryStorage storage_;
	};
}// namespace vc
