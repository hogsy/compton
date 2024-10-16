// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright © 2016-2024 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"

class GUIButton : public GUIPanel
{
public:
	IMPLEMENT_SUPER( GUIPanel )

	GUIButton( GUIPanel *parent = nullptr, const char *label = nullptr, int x = 0, int y = 0, int w = 32, int h = 16 );
	~GUIButton();

	void Tick() override;

protected:
private:
	void ( *ButtonCallback )( void *userDataPtr );

	char myLabel[ 64 ];
};
