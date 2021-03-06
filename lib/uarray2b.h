#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#define T UArray2b_T
typedef struct T *T;

extern T    UArray2b_new (int width, int height, int size, int blocksize);
  /* new blocked 2d array: blocksize = square root of # of cells in block */
extern T    UArray2b_new_16K_block(int width, int height, int size);
  /* new blocked 2d array: blocksize as large as possible provided
     block occupies at most 16KB (equivalently, 16 * 1024 B) (if possible) */

extern void  UArray2b_free  (T *array2b);

extern int   UArray2b_width (T array2b);
extern int   UArray2b_height(T array2b);
extern int   UArray2b_size  (T array2b);
extern int   UArray2b_blocksize(T array2b);

extern void *UArray2b_at(T array2b, int c, int r);
  /* return a pointer to the cell in column c, row r;
     index out of range is a checked run-time error
   */

extern void  UArray2b_map(T array2b, 
    void apply(int c, int r, T array2b, void *elem, void *cl), void *cl);
      /* visits every cell in one block before moving to another block */

/* it is a checked run-time error to pass a NULL T
   to any function in this interface */

#undef T
#endif
