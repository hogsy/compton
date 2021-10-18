/*
SimGame Engine
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
