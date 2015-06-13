#include "retrive.h"

int const SUCCESS = 0;
int const IMAGE_NO_DIVISIBLE_BY_K = 1;
int const AUTHENTICATION_ERROR = 2;

int retrive() {
    IMAGE** stegos = malloc(sizeof (IMAGE) * 8);
    char** stegoFilePaths = (char**)malloc(8 * sizeof(char*));
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("./group5")) != NULL) {
        /* print all the files and directories within directory */
        int i = 0;
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".bmp") != NULL) {
                char* path = (char*) malloc(1000);
                strcpy(path, "./group5");
                path = strcat(path, "/");
                path = strcat(path, ent->d_name);
                printf("Stego: %s\n", path);
                stegos[i] = loadBMPImage(path); //path IMAGE* stego

                stegoFilePaths[i] = (char*)malloc(sizeof(char) * strlen(path)+1);
                //TODO: lograr que path se copie en fileNames[i]
                strncpy(stegoFilePaths[i], path, strlen(path));
                //*(stegos + i) = stego;
                i++;
                if (i >= 8) break;
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        printf("directory could not be opened\n");
        return EXIT_FAILURE;
    }
    return aux( 8, 2, "decrypt.bmp", stegos, stegoFilePaths[0] );
}

//cantidad de pixeles que componen a la imagen
int pixelsInImage(IMAGE * secret){
    return secret->height * secret->width;
}

int aux(int n, int k, char * secretFileName, IMAGE ** stegos, char* firstStego) {
	//valido dimension de la primera stego
    int imageSize = pixelsInImage(*(stegos));
    if( imageSize % k != 0){
        return IMAGE_NO_DIVISIBLE_BY_K;
    }
    
    //TODO: build IMAGE * secret
    IMAGE * secret = createImage(secretFileName, *(stegos));
    printf("antes\n");

    int i = 0;
    for( ; i < imageSize ; i += k){
        
        //TODO: autenticar
        // int authenticationResult = writeAuthenticationBit(stegos, k , n, i, 1);
        // if(authenticationResult != 0){
        //      return AUTHENTICATION_ERROR;
        // }
        
        EquationSystem equationSystem;
        Variables variables;
        
        //recupera el sistema de ecuaciones y lo resuelve
        buildAndSolveEquationSystem(&equationSystem, &variables, stegos, k ,n, i);
        inserVariablesToImage(&variables, secret->bmp, k, i);
    }

    copy_file(firstStego, secretFileName);
    printf("%s, %s", firstStego, secretFileName);
    saveBMPImage(secretFileName, secret );
    
    return SUCCESS;
}

int writeAuthenticationBit(IMAGE ** stegos, int k, int n, int offset, int recovery) {
    int i;
    for (i = 0; i < n; i++) {
        unsigned char * pixelGroup = (*(stegos + i))->bmp + offset;
        if (k == 2) {
            unsigned char firstPart = *(pixelGroup + 1);
            unsigned char secondPart = *(pixelGroup + 1);
            firstPart &= 0b11100000; //~(1 << 3);
            secondPart &= 0b00001111;
            secondPart = (secondPart << 1);
            unsigned int stream = (*(pixelGroup)*256 + (firstPart | secondPart) / 2);
            int hashBit = md5(&stream);
            //TODO: append hash bit to image pixel
            //printf("before %u",*(pixelGroup+1));
            if (recovery == 0) {
                if (hashBit == 1) {
                    *(pixelGroup + 1) |= 1 << 4;
                } else if (hashBit == 0) {
                    *(pixelGroup + 1) &= ~(1 << 4);
                }
            } else {
                int authenticationBit = (*(pixelGroup + 1) & 0b00010000) / 16;
                if (authenticationBit != hashBit) {
                    if (authenticationBit == 2) return 2;
                }
            }
            //printf("after %u",*(pixelGroup+1));
        } else if (k == 3) {
            unsigned char firstPart = *(pixelGroup + 1);
            unsigned char secondPart = *(pixelGroup + 1);
            firstPart &= 0b11111000; //~(1 << 3);
            secondPart &= 0b00000011;
            secondPart = (secondPart << 1);
            unsigned int stream = (*(pixelGroup)*256 * 256 + *(pixelGroup + 1)*256 + (firstPart | secondPart) / 2);
            int hashBit = md5(&stream);
            //TODO: append hash bit to image pixel
            //printf("before %u",*(pixelGroup+1));
            if (recovery == 0) {
                if (hashBit == 1) {
                    *(pixelGroup + 2) |= 1 << 2;
                }
                else if (hashBit == 0) {
                    *(pixelGroup + 2) &= ~(1 << 2);
                }
            } else {
                int authenticationBit = (*(pixelGroup + 2) & 0b00000100) / 4;
                if (authenticationBit != hashBit) {
                    if (authenticationBit == 2) return 2;
                }
            }
        }
    }
    return 0;
}

