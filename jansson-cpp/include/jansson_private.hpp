/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_PRIVATE_H
#define JANSSON_PRIVATE_H

#include "hashtable.hpp"
#include "jansson.hpp"
#include "jansson_private_config.hpp"
#include "strbuffer.hpp"
#include <stddef.h>

namespace jansson {

#define JSON_PARSER_MAX_DEPTH 2048
#define JSON_INLINE inline

/* va_list is used in few places */
#include <stdarg.h>

/* Internal functions */

/* json_object */
const char* jsonp_object_iter_fullkey(void* iter);

/* json_string */

/* error reporting */
void jsonp_error_init(jansson::json_error_t* error, const char* source);
void jsonp_error_set_source(jansson::json_error_t* error, const char* source);
void jsonp_error_set(jansson::json_error_t* error, int line, int column, size_t position,
                     json_error_code code, const char* msg, ...);
void jsonp_error_vset(jansson::json_error_t* error, int line, int column, size_t position,
                      json_error_code code, const char* msg, va_list ap);

/* locale independent string<->number conversions */
int jsonp_strtod(strbuffer_t* strbuffer, double* out);
int jsonp_dtostr(char* buffer, size_t size, double value);

/* Wrappers for custom memory functions */
void* jsonp_malloc(size_t size);
void jsonp_free(void* ptr);
void* jsonp_realloc(void* ptr, size_t size);
char* jsonp_strdup(const char* str);
char* jsonp_strndup(const char* str, size_t len);

/* Windows compatibility */
#ifdef _WIN32
#define jsonp_snprintf _snprintf
#else
#define jsonp_snprintf snprintf
#endif

} // namespace jansson

#endif
