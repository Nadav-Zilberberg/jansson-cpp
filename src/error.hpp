#ifndef JASSON_ERROR_HPP
#define JASSON_ERROR_HPP

#include "jansson.h"
#include <string>
#include <cstdarg>
#include <cstring>
#include <stdexcept>

namespace jansson {

// Forward declarations
class JsonError;
class JsonException;

// Error code enum - using unique values
enum class JsonErrorCode {
    OUT_OF_MEMORY = 100,
    STACK_OVERFLOW = 101,
    CANNOT_OPEN_FILE = 102,
    INVALID_ARGUMENT = 103,
    INVALID_UTF8 = 104,
    PREMATURE_END_OF_INPUT = 105,
    END_OF_INPUT_EXPECTED = 106,
    INVALID_SYNTAX = 107,
    INVALID_FORMAT = 108,
    WRONG_TYPE = 109,
    NULL_CHARACTER = 110,
    NULL_VALUE = 111,
    NULL_BYTE_IN_KEY = 112,
    DUPLICATE_KEY = 113,
    NUMERIC_OVERFLOW = 114,
    ITEM_NOT_FOUND = 115,
    INDEX_OUT_OF_RANGE = 116
};

// Base error class
class JsonError {
public:
    JsonError() : code_(JsonErrorCode::INVALID_FORMAT) {
        error_.line = -1;
        error_.column = -1;
        error_.position = -1;
        error_.source[0] = '\0';
        error_.text[0] = '\0';
    }

    void set(const char* source, JsonErrorCode code, const char* format, ...) {
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

    JsonErrorCode code() const { return code_; }
    const char* text() const { return error_.text; }
    const char* source() const { return error_.source; }
    int line() const { return error_.line; }
    int column() const { return error_.column; }
    int position() const { return error_.position; }

    const json_error_t* get() const { return &error_; }

private:
    json_error_t error_;
    JsonErrorCode code_;
};

// Base exception class
class JsonException : public std::runtime_error {
public:
    JsonException(const JsonError& error) 
        : std::runtime_error(error.text()), error_(error) {}
    
    const JsonError& error() const { return error_; }

private:
    JsonError error_;
};

// Specific exception classes
class JsonOutOfMemoryError : public JsonException {
public:
    JsonOutOfMemoryError(const JsonError& error) : JsonException(error) {}
};

class JsonStackOverflowError : public JsonException {
public:
    JsonStackOverflowError(const JsonError& error) : JsonException(error) {}
};

class JsonCannotOpenFileError : public JsonException {
public:
    JsonCannotOpenFileError(const JsonError& error) : JsonException(error) {}
};

class JsonInvalidArgumentError : public JsonException {
public:
    JsonInvalidArgumentError(const JsonError& error) : JsonException(error) {}
};

class JsonInvalidUtf8Error : public JsonException {
public:
    JsonInvalidUtf8Error(const JsonError& error) : JsonException(error) {}
};

class JsonPrematureEndOfInputError : public JsonException {
public:
    JsonPrematureEndOfInputError(const JsonError& error) : JsonException(error) {}
};

class JsonEndOfInputExpectedError : public JsonException {
public:
    JsonEndOfInputExpectedError(const JsonError& error) : JsonException(error) {}
};

class JsonInvalidSyntaxError : public JsonException {
public:
    JsonInvalidSyntaxError(const JsonError& error) : JsonException(error) {}
};

class JsonInvalidFormatError : public JsonException {
public:
    JsonInvalidFormatError(const JsonError& error) : JsonException(error) {}
};

class JsonWrongTypeError : public JsonException {
public:
    JsonWrongTypeError(const JsonError& error) : JsonException(error) {}
};

class JsonNullCharacterError : public JsonException {
public:
    JsonNullCharacterError(const JsonError& error) : JsonException(error) {}
};

class JsonNullValueError : public JsonException {
public:
    JsonNullValueError(const JsonError& error) : JsonException(error) {}
};

class JsonNullByteInKeyError : public JsonException {
public:
    JsonNullByteInKeyError(const JsonError& error) : JsonException(error) {}
};

class JsonDuplicateKeyError : public JsonException {
public:
    JsonDuplicateKeyError(const JsonError& error) : JsonException(error) {}
};

class JsonNumericOverflowError : public JsonException {
public:
    JsonNumericOverflowError(const JsonError& error) : JsonException(error) {}
};

class JsonItemNotFoundError : public JsonException {
public:
    JsonItemNotFoundError(const JsonError& error) : JsonException(error) {}
};

class JsonIndexOutOfRangeError : public JsonException {
public:
    JsonIndexOutOfRangeError(const JsonError& error) : JsonException(error) {}
};

// Function to throw appropriate exception based on error code
void throwJsonException(const JsonError& error);

} // namespace jansson

#endif // JASSON_ERROR_HPP
