/*
 * share.h
 */

#ifndef SHARE_H_
#define SHARE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P 251
#define HEADER_SIZE 54

void share(char* secret_name, int k);
char* cutHeader(char* image_buffer);

#endif /* SHARE_H_ */