/*Hago un deep copy de imageToCopyStructure, le cambio el nombre a la 
 * copia y la retorno
 * 
 */
IMAGE* createImage(char * secretFileName, IMAGE* imageToCopyStructure){
    
    IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));
    
    //copio los valores basicos del bmp
    memcpy(img, imageToCopyStructure, sizeof(IMAGE));
    
    //copio el header
    HEADER* header = (HEADER*)malloc(sizeof(HEADER));
    memcpy(header, imageToCopyStructure->header, sizeof(HEADER));
    img->header = header;
    
    //anulo extra
    img->extra = NULL;
    
    //aloco memoria para la matriz del bmp
    img->bmp = (unsigned char*) malloc(img->height* img->width );
    
    return img;
}

void buildAndSolveEquationSystem(EquationSystem * equationSystem,
        Variables * variables,
        IMAGE ** stegos, int k, int n, int offset) {

    //aloco memoria para las n ecuaciones
    Equation * equations = malloc(sizeof (Equation) * n * 10);
    // Equation equations [n];
    int i;
    for (i = 0; i < n; i++) {
        unsigned char * pixelGroup = (*(stegos + i))->bmp + offset;
        Equation equation;
        rebuildEquationFromPixelGroup(&equation, pixelGroup, k);
        addEquation(equations, &equation, i);
    }

    equationSystem->k = k;
    equationSystem->n = n;
    equationSystem->equations = equations;

    //(equationSystem->equations + sizeof(Equation) * i)->coeficients[1]);

    //TODO: resolver el sistema y popular variables
    solveEquationSystem(equationSystem, variables);
    equationSystem->variables = *variables;
}

void rebuildEquationFromPixelGroup(Equation * equation, unsigned char * pixelGroup, int k) {
    rebuildEquation(equation, pixelGroup, k);

    //obtener b desde el pixel group
    equation->b = getBFromPixelGroup(pixelGroup, k);
}

void rebuildEquation(Equation * equation, unsigned char * pixelGroup, int k) {
    if (k == 2) {
        equation->coeficients[0] = (*pixelGroup & 0xF0) >> 4;
        equation->coeficients[1] = (*(pixelGroup + 1) & 0xE0) >> 5;
    } else {
        //k == 3               
        equation->coeficients[0] = (*pixelGroup & 0xF8) >> 3;
        equation->coeficients[1] = (*(pixelGroup + 1) & 0xF8) >> 3;
        equation->coeficients[2] = (*(pixelGroup + 2) & 0xF8) >> 3;
    }
}

int getBFromPixelGroup(unsigned char * pixelGroup, int k) {
    unsigned char b;
    if (k == 2) {
        b = ((*pixelGroup & 0x0F) << 4) | (*(pixelGroup + 1) & 0x0F);

    } else {
        //k == 3
        unsigned char b1 = ((*pixelGroup) & 0x07) << 5;
        unsigned char b2 = ((*(pixelGroup + 1) & 0x07) << 2);
        unsigned char b3 = (*(pixelGroup + 2) & 0x03);
        b = b1 | b2 | b3;
    }

    int bb = (int) b;
    return bb;
}

