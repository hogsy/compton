// Virtual Critters, Copyright (C) 2016 Mark Elsworth Sowden

#pragma once

typedef struct Vector2D
{
	Vector2D(float a, float b) : x(a), y(b) {}

	float x, y;

	void operator=(Vector2D a) { x = a.x; y = a.y; }
	void operator=(const Vector2D *a) { x = a->x; y = a->y; }
	void operator=(float a)	{ x = a; y = a;	}
	void operator*(Vector2D a) { x *= a.x; y *= a.y; }
	void operator*(const Vector2D *a) { x *= a->x; y *= a->y; }
	void operator*(float a) { x *= a; y *= a; }
	void operator/(Vector2D a) { x /= a.x; y /= a.y; }
	void operator/(const Vector2D *a) { x /= a->x; y /= a->y; }
	void operator/(float a) { x /= a; y /= a; }

	void Set(float a, float b) { x = a; y = b; }
} Vector2D;

const double pi = std::acos(-1);

#define FLOATTOBYTE(a)	((byte)a / 255)

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

// Interpolation
// http://paulbourke.net/miscellaneous/interpolation/

static inline float LinearInterpolate(float y1, float y2, float mu)
{
	return (y1 * (1 - mu) + y2 * mu);
}

static inline float CosineInterpolate(float y1, float y2, float mu)
{
	float mu2 = (1 - std::cos(mu * (float)M_PI)) / 2;
	return (y1 * (1 - mu2) + y2 * mu2);
}