char* loadfile(char *file);

// typedef enum { INITIAL, SECRET, DIR, K, N, ERROR } state_t;

int main(int argc, char **argv){
  retrive();

  // state_t current_state = INITIAL;
  // int i, j;
  // char* dir, buffer;
  // char* secret_image;
  // // 0 for divide, 1 for retrieve
  // int divideOrRetrive, n, k;
  // for (i = 0; i < argc; ++i){
  //   if(current_state == ERROR){
  //     break;
  //   }

  //   if(i == 0)
  //     continue;
  //   else if(strcmp(argv[i], "-d") == 0){
  //     if(current_state != INITIAL){
  //       error(&current_state);
  //     }
  //     else{
  //     divideOrRetrive = 0;
  //     }
  //   }
  //   else if(strcmp(argv[i], "-r") == 0) {
  //     if(current_state != INITIAL){
  //       error(&current_state);
  //     }
  //     divideOrRetrive = 1;
  //   }
  //   else if(strcmp(argv[i], "-secret") == 0) {
  //     if(current_state != INITIAL){
  //       error(&current_state);
  //     }
  //     else{
  //       current_state = SECRET;
  //     }
  //   }

  //   else if(strcmp(argv[i], "-n") == 0) {
  //     if(current_state != INITIAL){
  //       error(&current_state);
  //     }
  //     else{
  //       current_state = N;
  //     }
  //   }

  //   else if(strcmp(argv[i], "-k") == 0) {
  //       if(current_state != INITIAL){
  //         error(&current_state);
  //       }
  //       else{
  //         current_state = K;
  //       }
  //     }

  //   else if(strcmp(argv[i], "-dir") == 0) {
  //       if(current_state != INITIAL){
  //         error(&current_state);
  //       }
  //       else{
  //         current_state = DIR;
  //       }
  //     }

  //   else{
  //     switch(current_state) {
  //       case SECRET : secret_image = argv[i]; current_state = INITIAL; break;
  //       case N : n = atoi(argv[i]); current_state = INITIAL; break;
  //       case K : k = atoi(argv[i]); current_state = INITIAL; break;
  //       case DIR : dir = argv[i]; current_state = INITIAL; break;
  //       default: error(&current_state); break;
  //     }
  //   }

  // }
  // printf("divideOrRetrive: %i, secret: %s, k: %i, n: %i, dir: %s\n", divideOrRetrive, secret_image, k, n, dir);

  // if(divideOrRetrive == 0){
  //   share(secret_image,k);
  //   printf("Despues de share");
  // /*
  //   int polinomius[inputsize/k][k];
  //   for(i = 0; i < inputsize/k ; i++){
  //     for(j = 0; j < k; j++){
  //       polinomius[i][j] = buffer[i*k + j] - '0'; // http://stackoverflow.com/questions/26024259/passing-argument-1-of-atoi-makes-pointer-from-integer-without-a-cast-can-any-b
  //     }
  //   }
  //   */

  // }




}

// int error(int *current_state){
//   printf("ARGUMENTOS INVALIDOS\n");
//   printf("%i\n", *current_state);
//   *current_state = ERROR;
// }
