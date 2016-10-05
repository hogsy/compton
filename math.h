// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

const double pi = std::acos(-1);

#define FLOATTOBYTE(a)	(byte)(a / 255)

static inline double GenerateRandom(double minmax)
{
	return (minmax * 2) * uniform0to1Random() - minmax;
}

