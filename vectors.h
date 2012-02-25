#ifndef VECTORS_H
#define VECTORS_H
#include <stdint.h>

typedef struct { float x, y; } vec2f_t;
typedef struct { float x, y, z; } vec3f_t;
typedef struct { float x, y, z, w; } vec4f_t;

typedef struct { int32_t x, y; } vec2i_t;
typedef struct { int32_t x, y, z; } vec3i_t;
typedef struct { int32_t x, y, z, w; } vec4i_t;

inline float vec3f_dot(vec3f_t v0, vec3f_t v1);
inline vec3f_t vec3f_cross(vec3f_t v0, vec3f_t v1);

#endif

