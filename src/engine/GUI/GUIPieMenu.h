// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

namespace ct
{
	class GUIPieMenu : public GUIPanel
	{
	public:
		GUIPieMenu( GUIPanel *parent = nullptr );
		~GUIPieMenu();

		typedef void ( *OptionCallback )( void *userData );

		void PushOption( const char *label, OptionCallback callback );

	protected:
	private:
		struct Option
		{
			Option( const char *label_, OptionCallback callback_ ) : label( label_ ), callback( callback_ ) {}

			std::string    label;
			OptionCallback callback;
		};
		std::vector< Option > myOptions;
	};
}// namespace vc
