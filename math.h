// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

typedef struct Vector2D
{
	Vector2D(float a, float b) : x(a), y(b) {}

	float x, y;

	void operator=(Vector2D b)
	{
		x = b.x; y = b.y;
	}
	void operator=(const Vector2D *b)
	{
		x = b->x; y = b->y;
	}
	void operator=(float a)
	{
		x = a; y = a;
	}

	void Set(float a, float b) { x = a; y = b; }
} Vector2D;

const double pi = std::acos(-1);

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