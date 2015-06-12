/*
 * image_struct.h
 */

#ifndef IMAGE_STRUCT_H_
#define IMAGE_STRUCT_H_

typedef struct {
    unsigned int size;               /* File size in bytes        */
    unsigned int width,height;                /* Width and height of image */
    unsigned int start_image;
} IMAGE;

#endif /* IMAGE_STRUCT_H_ */
