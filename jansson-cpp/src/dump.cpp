/*
Copyright (c) 2009-2020 Petri Lehtinen <petri@digip.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <jansson.h>
#include <jansson_private.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <utility>
#include <new>
#include "jansson-cpp/include/jansson.hpp"
#include "jansson-cpp/include/jansson_private.hpp"

#define json_error_t jansson::json_error_t

#define MAX_INTEGER_STR_LENGTH 100
#define MAX_REAL_STR_LENGTH 100

#define FLAGS_TO_INDENT(f) ((f)&0x1F)
#define FLAGS_TO_PRECISION(f) (((f) >> 11) & 0x1F)

static int dump_to_buffer(const char *buffer, size_t size, void *data) {
    auto *buf = static_cast<dump_buffer_t*>(data);
    if (buf->size + size > buf->allocated) {
        size_t new_size = (buf->allocated + size) * 2;
        char *new_buffer = static_cast<char*>(jsonp_realloc(buf->buffer, new_size));
        if (!new_buffer)
            return -1;
        buf->buffer = new_buffer;
        buf->allocated = new_size;
    }
    memcpy(buf->buffer + buf->size, buffer, size);
    buf->size += size;
    return 0;
}

static int dump_to_file(const char *buffer, size_t size, void *data) {
    auto *stream = static_cast<FILE*>(data);
    if (fwrite(buffer, size, 1, stream) != 1)
        return -1;
    return 0;
}

static int do_dump(const jansson::json_t *json, size_t flags, int depth, jansson::hashtable_t *parents,
                   jansson::json_dump_callback_t callback, void *data) {
    int embed = flags & JSON_EMBED;
    int compact = flags & JSON_COMPACT;
    int escape_unicode = flags & JSON_ESCAPE_UNICODE;
    int ensure_ascii = flags & JSON_ENSURE_ASCII;
    int sort_keys = flags & JSON_SORT_KEYS;
    int precision = FLAGS_TO_PRECISION(flags);

    flags &= ~JSON_EMBED;

    if (!json)
        return -1;

    switch (json_typeof(json)) {
        case JSON_NULL:
            return callback("null", 4, data);

        case JSON_TRUE:
            return callback("true", 4, data);

        case JSON_FALSE:
            return callback("false", 5, data);

        case JSON_INTEGER: {
            char buffer[MAX_INTEGER_STR_LENGTH];
            int size;

            size = std::snprintf(buffer, MAX_INTEGER_STR_LENGTH, "%" JSON_INTEGER_FORMAT,
                               json_integer_value(json));
            if (size < 0 || size >= MAX_INTEGER_STR_LENGTH)
                return -1;
            return callback(buffer, size, data);
        }

        case JSON_REAL: {
            char buffer[MAX_REAL_STR_LENGTH];
            int size;
            double value = json_real_value(json);

            if (jansson_isinf(value) || jansson_isnan(value))
                return callback("null", 4, data);

#if JSON_REAL_IS_LONG_DOUBLE
            size = jsonp_dtostr(buffer, MAX_REAL_STR_LENGTH, value);
#else
            size = jsonp_dtostr(buffer, MAX_REAL_STR_LENGTH, value);
#endif
            if (size < 0)
                return -1;
            return callback(buffer, size, data);
        }

        case JSON_STRING: {
            const char *str = json_string_value(json);
            size_t len = json_string_length(json);
            return callback(str, len, data);
        }

        case JSON_ARRAY: {
            size_t n;
            size_t i;
            int ret;

            /* detect circular references */
            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                size_t loop_key_len;
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                loop_key_len = strlen(loop_key);
                if (jsonp_loop_check(parents, json, loop_key, sizeof(loop_key), &loop_key_len))
                    return -1;
            }

            n = json_array_size(json);
            if (n == 0)
                return callback("[]", 2, data);

            if (callback("[", 1, data))
                return -1;

            if (!compact && callback("\n", 1, data))
                return -1;

            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                if (hashtable_set(parents, loop_key, strlen(loop_key), json))
                    return -1;
            }

            for (i = 0; i < n; ++i) {
                jansson::json_t *value = json_array_get(json, i);

                if (!compact && indent(callback, data, depth + 1, flags))
                    return -1;

                if (do_dump(value, flags, depth + 1, parents, callback, data))
                    return -1;

                if (i < n - 1) {
                    if (callback(",", 1, data) ||
                        (!compact && callback("\n", 1, data)))
                        return -1;
                }
            }

            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                hashtable_del(parents, loop_key, strlen(loop_key));
            }

            if (!compact && indent(callback, data, depth, flags))
                return -1;

            return callback("]", 1, data);
        }

        case JSON_OBJECT: {
            void *iter;
            int ret;

            /* detect circular references */
            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                size_t loop_key_len;
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                loop_key_len = strlen(loop_key);
                if (jsonp_loop_check(parents, json, loop_key, sizeof(loop_key), &loop_key_len))
                    return -1;
            }

            if (json_object_size(json) == 0)
                return callback("{}", 2, data);

            if (callback("{", 1, data))
                return -1;

            if (!compact && callback("\n", 1, data))
                return -1;

            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                if (hashtable_set(parents, loop_key, strlen(loop_key), json))
                    return -1;
            }

            iter = json_object_iter((jansson::json_t *)json);
            while (iter) {
                const char *key = json_object_iter_key(iter);
                jansson::json_t *value = json_object_iter_value(iter);

                if (!compact && indent(callback, data, depth + 1, flags))
                    return -1;

                if (escape_string(key, strlen(key), callback, data))
                    return -1;

                if (callback(":", 1, data))
                    return -1;

                if (!compact && callback(" ", 1, data))
                    return -1;

                if (do_dump(value, flags, depth + 1, parents, callback, data))
                    return -1;

                iter = json_object_iter_next((jansson::json_t *)json, iter);
                if (iter) {
                    if (callback(",", 1, data) ||
                        (!compact && callback("\n", 1, data)))
                        return -1;
                }
            }

            if (parents->size > 0) {
                char loop_key[2 * sizeof(void *) + 1];
                snprintf(loop_key, sizeof(loop_key), "0x%p", json);
                hashtable_del(parents, loop_key, strlen(loop_key));
            }

            if (!compact && indent(callback, data, depth, flags))
                return -1;

            return callback("}", 1, data);
        }

        default:
            /* not reached */
            return -1;
    }
}

int jansson::json_dump_callback(const json_t *json, json_dump_callback_t callback, void *data, size_t flags) {
    hashtable_t parents_set;
    int res;

    hashtable_init(&parents_set);
    res = do_dump(json, flags, 0, &parents_set, callback, data);
    hashtable_close(&parents_set);
    return res;
}

char *jansson::json_dumps(const json_t *json, size_t flags) {
    dump_buffer_t buf = {nullptr, 0, 0};
    int res;

    res = json_dump_callback(json, dump_to_buffer, &buf, flags);
    if (res < 0) {
        jsonp_free(buf.buffer);
        return nullptr;
    }

    if (!buf.buffer) {
        buf.buffer = static_cast<char*>(jsonp_malloc(1));
        if (!buf.buffer)
            return nullptr;
    }

    buf.buffer[buf.size] = '\0';
    return buf.buffer;
}

int jansson::json_dumpf(const json_t *json, FILE *output, size_t flags) {
    return json_dump_callback(json, dump_to_file, output, flags);
}

int jansson::json_dump_file(const json_t *json, const char *path, size_t flags) {
    FILE *output = fopen(path, "wb");
    int res;

    if (!output)
        return -1;

    res = json_dumpf(json, output, flags);
    if (fclose(output) != 0)
        return -1;
    return res;
}
