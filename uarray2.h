#ifndef UARRAY_INCLUDED
#define UARRAY_INCLUDED
#include "array.h"
#define U Uarray2
typedef struct U *Uarray2;



extern void *array_at(U array, int row, int col);

//check for null pointers, assery(array_T)
extern U Uarray2_new(int size, int row, int col);

extern void array_put(Uarray2 array2, int width, int height, void* ele);

//given a 2d array, deallocates the memory allocated to the data structure
extern void array2_free(U *U_array2D);

//getter
extern int Uarray2_width(U uarray2);
extern int Uarray2_height(U uarray2);
extern int Uarray2_size(U uarray2);


//doesn’t return anything, just apply column by column.
extern void  Uarray2_map_row_major(Uarray2 Uarray, void apply(int row, int col, Uarray2 value, void *elem, void *cl), void *cl);


extern void  Uarray2_map_col_major(Uarray2 Uarray, void apply(int row, int col, Uarray2 value, void *elem, void *cl), void *cl);

#undef U
#endif