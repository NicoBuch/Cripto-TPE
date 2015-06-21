#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "encription.h"
#include "time.h"
#include "common.h"

typedef unsigned char byte;
int evaluate_polynomius(int* polynomius,int x,int k);
void k_encode(image_t** shadows, image_t* secret, int shadow_count, int k);

int encode(image_t* secret, int k, int n, char * dir) {
    int i;

    int quantity = n !=0 ? n : k;
    image_t ** shadows = read_images_from_dir(dir, quantity);

    k_encode(shadows, secret, quantity,k);
    for(i = 0; i< quantity ; i++){
        char number[4];
        itoa(i,number,10);
        char shadow_name[80] = "./src/secret/encodeshadow";
        strcat(shadow_name, number);
        strcat(shadow_name, ".bmp");
        shadows[i]->id = shadow_name;
        write_image(shadows[i]);
    }
    return 0;
}

void k_encode(image_t** shadows, image_t* secret, int shadow_count, int k){
    int i,j,y,x,p;
    int polynomius[k];
    //Itero de a k bytes
        //cada byte es un coef del pol
        //por las n shadows genero un x y un y del pol
        //Escribo en esa shadow el x en el header y el y dentro de la imagen
    for(i = 0; i< secret->size - secret->offset; i += k){
        for(j = i; j< i+k; j++){
            polynomius[j-i] = secret->bytes[j];
        }
        for(x = 1; x <= shadow_count; x++){
            unsigned char mask = mask_last_bit;
            for(p = 1; p < 8/k; p++){
                mask = mask << 1 | 0x01;
            }
            y = evaluate_polynomius(polynomius, x, k);
            for(j = 0 ; j<k ; j++){
                //Hay que poner un bit en cada byte
                unsigned char bit_to_put = 0x00;

                bit_to_put = bit_to_put | (y & (mask << j * 8/k));
                bit_to_put = bit_to_put >> j*8/k;
                //En cada byte pongo 8/K bits.
                shadows[x-1]->bytes[i+k-1-j] = (shadows[x-1]->bytes[i+k-1-j] & full_mask<<(8/k)) | bit_to_put;
            }
        }
    }

    //Escribo el x en cada sombra.
    for(j = 1; j <= shadow_count; j++){
        shadows[j-1]->hidden_x = j;
    }
}

int evaluate_polynomius(int* polynomius,int x,int k){
    int i, sum = 0;
    for(i = 0; i<k; i++){
        sum += polynomius[i] * pow(x, i);
    }
    return sum % MODULUS;
}