void addEquation(Equation * equations, Equation * equation, int offset) {
    void * dest = equations + sizeof (Equation) * offset;
    memcpy(dest, equation, sizeof (Equation));
}

int solveEquationSystem(EquationSystem * equationSystem, Variables * variables) {
   // n es cantidad de ec,
   int n = equationSystem->n;
   // k icognitas 
   int k = equationSystem->k;
   
   byte ** a = (byte **) malloc(n * sizeof (byte *));
   equationToMatrix(equationSystem, a);
  
   byte * x = (byte *)malloc(n*sizeof(byte *));

   int ans = gaussIntergerOriented(a,k,x);

   if (ans == 0){
//TODO EXIT FAILURE
      return 1;
   }
   
   variables->x = x[0];
   variables->y = x[1];
   variables->z = x[2];
   return 0;
}

byte** equationToMatrix(EquationSystem * equationSystem,byte ** a) {

    int i, j, w;
    // n es cantidad de ec,
    int n = equationSystem->n;
    // k icognitas
    int k = equationSystem->k;


    //a = (byte **) malloc(n * sizeof (byte *));
    for (w = 0; w < n; w++)
        a[w] = (byte *) malloc((k + 1) * sizeof (byte));

    //k filas, n columnas
    for (i = 0; i < n; i++) {
        for (j = 0; j < k; j++) {
            a[i][j] = (byte) (equationSystem->equations + sizeof (Equation) * i)->coeficients[j];
            // a[i][j] = (byte)equationSystem->equations[i].coeficients[j];  

        }
        a[i][k] = (byte) (equationSystem->equations + sizeof (Equation) * i)->b;
        // a[i][j] = (byte)equationSystem->equations[i].b;  

    }
    return a;
}

int md5(unsigned int* input) {
    md5_state_t state;
    md5_byte_t digest[16];
    int di;

    md5_init(&state);
    md5_append(&state, (const md5_byte_t *) input, 4);
    md5_finish(&state, digest);
    //printf("MD5 (%hu) = ", *input);
    md5_byte_t xor = digest[0];
    for (di = 1; di < 16; ++di) {
        //printf("%02x", digest[di]);
        //shift
        xor ^= digest[di];
    }
    //Xor within byte
    for (di = 1; di < 8; ++di) {
        md5_byte_t x2 = xor << di;
        xor ^= x2;
    }
    xor = xor >> 7;
    //printf("%d", xor);
    //printf("\n");
    return xor;
}

int gaussIntergerOriented(unsigned char ** A, int n, unsigned char * x) {
    int i,j,k;
    
    int c,sum=0.0;
    
    for(j=0;j<n;j++)    /* loop for the generation of upper triangular matrix*/
    {
        for(i=0;i<n;i++)
        {
            if(i>j)
            {
                c = divide( A[i][j], A[j][j] );
                for(k=0;k<n+1;k++)
                {
                    A[i][k]= reduce( A[i][k]- reduce(c*A[j][k] ) );
                }
            }
        }
    }  
//    --------------------------------------------
//            
//        for(j=1;j<=n;j++)    /* loop for the generation of upper triangular matrix*/
//    {
//        for(i=1;i<=n;i++)
//        {
//            if(i>j)
//            {
//                c = divide( A[i][j], A[j][j] );
//                for(k=1;k<=n+1;k++)
//                {
//                    A[i][k]= reduce( A[i][k]- reduce(c*A[j][k] ) );
//                }
//            }
//        }
//    }        
//            
//    -------------------------------------------        
    
    x[n-1]=divide(A[n-1][n],A[n-1][n-1]);
    /* this loop is for backward substitution*/
    for(i=n-2;i>=0;i--)
    {
        sum=0;

        for(j=i+1;j<n;j++)
        {
            sum= reduce( sum+A[i][j]*x[j] );
        }
        x[i]= divide( (A[i][n]-sum), A[i][i] );
    }
    
//    ---------------------------------
//    x[n]=divide(A[n][n+1],A[n][n]);
//    /* this loop is for backward substitution*/
//    for(i=n-1;i>=1;i--)
//    {
//        sum=0;
//
//        for(j=i+1;j<=n;j++)
//        {
//            sum= reduce( sum+A[i][j]*x[j] );
//        }
//        x[i]= divide( (A[i][n+1]-sum), A[i][i] );
//    }        
//            
//    -------------------------------
    
   
}

