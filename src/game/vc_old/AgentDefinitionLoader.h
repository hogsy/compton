/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"

#pragma once

class AgentDefinitionLoader {
public:
	AgentDefinitionLoader();
	explicit AgentDefinitionLoader( const std::string &path );

	std::string GetProperty( const std::string &property, const std::string &def = "" ) const;

protected:
private:
	std::map< std::string, std::string > properties_;
};
