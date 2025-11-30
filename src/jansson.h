#ifndef JANSSON_H
#define JANSSON_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    JSON_ERROR_NONE,
    JSON_ERROR_OUT_OF_MEMORY,
    JSON_ERROR_INVALID_FORMAT,
    JSON_ERROR_INVALID_VALUE,
    JSON_ERROR_INVALID_STRING,
    JSON_ERROR_INVALID_NUMBER,
    JSON_ERROR_INVALID_ARRAY,
    JSON_ERROR_INVALID_OBJECT,
    JSON_ERROR_END_OF_INPUT,
    JSON_ERROR_UNEXPECTED_CHARACTER,
    JSON_ERROR_DUPLICATE_KEY
} json_error_code_t;

typedef long long json_int_t;

typedef struct json_t json_t;
typedef struct json_error_t {
    int line;
    int column;
    int position;
    char source[80];
    char text[160];
} json_error_t;

/* Forward declarations */
json_t* json_object(void);
json_t* json_array(void);
json_t* json_string(const char* value);
json_t* json_stringn(const char* value, size_t len);
json_t* json_integer(json_int_t value);
json_t* json_real(double value);
json_t* json_true(void);
json_t* json_false(void);
json_t* json_null(void);

json_t* json_pack(const char* fmt, ...);
json_t* json_pack_ex(json_error_t* error, size_t flags, const char* fmt, ...);
int json_unpack(json_t* root, const char* fmt, ...);
int json_unpack_ex(json_t* root, json_error_t* error, size_t flags, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* JANSSON_H */