int divide(int num, int den){
    int division = num * inv_mult(den, 251);
    return reduce(division);
}

int reduce(int n){
    //return n % 251;
    return (n % 251 + 251) % 251;
}

int inv_mult(int n, int p){
    int x;
    for(x=0; x < p ; x++){
        if( n*x % p == 1 ){
            return x;
        }
    }
    return -1;
}

IMAGE* loadBMPImage(char* path) {

    int n = 0;
    HEADER* header = (HEADER*)malloc(sizeof(HEADER));
    
    IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));
    
    FILE* file = fopen(path, "rb");
    if (file == NULL) return NULL;
    
    n=fread(&header->type, sizeof(unsigned short), 1, file);
    n=fread(&header->size, sizeof( unsigned int), 1, file);
    n=fread(&header->reserved, sizeof( unsigned int), 1, file);
    n=fread(&header->offset, sizeof( unsigned int), 1, file);
    
    img->header = header;
    
    n=fread(&img->size, sizeof( unsigned int), 1, file);
    n=fread(&img->width, sizeof(  int), 1, file);
    n=fread(&img->height, sizeof(int), 1, file);
    n=fread(&img->planes, sizeof( unsigned short), 1, file);
    n=fread(&img->bits, sizeof( unsigned short), 1, file);
    n=fread(&img->compression, sizeof( unsigned int), 1, file);
    n=fread(&img->imagesize, sizeof( unsigned int), 1, file);
    n=fread(&img->xresolution, sizeof( int), 1, file);
    n=fread(&img->yresolution, sizeof(  int), 1, file);
    n=fread(&img->ncolours, sizeof( unsigned int), 1, file);
    n=fread(&img->importantcolours, sizeof( unsigned int), 1, file);
    
    int toread = header->offset - BMP_HEADER_SIZE;
    img->extra = malloc(toread);
    fread(img->extra,toread,1, file);

    if (n <1){
        printf("Error al leer la imagen\n");
        free(img);
        return NULL;
    } 

    img->bmp = (unsigned char*) malloc(img->height* img->width );
    int i=0;
    for (i=0; i < img->height * img->width; i++){
            n=fread(&img->bmp[i], 1, 1, file);

    }

    fclose(file);
    
    return img;
}

void inserVariablesToImage(Variables * variables, unsigned char * image, int k, int offset) {
    *(image + offset) = variables->x;
    *(image + offset + 1) = variables->y;
    if (k == 3) {
        *(image + offset + 2) = variables->z;
    }
}

int copy_file(char *old_filename, char *new_filename) {
    char ch;
    FILE *source, *target;

    source = fopen(old_filename, "r");

    if (source == NULL) {
        exit(EXIT_FAILURE);
    }
    target = fopen(new_filename, "w");

    if (target == NULL) {
        fclose(source);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(source)) != EOF)
        fputc(ch, target);

    fclose(source);
    fclose(target);

    return 0;
}

int saveBMPImage(char * filenameWithPath, IMAGE * img) {
    int n = 0;
    //int fd = open(filenameWithPath);
    //close(fd);
    FILE* file = fopen(filenameWithPath, "r+");
    if (file == NULL) return 0;

    fseek(file,img->header->offset,SEEK_SET);
    unsigned char test = 0;
    int i = 0;
    for (i=0; i < img->height * img->width; i++){
            n=fwrite(&img->bmp[i], 1, 1, file);//
    }
 
    fclose(file);
    return 1;
}