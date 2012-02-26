/**
 * @file
 * @brief Library header file
 * Contains all public functions for usage by the library user.
 */

#ifndef MINICTK_H
#define MINICTK_H

#include <stdint.h>

/**
 * @mainpage Welcome!
 * @author Martin Sandgren
 *
 * Welcome to the fantastic world of the Minimalistic C ToolKit.
 *
 * This toolkit is intended for static compiling against the end product.
 *
 * Articles:
 * - @ref page_main
 * - @ref page_mem
 * - @ref page_vectors
 * - @ref page_observer
 */



// main.h - Start/Stop
// ===================

/**
 * @defgroup main Library initialization and shutdown
 * @{
 */

enum {
	MCTK_SIG_NONE=0,		/**< Empty callback code */
	MCTK_SIG_MEMORY_LEAK,	/**< Memory leakage detected callback code */
	MCTK_SIG_MEMORY_FULL,	/**< Cannot perform allocation */
	MCTK_SIG_OBSERVER_FULL,	/**< No room for further callbacks in observer */
	MCTK_SIG_COUNT,			/**< @private */
};

/**
 * Event data for signal callbacks. This structure is passed in as a parameter.
 * @sa minictk_signal
 */
typedef union {
	/** Memory leakage information */
	struct {
		uint32_t amount;	/**< Allocation count */
		uint32_t size;		/**< Sum of active allocation */
	} leak;
	void *observer_ptr;		/**< Observer pointer */
} mctk_sigdata_t;

/**
 * Signal callback definition
 */
typedef void(*minictk_sig_t)(int sig);

/**
 * Initialize library
 */
void minictk_start();

/**
 * Shut down library. Will check for memory leaks in managed memory.
 */
void minictk_stop();

/**
 * Set library callback routine.
 * Should be in format: void sig_callback(int code).
 * @param sig Callback procedure 
 */
void minictk_signal(minictk_sig_t sig);

void minictk_signal_call(int code, mctk_sigdata_t data); /**< @private */

/**
 * Get current version of libminictk as a string
 * @return C string pointer
 */
const char *minictk_version();

/**
 * @page page_main Startup and shutdown
 * Most of the functionality in the library requires
 * a proper startup to be able to work. A startup of
 * the library is simple and done be calling
 * minictk_start and minictk_stop during shutdown.
 *
 * @code
 * #include <minictk.h>
 * int main(int argc, char **argv)
 * {
 * 	minictk_start(); // Library initialization
 * 	// Use library functionality
 * 	minictk_stop(); // Library shutdown
 * }
 * @endcode
 * @sa minictk_start minictk_stop
 */

/*@}*/



// mem.h - Memory
// ==============

/**
 * @defgroup mem Memmory managment
 * @{
 */

void mem_start(); /**< @private */
void mem_stop(); /**< @private */

/**
 * Managed malloc(). Allocates block of memory.
 * @param size Size of allocation in bytes
 * @return Pointer to data allocation
 */
void *mem_malloc(uint32_t size);

/**
 * Managed realloc(). Older pointer will be invalidated
 * @param ptr Pointer to allocation
 * @param new_size New size to set to allocation
 * @return New pointer with adjusted size
 */
void *mem_realloc(void *ptr, uint32_t new_size);

/**
 * Get size of allocation
 * @param ptr Allocation pointer
 * @return Allocation size in bytes
 */
uint32_t mem_size(void *ptr);

/**
 * Free active allocation.
 * @param ptr Pointer to allocation
 * @return Size of freed allocation
 */
uint32_t mem_free(void *ptr);


/**
 * @page page_mem Managed memory
 * The point of this subsystem is to give the user a simple memory leakage
 * detection. When the library is being shut down it will look for active
 * allocations. If there are active allocations it will send a signal callback
 * which the user can catch if minictk_signal has been called with a function
 * pointer.
 *
 * @code
 * #include <minictk.h>
 *
 * // Assuming that proper library initiation has been done
 * void *buf = mem_malloc(500); // Allocate a block of 500 bytes
 *
 * // Use the buffer
 *
 * buf = mem_realloc(buf, 1500); // Lets have some more memory, 1500 bytes
 * printf("Buf is %d b big\n", mem_size(buf));
 *
 * mem_free(buf); // Done with buf, lets free the memory
 * @endcode
 *
 * Using the wrapper gives your code memory leakage detection with ("almost")
 * no performance footprint.
 * @sa mem_malloc, mem_realloc, mem_free, mem_size, minictk_signal
 */

