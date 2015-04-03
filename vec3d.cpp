#include "vec3d.h"
#include "assert.h"
#include <cmath>

vec3d vec3d::cross(vec3d other)
{
	vec3d result;
	result.x = y*other.z - z*other.y;
	result.y = z*other.x - x*other.z;
	result.z = x*other.y - y*other.x;
	return result;
}

float vec3d::dot(vec3d other)
{
	float result;
	result = x * other.x;
	result += y * other.y;
	result += z * other.z;
	return result;
}

vec3d vec3d::operator-(struct vec3d other)
{
	vec3d result;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;
	return result;
}

vec3d vec3d::operator+(struct vec3d other)
{
	vec3d result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	return result;
}

vec3d vec3d::operator*(float other)
{
	vec3d result;
	result.x = x * other;
	result.y = y * other;
	result.z = z * other;
	return result;
}

float vec3d::tilt()
{
	// find the degree at which the vector is from Z
	return asin(sqrt(x*x + y*y) / sqrt(x*x + y*y + z*z));
}

float vec3d::magn()
{
	return sqrt(x*x + y*y + z*z);
}

vec3d find_normal(vec3d & p1, vec3d & p2, vec3d & p3)
{
	// subtract and take the cross-product
	vec3d one = p1-p2;
	vec3d two = p2-p3;
	one = one.cross(two);
	if (one.z < 0)
	{
		one = one*-1.0;
	}
	assert(one.z >= 0);
	return one;
}