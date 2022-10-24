// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#include "../Compton.h"
#include "../Input/InputManager.h"

#include "GUICursor.h"
#include "GUIStyleSheet.h"

ct::GUICursor::GUICursor( GUIPanel *parent, int x, int y, int w, int h )
    : GUIPanel( parent, x, y, w, h, Background::NONE, Border::NONE ) {}

ct::GUICursor::~GUICursor()
{
}

void ct::GUICursor::DrawBackground()
{
	if ( !isDrawing || styleSheet_ == nullptr )
		return;

	styleSheet_->mouseSprites[ currentCursorMode ]->Draw( x, y );
}

void ct::GUICursor::Tick()
{
	input::inputManager->GetMousePosition( &x, &y );

	int       pX, pY, pW, pH;
	GUIPanel *panel = GetParent();
	if ( panel == nullptr )
		return;

	panel->GetPosition( &pX, &pY );
	panel->GetSize( &pW, &pH );

	// Check the cursor is in the bounds of the parent
	if ( x + w < pX || x > pX + pW || y + h < pY || y > pY + pH )
		isDrawing = false;
	else
		isDrawing = true;
}
