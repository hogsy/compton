// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "engine/Compton.h"

#include "SpriteSheetEditor.h"

#include "engine/GUI/GUIImage.h"
#include "engine/GUI/GUIButton.h"
#include "engine/GUI/GUIStyleSheet.h"

#include "engine/Input/InputManager.h"

namespace ct::gui
{
	class RectEditor : public GUIPanel
	{
	public:
		IMPLEMENT_SUPER( GUIPanel )

		RectEditor( int x, int y, int w, int h ) : GUIPanel( nullptr, x, y, w, h )
		{
			background_ = new GUIImage( this, nullptr, x, y, w, BG_H );
			newButton_ = new GUIButton( this, "NEW", 10, 10, 32, 16 );
			saveButton_ = new GUIButton( this, "SAVE", 52, 10, 32, 16 );
		}
		~RectEditor() = default;

		void Tick() override
		{
			//SuperClass::Tick();

			cursor.Tick();

			if ( input::inputManager->GetKeyState( ALLEGRO_KEY_SPACE ) == input::State::PRESSED )
			{
				if ( selection.dragging )
				{
					selection.x = cursor.position.x;
					selection.y = cursor.position.y;
					selection.w = 0;
					selection.h = 0;
					selection.dragging = false;
				}
				else
				{
					selection.dragging = true;
					selection.x = cursor.position.x;
					selection.y = cursor.position.y;
				}
			}

			if ( selection.dragging )
			{
				selection.w = cursor.position.x;
				selection.h = cursor.position.y;
			}
		}

		void Draw() override
		{
			SuperClass::Draw();

			int sx, sy;
			GetPosition( &sx, &sy );

			selection.Draw();
			cursor.Draw();
		}

	private:
		GUIImage *background_;
		GUIButton *newButton_;
		GUIButton *saveButton_;

	public:
		struct Cursor
		{
			math::Vector2 position;

			void Draw() const
			{
				render::DrawLine( position.x, position.y, position.x + 4, position.y + 4, hei::Colour( 255, 0, 0 ) );
			}

			void Tick()
			{
				if ( input::inputManager->GetKeyState( ALLEGRO_KEY_DOWN ) == input::State::DOWN )
					position.y++;
				else if ( input::inputManager->GetKeyState( ALLEGRO_KEY_UP ) == input::State::DOWN )
					position.y--;

				if ( input::inputManager->GetKeyState( ALLEGRO_KEY_LEFT ) == input::State::DOWN )
					position.x--;
				else if ( input::inputManager->GetKeyState( ALLEGRO_KEY_RIGHT ) == input::State::DOWN )
					position.x++;

				if ( position.x < 0 ) position.x = 0;
				else if ( position.x > DISPLAY_WIDTH )
					position.x = DISPLAY_WIDTH;
				if ( position.y < 0 ) position.y = 0;
				else if ( position.y > DISPLAY_HEIGHT )
					position.y = DISPLAY_HEIGHT;
			}
		};
		Cursor cursor;

		struct SelectionBox
		{
			int x{ 0 }, y{ 0 };
			int w{ 0 }, h{ 0 };

			void Draw() const
			{
				render::DrawLine( x, y, x + w, y, hei::Colour( 255, 0, 0 ) );
				render::DrawLine( x, y, x, y + h, hei::Colour( 255, 0, 0 ) );
				render::DrawLine( x, y, x + w, y, hei::Colour( 255, 0, 0 ) );
				render::DrawLine( x, y, x + w, y, hei::Colour( 255, 0, 0 ) );

				char tmp[ 64 ];
				snprintf( tmp, sizeof( tmp ), "x:%d y:%d w:%d h:%d", x, y, w, h );

				BitmapFont *font = GetApp()->GetDefaultFont();

				int sx = x + w;
				int sy = y + h + font->GetCharacterHeight();

				font->DrawString( &sx, &sy, tmp );
			}

			bool dragging{ false };
		};
		SelectionBox selection;

		static constexpr unsigned int BG_H = DISPLAY_HEIGHT - 128;
	};
}// namespace ct::gui

///////////////////////////////////////////////////////////////////

ct::SpriteSheetEditor::SpriteSheetEditor()
{
	basePanel_ = new gui::RectEditor( 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT );

	styleSheet_ = new GUIStyleSheet();
	if ( !styleSheet_->LoadFile( "gui/skins/default.txt" ) )
		Error( "Failed to load default style sheet!\n" );

	basePanel_->SetStyleSheet( styleSheet_ );

	new GUICursor( basePanel_ );
}

ct::SpriteSheetEditor::~SpriteSheetEditor()
{
	delete basePanel_;
	delete styleSheet_;
}

void ct::SpriteSheetEditor::NewGame( const char *path )
{
}

void ct::SpriteSheetEditor::SaveGame( const char *path )
{
	Print( "Saving...\n" );

	PLPath outPath;
	snprintf( outPath, sizeof( outPath ), "%s/sprites/%s.dat", fs::GetDataLocation(), path );


	Print( "Done!\n" );
}

void ct::SpriteSheetEditor::RestoreGame( const std::string &path )
{
	Print( "Loading...\n" );

	std::string fullPath = fs::GetDataLocation();
	fullPath.append( "/sprites/" + path );
	if ( !PlFileExists( fullPath.c_str() ) )
	{
		Warning( "File doesn't exist: %s\n", fullPath.c_str() );
		return;
	}
}

void ct::SpriteSheetEditor::Tick()
{
	basePanel_->Tick();
}

void ct::SpriteSheetEditor::Draw()
{
	basePanel_->Draw();
}

void ct::SpriteSheetEditor::Precache()
{
}

bool ct::SpriteSheetEditor::HandleMouseEvent( int x, int y, int wheel, int button, bool buttonUp )
{
	if ( basePanel_->HandleMouseEvent( x, y, wheel, button, buttonUp ) )
		return true;

	return false;
}

bool ct::SpriteSheetEditor::HandleKeyboardEvent( int key, bool keyUp )
{
	if ( basePanel_->HandleKeyboardEvent( key, keyUp ) )
		return true;

	switch ( key )
	{
		default:
			break;

		case ALLEGRO_KEY_Q:
			exit( 0 );
		case ALLEGRO_KEY_S:
		{
			if ( keyUp )
				return false;

			SaveGame( std::string( "sprites/" + document_ ).c_str() );
			return true;
		}

		case ALLEGRO_KEY_O:
		{
		}
	}

	return false;
}
