// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

const double pi = std::acos(-1);

#define FLOATTOBYTE(a)	(byte)(a / 255)

// http://stackoverflow.com/questions/7978759/generate-float-random-values-also-negative
static inline double uniform0to1Random()
{
	double r = random();
	return r / ((double)RAND_MAX + 1);
}

static inline double GenerateRandom(double minmax)
{
	return (minmax * 2) * uniform0to1Random() - minmax;
}

static inline float GenerateRandom(float max)
{
	return static_cast <float> (rand()) / (RAND_MAX/max);
}

