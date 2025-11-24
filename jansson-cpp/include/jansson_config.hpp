/*
 * Copyright (c) 2010-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 *
 *
 * This file specifies a part of the site-specific configuration for
 * Jansson, namely those things that affect the public API in
 * jansson.h.
 *
 * The CMake system will generate the jansson_config.h file and
 * copy it to the build and install directories.
 */

#ifndef JANSSON_CONFIG_HPP
#define JANSSON_CONFIG_HPP

#include <cstdint>
#include <cstddef>
#include <climits>

namespace jansson {
namespace config {

// Header availability configuration
constexpr bool have_endian_h = true;
constexpr bool have_fcntl_h = true;
constexpr bool have_inttypes_h = true;
constexpr bool have_locale_h = true;
constexpr bool have_memory_h = true;
constexpr bool have_sched_h = true;
constexpr bool have_stdint_h = true;
constexpr bool have_stdlib_h = true;
constexpr bool have_strings_h = true;
constexpr bool have_string_h = true;
constexpr bool have_sys_param_h = true;
constexpr bool have_sys_stat_h = true;
constexpr bool have_sys_time_h = true;
constexpr bool have_sys_types_h = true;
constexpr bool have_unistd_h = true;

// Function availability configuration
constexpr bool have_close = true;
constexpr bool have_getpid = true;
constexpr bool have_gettimeofday = true;
constexpr bool have_open = true;
constexpr bool have_read = true;
constexpr bool have_sched_yield = true;
constexpr bool have_setlocale = true;
constexpr bool have_strtoq = true;
constexpr bool have_strtoll = true;

// Atomic operations support
constexpr bool have_atomic_builtins = true;
constexpr bool have_sync_builtins = true;
constexpr bool have_stdatomic_h = true;

// Type sizes
constexpr int sizeof_int = 4;
constexpr int sizeof_int32_t = 4;
constexpr int sizeof_int64_t = 8;
constexpr int sizeof_long = 8;
constexpr int sizeof_long_long = 8;
constexpr int sizeof_short = 2;
constexpr int sizeof_size_t = 8;
constexpr int sizeof_ssize_t = 8;
constexpr int sizeof_uint16_t = 2;
constexpr int sizeof_uint32_t = 4;
constexpr int sizeof_uint8_t = 1;

// Package information
constexpr const char* package_name = "jansson";
constexpr const char* package_version = "2.14";
constexpr const char* package_string = "jansson 2.14";
constexpr const char* version = "2.14";

// Thread safety configuration
constexpr bool thread_safe_refcount = have_atomic_builtins || have_sync_builtins;

} // namespace config
} // namespace jansson

// Compatibility macros for backward compatibility
// These are kept for existing code that uses the macro names
#define HAVE_ENDIAN_H 1
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_LOCALE_H 1
#define HAVE_MEMORY_H 1
#define HAVE_SCHED_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1

#define HAVE_CLOSE 1
#define HAVE_GETPID 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_OPEN 1
#define HAVE_READ 1
#define HAVE_SCHED_YIELD 1
#define HAVE_SETLOCALE 1
#define HAVE_STRTOQ 1
#define HAVE_STRTOLL 1

#define HAVE_ATOMIC_BUILTINS 1
#define HAVE_SYNC_BUILTINS 1
#define HAVE_STDATOMIC_H 1

#define SIZEOF_INT 4
#define SIZEOF_INT32_T 4
#define SIZEOF_INT64_T 8
#define SIZEOF_LONG 8
#define SIZEOF_LONG_LONG 8
#define SIZEOF_SHORT 2
#define SIZEOF_SIZE_T 8
#define SIZEOF_SSIZE_T 8
#define SIZEOF_UINT16_T 2
#define SIZEOF_UINT32_T 4
#define SIZEOF_UINT8_T 1

#define PACKAGE_NAME "jansson"
#define PACKAGE_VERSION "2.14"
#define PACKAGE_STRING "jansson 2.14"
#define VERSION "2.14"

#define STDC_HEADERS 1
#define LT_OBJDIR ".libs/"

#endif /* JANSSON_CONFIG_HPP */
