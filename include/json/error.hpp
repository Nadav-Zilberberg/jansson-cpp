#ifndef JASSON_ERROR_HPP
#define JASSON_ERROR_HPP

#include "jansson.h"
#include <string>
#include <cstdarg>
#include <cstring>

namespace jasson {

class Error {
public:
    Error() : code_(JSON_ERROR_NONE) {
        error_.line = -1;
        error_.column = -1;
        error_.position = -1;
        error_.source[0] = '\0';
        error_.text[0] = '\0';
    }

    void set(const char* source, json_error_code_t code, const char* format, ...) {
        code_ = code;
        
        if (source) {
            strncpy(error_.source, source, sizeof(error_.source) - 1);
            error_.source[sizeof(error_.source) - 1] = '\0';
        }

        va_list args;
        va_start(args, format);
        vsnprintf(error_.text, sizeof(error_.text), format, args);
        va_end(args);
        error_.text[sizeof(error_.text) - 1] = '\0';
    }

    json_error_code_t code() const { return code_; }
    const char* text() const { return error_.text; }
    const char* source() const { return error_.source; }
    int line() const { return error_.line; }
    int column() const { return error_.column; }
    int position() const { return error_.position; }

    const json_error_t* get() const { return &error_; }

private:
    json_error_t error_;
    json_error_code_t code_;
};

} // namespace jasson

#endif // JASSON_ERROR_HPP
