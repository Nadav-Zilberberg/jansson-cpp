#ifndef JANSSON_H
#define JANSSON_H

#ifdef __cplusplus
#include "error.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

#include "jansson_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Version */
#define JANSSON_MAJOR_VERSION  2
#define JANSSON_MINOR_VERSION  14
#define JANSSON_MICRO_VERSION  0

/* Type flags */
#define JSON_OBJECT     0x01
#define JSON_ARRAY      0x02
#define JSON_STRING     0x04
#define JSON_INTEGER    0x08
#define JSON_REAL       0x10
#define JSON_TRUE       0x20
#define JSON_FALSE      0x40
#define JSON_NULL       0x80

/* Forward declarations */
typedef struct json_t json_t;
typedef long long json_int_t;

/* Error handling (C interface) */
#ifdef __cplusplus
#define JSON_ERROR_TEXT_LENGTH    160
#define JSON_ERROR_SOURCE_LENGTH   80

struct json_error_t {
    int line;
    int column;
    int position;
    char source[JSON_ERROR_SOURCE_LENGTH];
    char text[JSON_ERROR_TEXT_LENGTH];
};

enum json_error_code {
    json_error_unknown,
    json_error_out_of_memory,
    json_error_stack_overflow,
    json_error_cannot_open_file,
    json_error_invalid_argument,
    json_error_invalid_utf8,
    json_error_premature_end_of_input,
    json_error_end_of_input_expected,
    json_error_invalid_syntax,
    json_error_invalid_format,
    json_error_wrong_type,
    json_error_null_character,
    json_error_null_value,
    json_error_null_byte_in_key,
    json_error_duplicate_key,
    json_error_numeric_overflow,
    json_error_item_not_found,
    json_error_index_out_of_range
};

static JSON_INLINE enum json_error_code json_error_get_code(const json_error_t *e) {
    return (enum json_error_code)e->text[JSON_ERROR_TEXT_LENGTH - 1];
}

void json_error_init(json_error_t *error, const char *source);
void json_error_set_source(json_error_t *error, const char *source);
void json_error_set(json_error_t *error, int line, int column, size_t position,
                    enum json_error_code code, const char *msg, ...);
void json_error_vset(json_error_t *error, int line, int column, size_t position,
                     enum json_error_code code, const char *msg, va_list ap);
#else
#define JSON_ERROR_TEXT_LENGTH    160
#define JSON_ERROR_SOURCE_LENGTH   80

typedef struct json_error_t {
    int line;
    int column;
    int position;
    char source[JSON_ERROR_SOURCE_LENGTH];
    char text[JSON_ERROR_TEXT_LENGTH];
} json_error_t;

typedef enum json_error_code {
    json_error_unknown,
    json_error_out_of_memory,
    json_error_stack_overflow,
    json_error_cannot_open_file,
    json_error_invalid_argument,
    json_error_invalid_utf8,
    json_error_premature_end_of_input,
    json_error_end_of_input_expected,
    json_error_invalid_syntax,
    json_error_invalid_format,
    json_error_wrong_type,
    json_error_null_character,
    json_error_null_value,
    json_error_null_byte_in_key,
    json_error_duplicate_key,
    json_error_numeric_overflow,
    json_error_item_not_found,
    json_error_index_out_of_range
} json_error_code;

static JSON_INLINE enum json_error_code json_error_get_code(const json_error_t *e) {
    return (enum json_error_code)e->text[JSON_ERROR_TEXT_LENGTH - 1];
}

void json_error_init(json_error_t *error, const char *source);
void json_error_set_source(json_error_t *error, const char *source);
void json_error_set(json_error_t *error, int line, int column, size_t position,
                    enum json_error_code code, const char *msg, ...);
void json_error_vset(json_error_t *error, int line, int column, size_t position,
                     enum json_error_code code, const char *msg, va_list ap);
#endif

/* Other jansson functions... */

#ifdef __cplusplus
}
#endif

#endif /* JANSSON_H */
