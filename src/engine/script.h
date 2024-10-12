// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once


namespace engine::script
{
	bool CompileFromFile( const std::string &path );
	bool CompileFromString( const std::string &eval );
}// namespace engine::script
