/**
 * @file
 * @brief Main library functionality
 * Initialization and shutdown procedures.
 * Signal callback connectivity.
 */

#ifndef MINICTK_H
#define MINICTK_H
#include <stdint.h>

enum {
	MCTK_SIG_NONE=0,		/**< Empty callback code */
	MCTK_SIG_MEMORY_LEAK,	/**< Memory leakage detected callback code */
	MCTK_SIG_MEMORY_FULL,	/**< Cannot perform allocation */
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

#endif

