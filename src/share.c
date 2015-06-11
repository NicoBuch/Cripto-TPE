#include "share.h"

void share(char* secret_image, int k){

  // Aca dejo en buffer la imagen secreta
  FILE *f1 = fopen(secret_image, "r");
  fseek(f1, 0, SEEK_SET);
  int inputsize = ftell(f1); //its about text's size
  char buffer[inputsize];
  char * image;
  int i = 0;
  for(i = 0; i < inputsize; i++)
  {
    fscanf(f1, "%c", &buffer[i]);
  }
  image = cutHeader(buffer);

  if(inputsize % k != 0){
    printf("EL VALOR DE K TIENE QUE SER DIVISOR DE LA CANTIDAD DE PIXELES DE LA IMAGEN\n");
    exit(1);
  }

}

char* cutHeader(char* image_buffer){

  //Salteo el header
  image_buffer += HEADER_SIZE;

  return image_buffer;
}
