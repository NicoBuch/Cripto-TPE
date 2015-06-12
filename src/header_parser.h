/*
 * header_parser.h
 */

#ifndef HEADER_PARSER_H_
#define HEADER_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_struct.h"

#define HEADER_SIZE 54

void retrive(int n, int k, char * secretFileName, IMAGE ** stegos);

#endif /* HEADER_PARSER_H_ */
