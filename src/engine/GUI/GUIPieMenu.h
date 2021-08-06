/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2021, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#pragma once

#include "GUIPanel.h"

namespace vc
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

			std::string	   label;
			OptionCallback callback;
		};
		std::vector< Option > myOptions;
	};
}// namespace vc
