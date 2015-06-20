#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>
#include "common.h"
#include "decrypt.h"
#include "file.h"
#include "itoa.h"

#define MODULUS 251

void decrypt_bytes(image_t * secret_image, image_t ** images, int index, int image_qty, int k);
int * resolve_matrix(int** polynomials, int k);
char get_mask(char k);


image_t * decrypt(char * directory, int k, char * img_name) {

	struct dirent* p_dirent;
  DIR* dir;
	dir = opendir(directory);
  int image_qty = 0;
	image_t * images[IMG_MAX];
	char * path;

	assure(dir != NULL, "Problem opening directory, check your sintax.\n");
  while ((p_dirent = readdir(dir)) != NULL) {
 	 	if(strstr(p_dirent->d_name, ".bmp") && image_qty < IMG_MAX) {
 	 		path = calloc(strlen(directory) + strlen(p_dirent->d_name) + 2, 1);
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, p_dirent->d_name);
        images[image_qty++] = read_image(path);
        free(path);
 	 	}
  }
  closedir(dir);

  assure(image_qty >= k, "You didn't provide the necessary amount of pics.\n");

  image_t * secret_image = (image_t *) malloc(sizeof(image_t));
  secret_image->id = img_name;
  printf("Image name : %s\n" , images[0]->id);
  secret_image->size = images[0]->size;
  secret_image->offset = images[0]->offset;
  printf("hiden x: %d\n", images[0]->hidden_x);
  secret_image->bytes = (unsigned char *) malloc(secret_image->size - secret_image->offset);
  secret_image->header = (unsigned char *) malloc(secret_image->offset);
  memcpy(secret_image->header, images[0]->header, images[0]->offset);

  int i;
  for (i = 0; i < secret_image->size - secret_image->offset; i += k) {
      decrypt_bytes(secret_image, images, i, image_qty, k);
  }
  return secret_image;
}


void decrypt_bytes(image_t * img, image_t ** imgs, int index, int image_qty, int k){
  unsigned char shadows[image_qty];
  int i,j;
  int** polynomials = (int**)malloc(sizeof(int*)* image_qty);
  for(i = 0; i < image_qty; i++){
    polynomials[i] = (int *) malloc((k+1) * sizeof(int));
  }
  char aux_byte = 0;
  char mask = get_mask(k);
  /*Retrieve shadows from the 8 bytes starting in index in each image*/
  for(i=0; i< image_qty; i++){
    for(j = index; j < index+k ; j++){
      aux_byte = aux_byte << (8/k);
      aux_byte |= imgs[i]->bytes[j] & mask;
    }
    shadows[i] = aux_byte;
    aux_byte = 0;
  }
  for(i=0; i< image_qty; i++){
    for(j=0;j<k;j++){
      polynomials[i][j] = ((int)pow(imgs[i]->hidden_x, j)) % MODULUS;
    }
    polynomials[i][k] = shadows[i];
  }
  int* coeficients = resolve_matrix(polynomials, k);
  for(i = 0; i<k; i++){
    img->bytes[index + i] = (char)coeficients[i];
  }
  for(i = 0; i < image_qty; i++){
    free(polynomials[i]);
  }
  free(polynomials);
}



int * resolve_matrix(int** polynomials, int k){
  int col,row,x,inv,index_in_row,i;

  /* diagonalization of the matrix */
  for(col = 0; col< k-1 ; col++){
    inv = inverse(polynomials[col][col]);
    for(row = 1; row < k; row++){
      if(row > col){
        x = (inv*polynomials[row][col]) % MODULUS;
        for(index_in_row = col ; index_in_row < k+1; index_in_row++){
          polynomials[row][index_in_row] =
            mod(polynomials[row][index_in_row] - polynomials[col][index_in_row]*x, MODULUS);
        }
      }
    }
  }

  /* going back in the diagonalization */
  for(col = k-1; col>0 ; col--){
    inv = inverse(polynomials[col][col]);
    for(row = col-1; row >= 0; row--){
      x = (inv*polynomials[row][col]) % MODULUS;
      for(index_in_row = k ; index_in_row >= col; index_in_row--){
        polynomials[row][index_in_row] =
          mod(polynomials[row][index_in_row] - polynomials[col][index_in_row]*x, MODULUS);
      }
    }
  }

  /* retrieve coeficients*/
  int* coeficients = malloc(sizeof(int) * k);

  for(i = 0; i< k; i++){
    coeficients[i] = (polynomials[i][k] * inverse(polynomials[i][i])) % MODULUS;
  }
  return coeficients;

}


int mod(num, base){
  if(num > 0){
    return num % base;
  }

  while(num < 0){
    num += base;
  }
  return num;
}


char get_mask(char k){
  int i = 0;
  char mask = mask_last_bit;
  for(i=1; i< (8/k) ; i++){
    mask = mask<<1 | 0x01;
  }
  return mask;
}
