
#include "uarray2b.h"
#include "uarray2.h"

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <stdio.h>
#include <math.h>
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#include "pnm.h"

#define T UArray2b_T

struct UArray2b_T{
    int width;
    int height;
    int size;
    int block_size;
    Uarray2 uarray2;
};




T    UArray2b_new (int width, int height, int size, int blocksize){

    T array2b;
    NEW(array2b);
    //printf("width: %d    height: %d   blocksize: %d\n", width, height, blocksize);
    array2b->width = width;
    array2b->height = height;
    array2b->size = size;
    array2b->block_size = blocksize;


    double bwidth = ceil((double)width/(double)blocksize);
    double bheight = ceil((double)height/(double)blocksize);
    //printf("bwidth: %f   height: %f\n", bwidth, bheight);
    Uarray2 testArray = Uarray2_new(sizeof(Array_T), (int)bwidth + 1 , (int)bheight + 1);
    //array2Loader(bwidth, bheight, blocksize, testArray);
    for (int i = 0; i < bwidth; i++){
        for(int j = 0; j < bheight; j++){
            Array_T* des = array_at(testArray , i, j);
            *des = Array_new(blocksize * blocksize, size);
        }
    }

    array2b-> uarray2 = testArray;
    //printf("hi in uarray2b 53\n");
    return array2b;
}
  /* new blocked 2d array: blocksize = square root of # of cells in block */
T    UArray2b_new_16K_block(int width, int height, int size){
    (void)width;
    (void)height;
    (void)size;
    // sqr(16kb * 1024byte / size of each element)
    //printf("REACHED 16K\n");
    int blocksize = sqrt(16 * 1024 / size);
    T returnArray = UArray2b_new(width, height, size, blocksize);
    assert(returnArray);
    return returnArray;
}
  /* new blocked 2d array: blocksize as large as possible provided
     block occupies at most 16KB (equivalently, 16 * 1024 B) (if possible) */

void  UArray2b_free  (T *array2b){
    (void) array2b;
}

extern int   UArray2b_width (T array2b){
    return array2b->width;
}
extern int   UArray2b_height(T array2b){
    return array2b->height;
}
extern int   UArray2b_size  (T array2b){
    return array2b->size;
}
extern int   UArray2b_blocksize(T array2b){
    return array2b->block_size;
}

extern void *UArray2b_at(T array2b, int c, int r){
    //width and height of spine 2d array
    int bwidth = c / array2b->block_size;
    int bheight = r / array2b->block_size;


    //get ele block array;
    //printf("c: %d   r: %d\n",c, r);
    // if(r >= array2b->height || c >= array2b->width){
    //     printf("c: %d   r: %d\n",c, r);
    // }
    //assert(c < array2b->width);
    //assert(r < array2b->height);
    
    Array_T* block = array_at(array2b->uarray2, bwidth, bheight);
    //printf("bw: %d   bh:%d\n", bwidth, bheight);
    int cellIndex = array2b->block_size * (c % array2b->block_size) + (r % array2b->block_size);
    Pnm_rgb output = Array_get(*block, cellIndex);
    //printf("got pixel data %d %d %d\n",output->red,output->green,output->blue);

    return output;
    
}
  /* return a pointer to the cell in column c, row r;
     index out of range is a checked run-time error
   */
//width: 3264    height: 2448   blocksize: 36
extern void  UArray2b_map(T array2b, void apply(int c, int r, T array2b, void *elem, void *cl), void *cl){
    int bwidth = (int)ceil((double)array2b->width / (double)array2b->block_size);
    int bheight = (int)ceil((double)array2b->height / (double)array2b->block_size);
    
    (void) apply;
    
     //block traversal is going in col-major
    for(int i = 0; i < bwidth; i++){
        for(int j = 0; j < bheight; j++){
        
            void* block = array_at(array2b->uarray2, i, j);
            (void)block;
            int blockWidthIndex = i * array2b->block_size;
            int blockHeightIndex = j * array2b->block_size;

            //printf("COL: %d   ROW: %d \n", i, j);
            
            for(int cellWidthIndex = 0; cellWidthIndex < array2b->block_size; cellWidthIndex ++){
                for (int cellHeightIndex = 0; cellHeightIndex < array2b->block_size; cellHeightIndex ++){
    
                    int c = blockWidthIndex + cellWidthIndex;
                    int r = blockHeightIndex + cellHeightIndex;
                    //printf("c: %d    r: %d \n", c, r);
                    //void* del = UArray2b_at(array2b, c, r);
                    //(void)del;
                    if(c < array2b->width && r < array2b->height){
                        apply(c, r, array2b, UArray2b_at(array2b, c, r), cl);
                    }
                    
                }
            }        
        }
    }
    
    
    (void) array2b;
}
    /* visits every cell in one block before moving to another block */

/* it is a checked run-time error to pass a NULL T
   to any function in this interface */