
#include "header_parser.h"
#include "image_struct.h"

void parse_header(IMAGE* image, char* image_name){

  char *header = (char *) malloc (sizeof(char)*HEADER_SIZE);
  if (header == NULL) {fputs ("Memory error",stderr); exit (2);}
  FILE *f1 = fopen(image_name, "r");
  if(f1==NULL){fputs ("File error",stderr); exit (1);}

  int newLen = fread(header, sizeof(char), HEADER_SIZE , f1);

  // int i = 0;
  // for(i = 0; i<HEADER_SIZE; i++){
  //   printf("CARACTER %d: \n" , i);
  //   printf("%c\n",header[i]);
  // }

  unsigned int start_image = ((header[13] << 24) + (header[12] << 16) + (header[11] << 8) + (header[10]));
  unsigned int width = ((header[21] << 24) + (header[20] << 16) + (header[19] << 8) + (header[18]));
  unsigned int height = ((header[25] << 24) + (header[24] << 16) + (header[23] << 8) + (header[22]));

  image->start_image = start_image;
  image->width = width;
  image->height = height;

}
