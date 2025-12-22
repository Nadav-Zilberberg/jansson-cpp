#ifndef JANSSON_H
#define JANSSON_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Version */
#define JANSSON_MAJOR_VERSION  2
#define JANSSON_MINOR_VERSION  14
#define JANSSON_MICRO_VERSION  0

/* Version as a 3-byte hex number, e.g. 0x010201 == 1.2.1. Use this
   for numeric comparisons, e.g. #if JANSSON_VERSION_HEX >= ... */
#define JANSSON_VERSION_HEX  ((JANSSON_MAJOR_VERSION << 16) |   \
                              (JANSSON_MINOR_VERSION << 8)  |   \
                              (JANSSON_MICRO_VERSION << 0))

/* If __GNUC__ is defined, use the visibility attribute to control symbol
   export. Otherwise, use Windows DLL export/import. */
#if defined(__GNUC__) && (__GNUC__ >= 4)
  #define JANSSON_API __attribute__((visibility("default")))
#elif defined(_WIN32) || defined(__CYGWIN__)
  #ifdef DLL_EXPORT
    #define JANSSON_API __declspec(dllexport)
  #else
    #define JANSSON_API __declspec(dllimport)
  #endif
#else
  #define JANSSON_API
#endif

/* Types */
typedef long long json_int_t;
typedef double json_real_t;

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INTEGER,
    JSON_REAL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
} json_type;

typedef struct json_t {
    json_type type;
    size_t refcount;
} json_t;

typedef enum {
    JSON_ERROR_OUT_OF_MEMORY,
    JSON_ERROR_INVALID_FORMAT,
    JSON_ERROR_INVALID_VALUE,
    JSON_ERROR_INVALID_STRING,
    JSON_ERROR_INVALID_NUMBER,
    JSON_ERROR_END_OF_INPUT,
    JSON_ERROR_DUPLICATE_KEY
} json_error_code_t;

typedef struct json_error_t {
    json_error_code_t code;
    int line;
    int column;
    int position;
    char source[80];
    char text[160];
} json_error_t;

/* Constructors */
JANSSON_API json_t* json_object(void);
JANSSON_API json_t* json_array(void);
JANSSON_API json_t* json_string(const char* value);
JANSSON_API json_t* json_stringn(const char* value, size_t len);
JANSSON_API json_t* json_stringn_nocheck(const char* value, size_t len);
JANSSON_API json_t* json_integer(json_int_t value);
JANSSON_API json_t* json_real(json_real_t value);
JANSSON_API json_t* json_true(void);
JANSSON_API json_t* json_false(void);
JANSSON_API json_t* json_null(void);

/* Memory management */
JANSSON_API typedef void* (*json_malloc_t)(size_t);
JANSSON_API typedef void* (*json_realloc_t)(void*, size_t);
JANSSON_API typedef void (*json_free_t)(void*);

JANSSON_API void json_set_alloc_funcs(json_malloc_t malloc_fn, json_free_t free_fn);
JANSSON_API void json_set_alloc_funcs2(json_malloc_t malloc_fn, json_realloc_t realloc_fn, json_free_t free_fn);
JANSSON_API void json_get_alloc_funcs(json_malloc_t *malloc_fn, json_free_t *free_fn);
JANSSON_API void json_get_alloc_funcs2(json_malloc_t *malloc_fn, json_realloc_t *realloc_fn, json_free_t *free_fn);

/* Reference counting */
JANSSON_API void json_incref(json_t *json);
JANSSON_API void json_decref(json_t *json);

/* Type checking */
JANSSON_API int json_is_object(const json_t *json);
JANSSON_API int json_is_array(const json_t *json);
JANSSON_API int json_is_string(const json_t *json);
JANSSON_API int json_is_integer(const json_t *json);
JANSSON_API int json_is_real(const json_t *json);
JANSSON_API int json_is_number(const json_t *json);
JANSSON_API int json_is_true(const json_t *json);
JANSSON_API int json_is_false(const json_t *json);
JANSSON_API int json_is_boolean(const json_t *json);
JANSSON_API int json_is_null(const json_t *json);

