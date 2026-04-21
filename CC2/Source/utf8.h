//
//  utf8.h
//  training
//
//  Created by Conrad Kleinespel on 5/27/13.
//  Copyright (c) 2013 Conrad Kleinespel. All rights reserved.
//

#ifndef training_utf8_h
#define training_utf8_h


#ifndef LINUX
typedef __int32 int32_t;
#endif

#include <stdint.h>

int32_t utf8_is_continuation(char c);
int32_t utf8_validate(char * s);
size_t utf8_strlen(char * s);
int32_t utf8_is_single_byte(char * c);
int32_t utf8_is_double_byte(char * c);
int32_t utf8_is_triple_byte(char * c);
int32_t utf8_is_quadruple_byte(char * c);
char * utf8_remove_trailing_newline(char * s);
char * utf8_remove_char(char * s, size_t n);
char * utf8_add_char(char * s, char * c, size_t n);
char * utf8_replace(char * needle, char * replace, char * haystack);
char * utf8_replace_all(char * needle, char * replace, char * haystack);
size_t utf8_num_bytes(char * s);

// Escape the null bytes in the given string that has the given length
char * utf8_escape_null_bytes(const char * s, size_t num);

#endif