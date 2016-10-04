/*
DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
Version 2, December 2004

Copyright (C) 2011-2016 Mark E Sowden <markelswo@gmail.com>

Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.

DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#pragma once

#include "platform.h"

// Base Defines

#define PL_PI   3.14159265358979323846

#define	PL_X    0
#define	PL_Y    1
#define	PL_Z    2

#define	PL_WIDTH    0
#define	PL_HEIGHT   1

#define PL_PITCH    0
#define	PL_YAW      1
#define	PL_ROLL     2

#define	PL_RED      0
#define	PL_GREEN    1
#define	PL_BLUE     2
#define	PL_ALPHA    3

// Vectors

typedef struct PLVector2D
{
	PLVector2D(PLfloat a, PLfloat b) : x(a), y(b) {}
	PLVector2D() : x(0), y(0) {}

	PLfloat x, y;

	void operator=(PLVector2D a)            { x = a.x; y = a.y; }
	void operator=(const PLVector2D *a)     { x = a->x; y = a->y; }
	void operator=(PLfloat a)               { x = a; y = a;	}
	void operator*=(PLVector2D a)           { x *= a.x; y *= a.y; }
	void operator*=(const PLVector2D *a)    { x *= a->x; y *= a->y; }
	void operator*=(PLfloat a)              { x *= a; y *= a; }
	void operator/=(PLVector2D a)           { x /= a.x; y /= a.y; }
	void operator/=(const PLVector2D *a)    { x /= a->x; y /= a->y; }
	void operator/=(PLfloat a)              { x /= a; y /= a; }

	void PL_INLINE Set(PLfloat a, PLfloat b)    { x = a; y = b; }
} PLVector2D;

// Quaternion

typedef struct PLQuaternion
{
	PLQuaternion(PLfloat a, PLfloat b, PLfloat c, PLfloat d) : x(a), y(b), z(c), w(d) {}
	PLQuaternion(PLfloat a, PLfloat b, PLfloat c) : x(a), y(b), z(c), w(0) {}
	PLQuaternion() : x(0), y(0), z(0), w(0) {}

	PLfloat x, y, z, w;

	void operator=(PLQuaternion a)          { x = a.x; y = a.y; z = a.z; w = a.w; }
	void operator=(const PLQuaternion *a)   { x = a->x; y = a->y; z = a->z; w = a->w; }
	void operator*=(PLfloat a)              { x *= a; y *= a; z *= a; w *= a; }
	void operator*=(PLQuaternion a)         { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
	void operator*=(const PLQuaternion *a)  { x *= a->x; y *= a->y; z *= a->z; w *= a->w; }

	PLQuaternion operator*(PLQuaternion a)
	{
		return PLQuaternion(x * a.x, y * a.y, z * a.z, w * a.w);
	}
	PLQuaternion operator*(const PLQuaternion *a)
	{
		return PLQuaternion(x * a->x, y * a->y, z * a->z, w * a->w);
	}

	void PL_INLINE Set(PLfloat a, PLfloat b, PLfloat c, PLfloat d)  { x = a; y = b; z = c; w = d; }
	void PL_INLINE Set(PLfloat a, PLfloat b, PLfloat c)             { x = a; y = b; z = c; }

	PLQuaternion PL_INLINE Inverse()
	{
		return PLQuaternion(-x, -y, -z, w);
	}

	PLfloat PL_INLINE Length()
	{
		return sqrtf((x * x + y * y + z * z + w * w));
	}
} PLQuaternion;

// Interpolation
// http://paulbourke.net/miscellaneous/interpolation/

static PL_INLINE PLfloat plLinearInterpolate(PLfloat y1, PLfloat y2, PLfloat mu)
{
	return (y1 * (1 - mu) + y2 * mu);
}

static PL_INLINE PLfloat plCosineInterpolate(PLfloat y1, PLfloat y2, PLfloat mu)
{
	PLfloat mu2 = (1 - std::cos(mu * (PLfloat)PL_PI)) / 2;
	return (y1 * (1 - mu2) + y2 * mu2);
}
