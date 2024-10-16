// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#include "app.h"

#include "console.h"
#include "render/render.h"
#include "render/render_bitmap_font.h"

Console::Console()  = default;
Console::~Console() = default;

void Console::Open()
{
	buffer.clear();
	isOpen = true;
}

void Console::Close()
{
	isOpen = false;
}

void Console::Draw()
{
	if ( !IsOpen() )
	{
		return;
	}

	unsigned int sh = GetApp()->GetDrawHeight();
	unsigned int sw = GetApp()->GetDrawWidth();

	static constexpr unsigned int CONSOLE_PANE_HEIGHT = 16;
	unsigned int                  CONSOLE_PANE_Y      = sh - CONSOLE_PANE_HEIGHT;

	render::DrawFilledRectangle( 0, CONSOLE_PANE_Y, sw, CONSOLE_PANE_HEIGHT, hei::Colour( 0, 0, 0 ) );
	render::DrawLine( 2, CONSOLE_PANE_Y + 2, 8, CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ), hei::Colour( 0, 255, 255 ) );
	render::DrawLine( 2, sh - 2, 8, CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ), hei::Colour( 0, 255, 255 ) );

	const BitmapFont *font = GetApp()->GetDefaultFont();
	assert( font != nullptr );

	int x = 10;
	int y = CONSOLE_PANE_Y + ( CONSOLE_PANE_HEIGHT / 2 ) - ( font->GetCharacterHeight() / 2 );
	font->DrawString( &x, &y, buffer.c_str(), hei::Colour( 255, 255, 0 ) );
}

void Console::PushCharacter( char c )
{
	if ( c == '\r' )
	{
		if ( buffer.empty() )
		{
			return;
		}

		PlParseConsoleString( buffer.c_str() );
		history.push_back( buffer );
		buffer.clear();
		return;
	}

	if ( c == '\b' || c == 127 )
	{
		if ( buffer.empty() )
		{
			return;
		}

		buffer.pop_back();
		return;
	}

	buffer.push_back( c );
}

void Console::ScrollHistory( bool forward )
{
	if ( forward )
	{
		historyPosition++;
	}
	else
	{
		if ( historyPosition <= 0 )
		{
			historyPosition = history.size();
		}
		else
		{
			historyPosition--;
		}
	}

	if ( historyPosition >= history.size() )
	{
		historyPosition = 0;
	}

	buffer = history[ historyPosition ];
}
