#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <assert.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "except.h"
#include "pnm.h"

static A2Methods_T methods;
typedef A2Methods_Array2 A2;

//_________bellow are all the apply function we used to transform the image__________


//transpose image
void applyTranspose(int c, int r, A2 array2b, void *elem, void *cl){
  (void) array2b;
  int r_new = c;
  int c_new = r;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;
 
}

//-vertical flip
void applyVflip(int c, int r, A2 array2b, void *elem, void *cl){
  int r_new = methods->height(array2b) - 1 - r;
  int c_new = c;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;
}


void applyHflip(int c, int r, A2 array2b, void *elem, void *cl){
  int r_new = r;
  int c_new = methods->width(array2b) - 1 - c;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;
} 


//this function will print to stdout
void hard_print_tofile(Pnm_ppm ppm){
  //(void) ppm;
  //FILE *fp;
  //fp = fopen ("out_put.ppm", "w+");
  //assert(fp);
  //fscanf(fp, "opss");
  Pnm_ppmwrite(stdout, ppm);
  
}


void apply90(int c, int r, A2 array2b, void *elem, void *cl){
  int r_new = c;
  int c_new = methods->height(array2b) - 1 - r;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;

}

void apply270(int c, int r, A2 array2b, void *elem, void *cl){
  int r_new = methods->width(array2b) - 1 - c;
  int c_new = r;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;
}



void apply180(int c, int r, A2 array2b, void *elem, void *cl){
  int r_new = methods->height(array2b) - 1 - r;
  int c_new = methods->width(array2b) - 1 - c;
  struct Pnm_rgb* newElement = methods->at(*(A2 *)cl, c_new, r_new);
  *newElement = *(struct Pnm_rgb*)elem;
}

//___________________________end of apply functions_____________________________________________________




int main(int argc, char *argv[]) {
  int rotation = 0;
  
  methods = array2_methods_blocked;
  

  
  A2Methods_mapfun *map = methods->map_default;
  assert(methods);
  assert(map);


  
#define SET_METHODS(METHODS, MAP, WHAT) do { \
      methods = (METHODS); \
      assert(methods); \
      map = methods->MAP; \
      if (!map) { \
        fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
        exit(1); \
      } \
    } while(0)

  int i;
  for (i = 1; i < argc; i++) {
    
    if (!strcmp(argv[i], "-row-major")) {
      SET_METHODS(array2_methods_plain, map_row_major, "row-major");
    } else if (!strcmp(argv[i], "-col-major")) {

      SET_METHODS(array2_methods_plain, map_col_major, "column-major");
    } else if (!strcmp(argv[i], "-block-major")) {
      
      SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
    } else if (!strcmp(argv[i], "-rotate")) {
      assert(i + 1 < argc);
      char *endptr;
      rotation = strtol(argv[++i], &endptr, 10);
      assert(*endptr == '\0'); // parsed all correctly
      assert(rotation == 0   || rotation == 90
          || rotation == 180 || rotation == 270);
    } else if (!strcmp(argv[i], "-transpose")){
      rotation = 69;
    }else if (!strcmp(argv[i], "-flip")){
      if(!strcmp(argv[i+1], "vertical")){
        rotation = 419;
      }
      else if (!strcmp(argv[i+1], "horizontal")){
        rotation = 420;
      }
    } else if (*argv[i] == '-') {
      fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
      exit(1);
    } else if (argc - i > 2) {
      fprintf(stderr, "Usage: %s [-rotate <angle>] "
              "[-{row,col,block}-major] [filename]\n", argv[0]);
      exit(1);
    } 
      else {
        break;
      }
  }

  // below are the logic for calling apply funcions, I know I know it is ugly and repetitive but I'm so tired and is almost 10pm on friday
  // just got everything to work so...... ya



 Pnm_ppm pnm_struct;
    FILE *open;
    char *lastarg = argv[argc-1];
    char buffer[127];
    strcpy(buffer, lastarg);
    int ch = '/';
    int chdot = '.';
    if(strchr(buffer, ch) || strchr(buffer, chdot)){
      open = fopen (argv[argc-1], "r");
      assert(open);
      pnm_struct = Pnm_ppmread(open, methods);
    }else{
      open = stdin;
      assert(open);
      pnm_struct = Pnm_ppmread(open, methods);
    }

  //Pnm_ppm pnm_struct = readpnm("csc/411/images/large/mobo.ppm", methods);
  A2 array = pnm_struct->pixels;


  if(rotation == 0){
    hard_print_tofile(pnm_struct);
  }else if(rotation == 180){
    // each array_final would have a different size depending on the applyfunction
    A2 array_final = methods->new_with_blocksize(methods->width(array), methods->height(array),sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, apply180, (void *)&array_final);
    pnm_struct->width = methods->width(array);
    pnm_struct->height = methods->height(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }else if(rotation == 90){
    
    A2 array_final = methods->new_with_blocksize(methods->height(array), methods->width(array), sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, apply90, (void *)&array_final);
    pnm_struct->width = methods->height(array);
    pnm_struct->height = methods->width(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }else if(rotation == 270){
    
    A2 array_final = methods->new_with_blocksize(methods->height(array), methods->width(array), sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, apply270, (void *)&array_final);
    pnm_struct->width = methods->height(array);
    pnm_struct->height = methods->width(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }else if(rotation == 69){
    A2 array_final = methods->new_with_blocksize(methods->height(array), methods->width(array), sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, applyTranspose, (void *)&array_final);
    pnm_struct->width = methods->height(array);
    pnm_struct->height = methods->width(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }else if(rotation == 419){
    A2 array_final = methods->new_with_blocksize(methods->width(array), methods->height(array),sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, applyVflip, (void *)&array_final);
    pnm_struct->width = methods->width(array);
    pnm_struct->height = methods->height(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }else if(rotation == 420){
    A2 array_final = methods->new_with_blocksize(methods->width(array), methods->height(array),sizeof(struct Pnm_rgb), methods->blocksize(array));
    
    map(array, applyHflip, (void *)&array_final);
    pnm_struct->width = methods->width(array);
    pnm_struct->height = methods->height(array);
    pnm_struct->pixels = array_final;
    
    hard_print_tofile(pnm_struct);
  }

}