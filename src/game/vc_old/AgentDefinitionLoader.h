// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../shared.h"

#pragma once

class AgentDefinitionLoader
{
public:
	AgentDefinitionLoader();
	explicit AgentDefinitionLoader( const std::string &path );

	std::string GetProperty( const std::string &property, const std::string &def = "" ) const;

protected:
private:
	std::map< std::string, std::string > properties_;
};
