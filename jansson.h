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

#ifndef JANSSON_H
#define JANSSON_H

#include <stddef.h>
#include <stdarg.h>
#include "jansson-cpp/include/jansson.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/* Version */
#define JANSSON_MAJOR_VERSION  2
#define JANSSON_MINOR_VERSION  14
#define JANSSON_MICRO_VERSION  0

/* Version functions */
const char *jansson_version_str(void);
int jansson_version_cmp(int major, int minor, int micro);

#ifdef __cplusplus
}
#endif

#endif /* JANSSON_H */
