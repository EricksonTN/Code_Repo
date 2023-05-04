/*
 * defs.h - Various definitions for the Performance Lab.
 * 
 * DO NOT MODIFY ANYTHING IN THIS FILE
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define RIDX(i,j,n) ((i)*(n)+(j))

typedef struct {
  char *name, *email;
} student_t;

extern student_t student;

typedef struct {
   unsigned short red;
   unsigned short green;
   unsigned short blue;
} pixel;

typedef void (*complex_test_func) (int, pixel*, pixel*);
typedef void (*blur_test_func) (int, pixel*, pixel*);

void complex(int, pixel *, pixel *);
void dither_blur(int, pixel *, pixel *);

void register_complex_functions(void);
void register_blur_functions(void);
void add_complex_function(complex_test_func, char*);
void add_blur_function(blur_test_func, char*);

#endif /* _DEFS_H_ */

