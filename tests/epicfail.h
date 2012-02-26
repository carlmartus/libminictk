#define H_CONTENT = 
#ifndef EPICFAIL_H
#define EPICFAIL_H

typedef int (*epicfail_func_t) ();
typedef struct {
	const char *title;
	epicfail_func_t call;
} epicfail_case_t;

#endif
