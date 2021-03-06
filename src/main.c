#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "common.h"
#include "file.h"
#include "decrypt.h"

#define FALSE 0
#define TRUE !FALSE

image_t* extract_image(char* filename);


int main(int argc, char* argv[]) {
  //Parseo argumentos
  int isRecovery = FALSE;
  int k = -1;
  int n = 0;
  char * image = calloc(sizeof(char), 40);
  char * dir = calloc(sizeof(char), 40);

  int opt = 0;
  static struct option long_options[] = {
        {"secret",    required_argument, 0,  's' },
        {"dir",       required_argument, 0,  'i' },
        {"d",         no_argument,       0,  'd' },
        {"r",         no_argument,       0,  'r' },
        {"k",         required_argument, 0,  'k' },
        {"n",         required_argument, 0,  'n' },
        {0,           0,                 0,  0   }
  };

  int long_index =0;
  while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
    switch (opt){
      case 's':
        strcpy(image, optarg);
        break;
      case 'i':
        strcpy(dir, optarg);
        break;
      case 'd':
        isRecovery = FALSE;
        break;
      case 'r':
        isRecovery = TRUE;
        break;
      case 'k':
        k = atoi(optarg);
        break;
      case 'n':
        n = atoi(optarg);
        break;
      default:
        printf("dafuq am I doing here\n");
        break;
    }
  }
  //-------End parsing arguments

    //image_t* secret_image = extract_image("src\\secret.bmp");
    //int result = encode(secret_image,2);

  assure(k != -1, "You must specify k.\n");
  assure(k == 2 || k == 4 || k == 8, "Wrong value for k.\n");
  assure(dir[0] != 0, "Please specify a directory.\n");

  if (isRecovery) {
    //recovery
    write_image(decrypt(dir, k, image));
  } else {
    encode(read_image(image), k, n, dir);
  }

  return EXIT_SUCCESS;
}

image_t*  extract_image(char* filename) {
    image_t* image = read_image(filename);
  //  write_image(image);
    return image;
}
