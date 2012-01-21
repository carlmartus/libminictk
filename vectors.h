/**
 * @file
 * @brief Vector math operations
 * Mathematical operations on vectors and matrises
 */

#ifndef VECTORS_H
#define VECTORS_H
#include <stdint.h>

typedef struct { float x, y; } vec2f_t; /**< 2D float vector */
typedef struct { float x, y, z; } vec3f_t; /**< 3D float vector */
typedef struct { float x, y, z, w; } vec4f_t; /**< 4D float vector */

typedef struct { int32_t x, y; } vec2i_t; /**< 2D int32 vector */
typedef struct { int32_t x, y, z; } vec3i_t; /**< 3D int32 vector */
typedef struct { int32_t x, y, z, w; } vec4i_t; /**< 4D int32 vector */


// =========================
//  Float vector operations
// =========================


/**
 * 3D float dot product:
 *
 * \f$ d_{prod} = a_x \cdot b_x + a_y \cdot b_y + a_z \cdot b_z \f$
 */
inline float vec3f_dot(vec3f_t v0, vec3f_t v1); /**<  */
inline vec3f_t vec3f_cross(vec3f_t v0, vec3f_t v1); /**< 3D float dot product */

#endif


/**
 * @page page_vectors Quick: Vectors
 *
 * Example of how to use some of the procedures on 3D float vectors:
 *
 * @code
 * vec3f_t v0, v1, cr;
 * v0 = (vec3f_t) { 3.0f, 4.0f, 5.0f }; // One way of assigning
 * v1.x = -4.0f; // Another way of assigning
 * v1.y = -7.0f;
 * v1.z =  2.0f;
 *
 * printf("Dot product: %.3f\n", vec3f_dot(v0, v1));
 *
 * cr = vec3f_cross(v0, v1);
 * printf("Cross vector: %.3f %.3f %.3f\n", cr.x, cr.y, cr.z);
 * @endcode
 */

