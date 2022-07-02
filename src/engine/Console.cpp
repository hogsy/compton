// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "Compton.h"

#include "Renderer/BitmapFont.h"
#include "Console.h"

// A very primitive console interface!

ct::Console::Console() = default;
ct::Console::~Console() = default;

void ct::Console::Open()
{
	buffer_.clear();
	isOpen_ = true;
}

void ct::Console::Close()
{
	isOpen_ = false;
}

void ct::Console::Draw()
{
	if ( !IsOpen() )
		return;

	static constexpr unsigned int CONSOLE_PANE_HEIGHT = 16;
	static constexpr unsigned int CONSOLE_PANE_Y = DISPLAY_HEIGHT - CONSOLE_PANE_HEIGHT;

	render::DrawFilledRectangle( 0, CONSOLE_PANE_Y, DISPLAY_WIDTH, CONSOLE_PANE_HEIGHT, hei::Colour( 0, 0, 0 ) );
	render::DrawLine( 2, CONSOLE_PANE_Y + 2, 8, CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ), hei::Colour( 0, 255, 255 ) );
	render::DrawLine( 2, DISPLAY_HEIGHT - 2, 8, CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ), hei::Colour( 0, 255, 255 ) );

	static const BitmapFont *font = GetApp()->GetDefaultFont();
	if ( font == nullptr )
		return;

	int x = 10;
	int y = CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ) - ( font->GetCharacterHeight() / 2 );
	font->DrawString( &x, &y, buffer_.c_str(), hei::Colour( 255, 255, 0 ) );
}

void ct::Console::PushCharacter( int c )
{
	if ( c == '\r' )
	{
		if ( buffer_.empty() )
			return;

		PlParseConsoleString( buffer_.c_str() );
		history_.push_back( buffer_ );
		buffer_.clear();
		return;
	}

	if ( c == '\b' || c == 127 )
	{
		buffer_.pop_back();
		return;
	}

	buffer_.push_back( c );
}

void ct::Console::ScrollHistory( bool forward )
{
	if ( forward )
	{
		historyPosition_++;
		if ( historyPosition_ >= history_.size() )
			historyPosition_ = 0;
	}
	else
	{
		if ( historyPosition_ <= 0 )
			historyPosition_ = history_.size();
		else
			historyPosition_--;
	}

	buffer_ = history_[ historyPosition_ ];
}
