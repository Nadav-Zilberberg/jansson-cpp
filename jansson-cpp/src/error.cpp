#include "jansson.hpp"
#include <cstring>
#include <string>
#include <cstdarg>
#include <cstdio>

// Forward declare the functions that are expected by other files
extern "C" {

// Forward declarations
void jsonp_error_set_source(jansson::json_error_t *error, const char *source);
void jsonp_error_vset(jansson::json_error_t *error, int line, int column, size_t position,
                      jansson::json_error_code code, const char *msg, va_list ap);

void jsonp_error_init(jansson::json_error_t *error, const char *source) {
    if (!error) {
        return;
    }
    
    error->text[0] = '\0';
    error->line = -1;
    error->column = -1;
    error->position = 0;
    
    if (source) {
        jsonp_error_set_source(error, source);
    } else {
        error->source[0] = '\0';
    }
}

void jsonp_error_set_source(jansson::json_error_t *error, const char *source) {
    if (!error || !source) {
        return;
    }
    
    std::string source_str(source);
    
    if (source_str.length() < jansson::JSON_ERROR_SOURCE_LENGTH) {
        std::strncpy(error->source, source, source_str.length() + 1);
        error->source[source_str.length()] = '\0';
    } else {
        size_t extra = source_str.length() - jansson::JSON_ERROR_SOURCE_LENGTH + 4;
        std::memcpy(error->source, "...", 3);
        std::strncpy(error->source + 3, source + extra, source_str.length() - extra + 1);
        error->source[jansson::JSON_ERROR_SOURCE_LENGTH - 1] = '\0';
    }
}

void jsonp_error_set(jansson::json_error_t *error, int line, int column, size_t position,
                     jansson::json_error_code code, const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    jsonp_error_vset(error, line, column, position, code, msg, ap);
    va_end(ap);
}

void jsonp_error_vset(jansson::json_error_t *error, int line, int column, size_t position,
                      jansson::json_error_code code, const char *msg, va_list ap) {
    if (!error) {
        return;
    }
    
    // Error already set
    if (error->text[0] != '\0') {
        return;
    }
    
    error->line = line;
    error->column = column;
    error->position = static_cast<int>(position);
    
    // Use vsnprintf for safe string formatting
    std::vsnprintf(error->text, jansson::JSON_ERROR_TEXT_LENGTH - 1, msg, ap);
    
    // Ensure null termination
    error->text[jansson::JSON_ERROR_TEXT_LENGTH - 2] = '\0';
    
    // Store the error code in the last byte
    error->text[jansson::JSON_ERROR_TEXT_LENGTH - 1] = static_cast<char>(code);
}

} // extern "C"
