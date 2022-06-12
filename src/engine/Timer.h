// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2016-2022 Mark E Sowden <hogsy@oldtimes-software.com>

#pragma once

#include <time.h>

class Timer
{
public:
	Timer()
	{
		Start();
	}

	void Start()
	{
		timeStarted = PlGetCurrentSeconds() * 1000.0f;
	}

	void End()
	{
		timeTaken += ( PlGetCurrentSeconds() * 1000.0f ) - timeStarted;
	}

	inline double GetTimeTaken() const
	{
		return timeTaken;
	}

private:
	double timeStarted{ 0 };
	double timeTaken{ 0 };
};
