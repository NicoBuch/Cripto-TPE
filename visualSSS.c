#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* loadfile(char *file);

typedef enum { INITIAL, SECRET, DIR, K, N, ERROR } state_t;

int main(int argc, char **argv){
  state_t current_state = INITIAL;
  int i, j;
  char* dir, buffer;
  char* secret_image;
  // 0 for divide, 1 for retrieve
  int divideOrRetrive, n, k;
  for (i = 0; i < argc; ++i){
    if(current_state == ERROR){
      break;
    }

    if(i == 0)
      continue;
    else if(strcmp(argv[i], "-d") == 0){
      if(current_state != INITIAL){
        error(&current_state);
      }
      else{
      divideOrRetrive = 0;
      }
    }
    else if(strcmp(argv[i], "-r") == 0) {
      if(current_state != INITIAL){
        error(&current_state);
      }
      divideOrRetrive = 1;
    }
    else if(strcmp(argv[i], "-secret") == 0) {
      if(current_state != INITIAL){
        error(&current_state);
      }
      else{
        current_state = SECRET;
      }
    }

    else if(strcmp(argv[i], "-n") == 0) {
      if(current_state != INITIAL){
        error(&current_state);
      }
      else{
        current_state = N;
      }
    }

    else if(strcmp(argv[i], "-k") == 0) {
        if(current_state != INITIAL){
          error(&current_state);
        }
        else{
          current_state = K;
        }
      }

    else if(strcmp(argv[i], "-dir") == 0) {
        if(current_state != INITIAL){
          error(&current_state);
        }
        else{
          current_state = DIR;
        }
      }

    else{
      switch(current_state) {
        case SECRET : secret_image = argv[i]; current_state = INITIAL; break;
        case N : n = atoi(argv[i]); current_state = INITIAL; break;
        case K : k = atoi(argv[i]); current_state = INITIAL; break;
        case DIR : dir = argv[i]; current_state = INITIAL; break;
        default: error(&current_state); break;
      }
    }

  }
  printf("divideOrRetrive: %i, secret: %s, k: %i, n: %i, dir: %s\n", divideOrRetrive, secret_image, k, n, dir);

  if(divideOrRetrive == 0){
    // Aca dejo en buffer la imagen secreta
    FILE *f1 = fopen(secret_image, "r");
    fseek(f1, 0, SEEK_SET);
    int inputsize = ftell(f1); //its about text's size
    char buffer[inputsize];

    for(i = 0; i < inputsize; i++)
    {
      fscanf(f1, "%c", &buffer[i]);
    }
    if(inputsize % k != 0){
      printf("EL VALOR DE K TIENE QUE SER DIVISOR DE LA CANTIDAD DE PIXELES DE LA IMAGEN\n");
      exit(1);
    }
    int polinomius[inputsize/k][k];
    for(i = 0; i < inputsize/k ; i++){
      for(j = 0; j < k; j++){
        polinomius[i][j] = atoi(buffer[i*k + j]);
      }
    }

  }




}

int error(int *current_state){
  printf("ARGUMENTOS INVALIDOS\n");
  printf("%i\n", *current_state);
  *current_state = ERROR;
}

