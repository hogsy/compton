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
		~RectEditor() {}

		void Tick() override
		{
			int mx, my;
			input::inputManager->GetMousePosition( &mx, &my );

			if ( input::inputManager->IsMouseButtonDown( input::MOUSE_BUTTON_LEFT ) )
			{
				if ( !startedDrag_ )
				{
					selection_.x = mx;
					selection_.y = my;
					startedDrag_ = true;
				}

				selection_.w = mx;
				selection_.h = my;
			}
			else
				startedDrag_ = false;
		}

		void Draw() override
		{
			SuperClass::Draw();

			int sx, sy;
			GetPosition( &sx, &sy );

			selection_.Draw();
		}

	private:
		GUIImage *background_;

		GUIButton *newButton_;
		GUIButton *saveButton_;

		struct SelectionBox
		{
			int x{ 0 }, y{ 0 };
			int w{ 0 }, h{ 0 };

			void Draw()
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
		};
		SelectionBox selection_;
		bool startedDrag_{ false };

		static constexpr unsigned int BG_H = DISPLAY_HEIGHT - 128;
	};
}

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

void ct::SpriteSheetEditor::RestoreGame( const char *path )
{
	Print( "Loading...\n" );

	PLPath inPath;
	snprintf( inPath, sizeof( path ), "%s/sprites/%s.dat", fs::GetDataLocation(), path );
	if ( !PlFileExists( inPath ) )
	{
		Warning( "File doesn't exist: %s\n", inPath );
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
