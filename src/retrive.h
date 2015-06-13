/*
 * retrive.h
 */

#ifndef RETRIVE_H_
#define RETRIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "share.h"
#include "dirent.h"
#include "md5.h"

#define P 251
#define HEADER_SIZE 54

#define BMP_HEADER_SIZE 54

typedef unsigned char byte;

typedef struct Variables{
        byte x;
        byte y;
        byte z;
} Variables;

typedef struct equation{
    byte coeficients [3];
    byte b; 
} Equation;

typedef struct EquationSystem
{
    int k;
    int n;

    //dimensiones: k
    Variables variables;

    //dimensiones: n x k
    Equation * equations;
} EquationSystem;

typedef struct {
    unsigned short type;                 /* Magic identifier            */
    unsigned int size;                       /* Header size in bytes          */
    unsigned int reserved;
    unsigned int offset;                     /* Offset to image data, bytes */
} HEADER;

typedef struct {
    unsigned int size;               /* File size in bytes      */
    int width,height;                /* Width and height of image */
    unsigned short planes;       /* Number of colour planes   */
    unsigned short bits;         /* Bits per pixel            */
    unsigned int compression;        /* Compression type          */
    unsigned int imagesize;          /* Image size in bytes       */
    int xresolution,yresolution;     /* Pixels per meter          */
    unsigned int ncolours;           /* Number of colours         */
    unsigned int importantcolours;   /* Important colours         */
    unsigned char * bmp;
    HEADER * header;
    void * extra;
} IMAGE;

int retrive();
int pixelsInImage(IMAGE * secret);
int aux(int n, int k, char * secretFileName, IMAGE ** stegos, char* firstStego);
int writeAuthenticationBit(IMAGE ** stegos, int k, int n, int offset, int recovery);
IMAGE* createImage(char * secretFileName, IMAGE* imageToCopyStructure);
void buildAndSolveEquationSystem(EquationSystem * equationSystem,
        Variables * variables,
        IMAGE ** stegos, int k, int n, int offset);
void rebuildEquationFromPixelGroup(Equation * equation, unsigned char * pixelGroup, int k);
void rebuildEquation(Equation * equation, unsigned char * pixelGroup, int k);
int getBFromPixelGroup(unsigned char * pixelGroup, int k);
void addEquation(Equation * equations, Equation * equation, int offset);
int solveEquationSystem(EquationSystem * equationSystem, Variables * variables);
byte** equationToMatrix(EquationSystem * equationSystem,byte ** a);
int md5(unsigned int* input);
int gaussIntergerOriented(unsigned char ** A, int n, unsigned char * x);
int divide(int num, int den);
int reduce(int n);
int inv_mult(int n, int p);
IMAGE* loadBMPImage(char* path);
void inserVariablesToImage(Variables * variables, unsigned char * image, int k, int offset);
int copy_file(char *old_filename, char *new_filename);
int saveBMPImage(char * filenameWithPath, IMAGE * img);

#endif /* RETRIVE_H_ */
