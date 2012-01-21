/**
 * @file
 * @brief Memory Control
 * All memory allocation should be run through this module
 * to achive control of allocations and simple memory leak
 * detection.
 */
#ifndef MEM_H
#define MEM_H

#include <stdint.h>

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

#endif


/**
 * @page page_mem Quick: Managed memory
 * The point of this subsystem is to give the user a simple memory leakage
 * detection. When the library is being shut down it will look for active
 * allocations. If there are active allocations it will send a signal callback
 * which the user can catch if minictk_signal has been called with a function
 * pointer.
 *
 * @code
 * #include <minictk/mem.h>
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

