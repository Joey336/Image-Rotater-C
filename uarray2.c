#include "uarray2.h"
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <stdio.h>
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#define U Uarray2


struct Uarray2{
	int width;
	int height;
 	int size;
    Array_T uarray2;
};


extern void *array_at(Uarray2 array2, int width, int height){
    return Array_get(*(Array_T *)Array_get(array2->uarray2, width), height);
}

extern void array_put(Uarray2 array2, int width, int height, void* ele){
    //code bellow also works but is not recomanded 
    //Array_put(*(Array_T *)Array_get(array2->uarray2, width), height, ele);
    assert(ele);
    assert(array2);
    int* des = array_at(array2, width, height);
    //memcpy(des, ele, sizeof(void*));
    *des = *(int *)ele;
}

extern int Uarray2_width(U uarray2){
    return uarray2->width;
}
extern int Uarray2_height(U uarray2){
    return uarray2->height;
}
extern int Uarray2_size(U uarray2){
    return uarray2->size;
}


// void arrayLoader(int width,int height,  int size, Array_T widthArray){
    
//     //Array_T ptr[width];
//     for(int i = 0; i < width; i++){
//         Array_T ptr = Array_new(height, size);
//         //|notice here I have the address of Array_T* so it is a double pointer
//         void* des = Array_get(widthArray, i);

//         *(Array_T*)des = ptr;
//     }
// }


//check for null pointers, assery(array_T)
extern U Uarray2_new(int size, int width, int height){
    U array2;
    NEW(array2);
    
    Array_T widthArray = Array_new(width, sizeof(Array_T));
    

    array2->uarray2 = widthArray;
    array2->width = width;
    array2->height = height;

    //arrayLoader(width, height, size, array2->uarray2);
    
    for (int i = 0; i < width; i ++){
        Array_T *ptr = Array_get(array2->uarray2, i);
        *ptr = Array_new(height, size);
    }
    
    return array2;
}



//given a 2d array, deallocates the memory allocated to the data structure
extern void array2_free(U *array2){
    assert(array2 && *array2);
    for(int i = 0; i < (*array2)->width; i++){
        Array_T arr = *(Array_T *)Array_get((*array2)->uarray2, i);
        
        Array_free(&arr);
    }
    Array_free(&(*array2)->uarray2);
    FREE(*array2);
}




//doesn’t return anything, just apply column by column.
extern void Uarray2_map_row_major(Uarray2 Uarray, void apply(int width, int height, Uarray2 value, void *elem, void *cl), void *cl){
    int test; 
    for(int i = 0; i <  Uarray->height; i++){
        for(int j = 0; j < Uarray->width; j++){
            //use test to pass in j
            
            test = j;
            (void)test;
            apply(j, i, Uarray, array_at(Uarray, j, i),cl);
        }
    }
}

//extern void row_major(U Uarray, void *apply(void **, void *, void*), void* cl)
extern void Uarray2_map_col_major(Uarray2 Uarray, void apply(int width, int height, Uarray2 value,void *elem,void *cl), void *cl){
    int test;
    for(int i = 0; i < Uarray->width; i++){
        for(int j = 0; j < Uarray->height; j++){
            test = j;
            (void)test;
            apply(i, j, Uarray, array_at(Uarray, i, j),cl);
        }
    }
}