/* Object operations */
JANSSON_API json_t* json_object_get(const json_t* object, const char* key);
JANSSON_API int json_object_set(json_t* object, const char* key, json_t* value);
JANSSON_API int json_object_set_new(json_t* object, const char* key, json_t* value);
JANSSON_API int json_object_del(json_t* object, const char* key);
JANSSON_API int json_object_clear(json_t* object);
JANSSON_API int json_object_update(json_t* object, json_t* other);
JANSSON_API int json_object_update_existing(json_t* object, json_t* other);
JANSSON_API int json_object_update_missing(json_t* object, json_t* other);
JANSSON_API void* json_object_iter(json_t* object);
JANSSON_API void* json_object_iter_at(json_t* object, const char* key);
JANSSON_API void* json_object_iter_next(json_t* object, void* iter);
JANSSON_API const char* json_object_iter_key(void* iter);
JANSSON_API json_t* json_object_iter_value(void* iter);
JANSSON_API int json_object_iter_set(json_t* object, void* iter, json_t* value);
JANSSON_API int json_object_iter_set_new(json_t* object, void* iter, json_t* value);
JANSSON_API void json_object_seed(size_t seed);
JANSSON_API size_t json_object_size(const json_t* object);

/* Array operations */
JANSSON_API json_t* json_array_get(const json_t* array, size_t index);
JANSSON_API int json_array_set(json_t* array, size_t index, json_t* value);
JANSSON_API int json_array_set_new(json_t* array, size_t index, json_t* value);
JANSSON_API int json_array_append(json_t* array, json_t* value);
JANSSON_API int json_array_append_new(json_t* array, json_t* value);
JANSSON_API int json_array_insert(json_t* array, size_t index, json_t* value);
JANSSON_API int json_array_insert_new(json_t* array, size_t index, json_t* value);
JANSSON_API int json_array_remove(json_t* array, size_t index);
JANSSON_API int json_array_clear(json_t* array);
JANSSON_API int json_array_extend(json_t* array, json_t* other);
JANSSON_API size_t json_array_size(const json_t* array);

/* String operations */
JANSSON_API const char* json_string_value(const json_t* string);
JANSSON_API size_t json_string_length(const json_t* string);
JANSSON_API int json_string_set(json_t* string, const char* value);
JANSSON_API int json_string_setn(json_t* string, const char* value, size_t len);
JANSSON_API int json_string_setn_nocheck(json_t* string, const char* value, size_t len);

/* Integer operations */
JANSSON_API json_int_t json_integer_value(const json_t* integer);
JANSSON_API int json_integer_set(json_t* integer, json_int_t value);

/* Real operations */
JANSSON_API json_real_t json_real_value(const json_t* real);
JANSSON_API int json_real_set(json_t* real, json_real_t value);

/* Boolean operations */
JANSSON_API int json_boolean_value(const json_t* boolean);

/* Equality and copying */
JANSSON_API int json_equal(json_t* value1, json_t* value2);
JANSSON_API json_t* json_copy(json_t* value);
JANSSON_API json_t* json_deep_copy(json_t* value);

/* Serialization */
JANSSON_API char* json_dumps(const json_t* json, size_t flags);
JANSSON_API int json_dumpf(const json_t* json, FILE* output, size_t flags);
JANSSON_API int json_dumpfd(const json_t* json, int output, size_t flags);
JANSSON_API int json_dump_file(const json_t* json, const char* path, size_t flags);
JANSSON_API char* json_dumpb(const json_t* json, char* buffer, size_t size, size_t flags);

/* Deserialization */
JANSSON_API json_t* json_loads(const char* input, size_t flags, json_error_t* error);
JANSSON_API json_t* json_loadf(FILE* input, size_t flags, json_error_t* error);
JANSSON_API json_t* json_loadb(const char* buffer, size_t buflen, size_t flags, json_error_t* error);
JANSSON_API json_t* json_load_file(const char* path, size_t flags, json_error_t* error);

/* Error handling */
JANSSON_API json_error_code_t json_error_code(const json_error_t* error);

#ifdef __cplusplus
}
#endif

#endif /* JANSSON_H */
