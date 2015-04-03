#ifndef JPLCV_VEC3D_H
#define JPLCV_VEC3D_H

struct vec3d
{
	float x, y, z;
	
	struct vec3d operator-(struct vec3d other);
	struct vec3d operator+(struct vec3d other);
	struct vec3d operator*(float other);

	float dot(struct vec3d other);
	struct vec3d cross(struct vec3d other);
	float tilt(); // Z is UP
	float magn();
};
typedef struct vec3d vec3d;

vec3d find_normal(vec3d & p1, vec3d & p2, vec3d & p3);

#endif