/*@}*/


// structs.h - Datastructures
// ==========================

/**
 * @defgroup structs Data structures
 * @{
 */

typedef struct {
	uint32_t size, alloc;
	void *data;
} dbuf_t;

typedef struct {
	dbuf_t *db;
	uint32_t offset;
} dbuf_ptr_t;

/**
 * Initialize structure for usage.
 * @param db Instance
 * @param init_size Initial size to be allocated
 */
void dbuf_create(dbuf_t *db, uint32_t init_size);

/**
 * Clear dynamic buffer
 * @param db Instance
 */
void dbuf_destroy(dbuf_t *db);

/**
 * Pack data into dynamic buffer
 * @param ptr Data to pack
 * @param size Size of data to pack
 * @return Pointer of data placement in buffer
 */
void *dbuf_pack(dbuf_t *db, void *ptr, uint32_t size);

/**
 * Reserve block of memmory in buffer
 * @param size Size of block to reserve
 * @return Pointer to reserved block
 */
void *dbuf_reserve(dbuf_t *db, uint32_t size);

/**
 * Retrive buffer offset from pointer in dynamic buffer
 * @param ptr Pointer towards buffer to be converted
 * @return Offset, works a safer pointer in buffer
 */
inline uint32_t dbuf_offset(dbuf_t *db, void *ptr);

/**
 * Convert offset to a real pointer
 * @param offset Offset pointer to be converted
 * @return Real pointer to data in buffer
 */
inline void *dbuf_retrive(dbuf_t *db, uint32_t offset);

/**
 * Get definite pointer of data in buffer. Since the buffer
 * is dynamic and can grow by performing reallocations it
 * is never safe to have a direct pointer to data in buffer
 * between insertions.
 * @param ptr Pointer inside of buffer
 * @return Dynamic buffer pointer object. Can be converted to pointer
 * @so dbuf_ptr_retrive
 */
dbuf_ptr_t dbuf_get_definite(dbuf_t *db, void *ptr);

/**
 * Convert a dbuf_ptr_retrive into a real pointer.
 * @param dp Indirect pointer to be converted
 */
void *dbuf_ptr_retrive(dbuf_ptr_t dp);

/**
 * @page page_dbuf Dynamic buffer
 * Data structure working as a dynamicaly growing buffer.
 * The structures takes data into a block of memory. If a
 * buffer overflow is about to occure, the block of memory
 * will grow to fit the inserted data.
 * The growing is done by performing a reallocation.
 * Only one memory allocation is done in dbuf_create.
 *
 * @code
 * #include <minictk.h>
 * #include <string.h>
 * #include <assert.h>
 * #include <stdio.h>
 *
 * int main(int argc, char **argv)
 * {
 * 	dbuf_t db;
 * 	char msg[] = "Hello Dynamic buffer!";
 * 	char *in_buffer;
 *
 * 	dbuf_create(&db, 500); // New buffer with 500 b as initial allocation
 * 	in_buffer = dbuf_pack(&db, msg, strlen(msg)+1); // +1 for the strings trailing 0
 * 	dbuf_destroy(&db); // Free buffer and block of memory
 * 	assert(in_buffer != msg);
 * 	printf("%s\n%s\n", msg, in_buffer);
 * 	return 0;
 * }
 * @endcode
 * This will produce:
 * @verbatim
Hello Dynamic buffer!
Hello Dynamic buffer!
@endverbatim
 */

/*@}*/



// vectors.h - Mathematics
// =======================

/**
 * @defgroup vectors Vectors, matrices and mathematics
 * @{
 */

typedef struct { float x, y; } vec2f_t; /**< 2D float vector */
typedef struct { float x, y, z; } vec3f_t; /**< 3D float vector */
typedef struct { float x, y, z, w; } vec4f_t; /**< 4D float vector */

