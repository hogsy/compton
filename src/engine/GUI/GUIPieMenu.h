// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include "GUIPanel.h"
#include "GUIButton.h"

namespace ct
{
	class GUIPieMenu : public GUIPanel
	{
	public:
		GUIPieMenu( GUIPanel *parent = nullptr );
		~GUIPieMenu();

		void Show() override{};
		void Hide() override{};

		typedef void ( *OptionCallback )( void *userData );
		void PushOption( const char *label, OptionCallback callback );

	protected:
	private:
		enum class AnimationStatus
		{
			NONE,
			IS_SHOWING,
			IS_HIDING,
		};
		AnimationStatus animationStatus_{ AnimationStatus::NONE };

		std::vector< GUIButton * > buttons_;
	};
}// namespace ct
