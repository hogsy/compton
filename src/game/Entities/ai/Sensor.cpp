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
/*--------------------------------------------------------------------------------------
 * Sensor.cpp
 *  For simple detection of external elements. Passes any information back to brain.
 *------------------------------------------------------------------------------------*/

#include "Compton.h"
#include "Sensor.h"

void vc::ai::Sensor::Tick()
{
	switch ( myType )
	{
		case Type::SIGHT:
		{
			break;
		}
		case Type::SOUND: break;
		case Type::SMELL: break;
		case Type::TASTE: break;
		case Type::TOUCH: break;
		case Type::PAIN: break;
		case Type::TIME: break;
		default: break;
	}
}
