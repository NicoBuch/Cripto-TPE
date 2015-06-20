#include "share.h"
#include "header_parser.h"

void share(char* secret_image, int k){

  IMAGE *image = (IMAGE *)malloc(sizeof(IMAGE));
  parse_header(image,secret_image);
  printf("Imagen comienza: %u\n", image->start_image);
  printf("Imagen ancho: %u\n", image->width);
  printf("Imagen alto: %u\n", image->height);


  //Leo la imagen

  char* image_content = get_image_content(image,secret_image);

  free(image);

  // Aca dejo en buffer la imagen secreta
  // char buffer[inputSize];
  // char * image;
  // int i = 0;
  // for(i = 0; i < 200; i++)
  // {
  //   printf("%d", (int)(*(image_content+i)));
  // }
  //
  //
  //
  // printf("%d\n", inputSize+1);
  // printf("%lu\n", sizeof(buffer));
  // image = cutHeader(buffer);
  // printf("%lu\n" , sizeof(image));
  //
  // if(inputSize % k != 0){
  //   printf("EL VALOR DE K TIENE QUE SER DIVISOR DE LA CANTIDAD DE PIXELES DE LA IMAGEN\n");
  //   exit(1);
  // }
}

char* cutHeader(char* image_buffer){

  //Salteo el header
  image_buffer += HEADER_SIZE;

  return image_buffer;
}
