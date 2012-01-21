#include "vectors.h"

inline float vec3f_dot(vec3f_t v0, vec3f_t v1)
{
	return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

inline vec3f_t vec3f_cross(vec3f_t v0, vec3f_t v1)
{
	return (vec3f_t) {
		v0.y*v1.z - v0.z*v1.y,
		v0.z*v1.y - v0.x*v1.z,
		v0.x*v1.z - v0.y*v1.x,
	};
}

