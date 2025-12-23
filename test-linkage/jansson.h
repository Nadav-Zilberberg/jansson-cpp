#ifndef JANSSON_H
#define JANSSON_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
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

/* Opaque structure for JSON values */
struct json_t;
typedef struct json_t json_t;

/* Reference counting */
void json_incref(json_t *json);
void json_decref(json_t *json);

/* Creation functions */
json_t *json_object(void);
json_t *json_array(void);
json_t *json_string(const char *value);
json_t *json_integer(json_int_t value);
json_t *json_real(json_real_t value);
json_t *json_true(void);
json_t *json_false(void);
json_t *json_null(void);

/* Type checking functions */
int json_is_object(const json_t *json);
int json_is_array(const json_t *json);
int json_is_string(const json_t *json);
int json_is_integer(const json_t *json);
int json_is_real(const json_t *json);
int json_is_number(const json_t *json);
int json_is_true(const json_t *json);
int json_is_false(const json_t *json);
int json_is_boolean(const json_t *json);
int json_is_null(const json_t *json);

/* Memory allocation functions */
void json_set_alloc_funcs(void *(*malloc_fn)(size_t), void (*free_fn)(void *));
void json_get_alloc_funcs(void *(**malloc_fn)(size_t), void (**free_fn)(void *));
void json_set_alloc_funcs2(void *(*malloc_fn)(size_t), void *(*realloc_fn)(void *, size_t), void (*free_fn)(void *));
void json_get_alloc_funcs2(void *(**malloc_fn)(size_t), void *(**realloc_fn)(void *, size_t), void (**free_fn)(void *));

#ifdef __cplusplus
}
#endif

#endif /* JANSSON_H */
