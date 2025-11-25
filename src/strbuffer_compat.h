/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef STRBUFFER_COMPAT_H
#define STRBUFFER_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// Keep the original struct layout for C compatibility
typedef struct strbuffer {
    char *value;
    size_t length;
    size_t size;
    void *cpp_buffer;  // Pointer to the actual C++ StringBuffer object
} strbuffer_t;

int strbuffer_init(strbuffer_t *strbuff);
void strbuffer_close(strbuffer_t *strbuff);
void strbuffer_clear(strbuffer_t *strbuff);
const char *strbuffer_value(const strbuffer_t *strbuff);
char *strbuffer_steal_value(strbuffer_t *strbuff);
int strbuffer_append_byte(strbuffer_t *strbuff, char byte);
int strbuffer_append_bytes(strbuffer_t *strbuff, const char *data, size_t size);
char strbuffer_pop(strbuffer_t *strbuff);

#ifdef __cplusplus
}
#endif

#endif /* STRBUFFER_COMPAT_H */
