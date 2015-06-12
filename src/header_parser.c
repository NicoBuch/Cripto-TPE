
#include "header_parser.h"
#include "image_struct.h"

void parse_header(IMAGE* image, char* image_name){

  char *header = (char *) malloc (sizeof(char)*HEADER_SIZE);
  if (header == NULL) {fputs ("Memory error",stderr); exit (2);}
  FILE *f1 = fopen(image_name, "r");
  if(f1==NULL){fputs ("File error",stderr); exit (1);}

  int newLen = fread(header, sizeof(char), HEADER_SIZE , f1);

  //Leo los bytes del header
  unsigned int start_image = ((header[13] << 24) + (header[12] << 16) + (header[11] << 8) + (header[10]));
  unsigned int width = ((header[21] << 24) + (header[20] << 16) + (header[19] << 8) + (header[18]));
  unsigned int height = ((header[25] << 24) + (header[24] << 16) + (header[23] << 8) + (header[22]));
  unsigned int size = ((header[37] << 24) + (header[36] << 16) + (header[35] << 8) + (header[34]));

  image->start_image = start_image;
  image->width = width;
  image->height = height;

  //Cierro el archivo
  fclose(f1);
}

char* get_image_content(IMAGE* image, char* image_name){

  printf("EMPIEZA EN  : %d ", image->start_image);
  // Aca dejo en buffer la imagen secreta
  FILE *f1 = fopen(image_name, "r");
  fseek(f1, 0 , SEEK_END);
  int inputSize = ftell(f1);
  fseek(f1, image->start_image , SEEK_SET);

  char *image_content = malloc(image->width * image->height + 1);
  fread(image_content, sizeof(char), image->width * image->height, f1);
  fclose(f1);
  return image_content;
}
