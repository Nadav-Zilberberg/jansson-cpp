#ifndef JANSSON_ERROR_H
#define JANSSON_ERROR_H

#include "src/jansson.h"
#include <stdexcept>
#include <string>
#include <cstddef>

namespace jansson {

enum class JsonErrorCode {
    UNKNOWN,
    OUT_OF_MEMORY,
    STACK_OVERFLOW,
    CANNOT_OPEN_FILE,
    INVALID_ARGUMENT,
    INVALID_UTF8,
    PREMATURE_END_OF_INPUT,
    END_OF_INPUT_EXPECTED,
    INVALID_SYNTAX,
    INVALID_FORMAT,
    WRONG_TYPE,
    NULL_CHARACTER,
    NULL_VALUE,
    NULL_BYTE_IN_KEY,
    DUPLICATE_KEY,
    NUMERIC_OVERFLOW,
    ITEM_NOT_FOUND,
    INDEX_OUT_OF_RANGE
};

class JsonError {
public:
    JsonError(JsonErrorCode code, const std::string& message,
              const std::string& source, int line, int column, size_t position)
        : code_(code), message_(message), source_(source),
          line_(line), column_(column), position_(position) {}

    JsonErrorCode code() const { return code_; }
    const std::string& message() const { return message_; }
    const std::string& source() const { return source_; }
    int line() const { return line_; }
    int column() const { return column_; }
    size_t position() const { return position_; }

private:
    JsonErrorCode code_;
    std::string message_;
    std::string source_;
    int line_;
    int column_;
    size_t position_;
};

class JsonException : public std::runtime_error {
public:
    JsonException(const JsonError& error)
        : std::runtime_error(error.message()), error_(error) {}

    const JsonError& error() const { return error_; }

private:
    JsonError error_;
};

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

void throwJsonException(const JsonError& error);

} // namespace jansson

#endif // JANSSON_ERROR_H