typedef struct { int32_t x, y; } vec2i_t; /**< 2D int32 vector */
typedef struct { int32_t x, y, z; } vec3i_t; /**< 3D int32 vector */
typedef struct { int32_t x, y, z, w; } vec4i_t; /**< 4D int32 vector */

/**
 * 3D float dot product:
 *
 * \f$ d_{prod} = a_x \cdot b_x + a_y \cdot b_y + a_z \cdot b_z \f$
 */
inline float vec3f_dot(vec3f_t v0, vec3f_t v1); /**<  */
inline vec3f_t vec3f_cross(vec3f_t v0, vec3f_t v1); /**< 3D float dot product */

/**
 * @page page_vectors Vectors
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
/*@}*/



// modules.h - Modularity functionality
// ====================================
/**
 * @defgroup modules Modularity support
 * @{
 */

/**
 * Observer callback function
 */
typedef void (*obs_func_t) (void*);

/* @private */
typedef struct {
	obs_func_t func;
} obs_bind_t;

/**
 * @brief Observer
 * Observer design pattern
 * @so minictk_signal MCTK_SIG_OBSERVER_FULL
 */
typedef struct {
	uint16_t count; /**< Amount of registered observer callbacks */
 	/**
	 * Maximum amount of callbacks. If exceded a MCTK_SIG_OBSERVER_FULL
	 * will be signaled.
	 */
	uint16_t max;
	/**
	 * List of registered callbacks. This variable is properly allocated
	 * when the observer is declared with OBSERVER_PAD.
	 */
	obs_bind_t list[0];
} observer_t;
/**
 * Observer declarator. Use this when declaring a observer. The parameter
 * max sets the maximum amount of callbacks. After declaration a call to
 * obs_reset must be called with the same max parameter is in the
 * declaration.
 */
#define OBSERVER_PAD(max) union{observer_t s;char p[sizeof(observer_t)+sizeof(obs_bind_t)*max];}
#define OBSERVER_PTR(obs) ((observer_t*) obs)

/**
 * Prepare a observer for usage.
 * @param max Maximum amount of callbacks. Must be same as in declaration.
 */
void obs_reset(observer_t *o, uint16_t max);

/**
 * Register new callback in observer. If maximum amount is exceded a
 * MCTK_SIG_OBSERVER_FULL signal will be sent.
 * @param func Callback
 * @so MCTK_SIG_OBSERVER_FULL minictk_signal
 */
void obs_register(observer_t *o, obs_func_t func);

/**
 * Unregister previously registered callback
 * @param func Callback to unregister
 */
void obs_unregister(observer_t *o, obs_func_t func);

/**
 * Trigger observer. All callbacks will be notified.
 * @param data Data to be passed along to all callbacks
 */
void obs_trigger(observer_t *o, void *data);

/**
 * @page page_observer Observer
 *
 * The observer type does not require a proper library startup
 * (minictk_start).
 * Code sample:
 *
 * @code
 * #include <minictk.h>
 * #include <stdio.h>
 *
 * static void callback1(void *ptr)
 * {
 * 	printf("Data for callback 1: \"%s\"\n", (const char*) ptr);
 * }
 *
 * static void callback2(void *ptr)
 * {
 * 	printf("Data for callback 2: \"%s\"\n", (const char*) ptr);
 * }
 *
 * int main(int argc, char **argv)
 * {
 * 	OBSERVER_PAD(2) observer; // Observer with maximum of 3 callbacks
 * 	observer_t *obs_ptr = OBSERVER_PTR(&observer);
 *
 * 	obs_reset(obs_ptr, 2);
 * 	obs_register(obs_ptr, callback1);
 * 	obs_register(obs_ptr, callback2); // Maximum amount of callbacks have now been reached
 * 	obs_trigger(obs_ptr, "Hello Observers!");
 * 	return 0;
 * }
 * @endcode
 *
 * This program will produce:
 * @verbatim
Data for callback 1: "Hello Observers!"
Data for callback 2: "Hello Observers!"
@endverbatim
 */

/*@}*/

#endif

