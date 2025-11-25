#ifndef JANSSON_PRIVATE_H
#define JANSSON_PRIVATE_H

#include "jansson.h"
#include <stddef.h>

/* Define hashtable_t */
typedef struct hashtable_t {
    size_t size;
    size_t used;
    struct hashtable_bucket_t *buckets;
} hashtable_t;

/* Define json_t and related types */
struct json_t {
    unsigned char type;
    size_t refcount;
};

typedef struct {
    json_t json;
    hashtable_t hashtable;
} json_object_t;

typedef struct {
    json_t json;
    size_t size;
    size_t entries;
    json_t **table;
} json_array_t;

typedef struct {
    json_t json;
    size_t length;
    char *value;
} json_string_t;

typedef struct {
    json_t json;
    double value;
} json_real_t;

typedef struct {
    json_t json;
    json_int_t value;
} json_integer_t;

/* Other private declarations... */

#endif /* JANSSON_PRIVATE_H */
