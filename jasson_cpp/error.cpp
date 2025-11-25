#include "error.h"

namespace jansson {

void throwJsonException(const JsonError& error) {
    switch (error.code()) {
        case JsonErrorCode::OUT_OF_MEMORY:
            throw JsonOutOfMemoryError(error);
        case JsonErrorCode::STACK_OVERFLOW:
            throw JsonStackOverflowError(error);
        case JsonErrorCode::CANNOT_OPEN_FILE:
            throw JsonCannotOpenFileError(error);
        case JsonErrorCode::INVALID_ARGUMENT:
            throw JsonInvalidArgumentError(error);
        case JsonErrorCode::INVALID_UTF8:
            throw JsonInvalidUtf8Error(error);
        case JsonErrorCode::PREMATURE_END_OF_INPUT:
            throw JsonPrematureEndOfInputError(error);
        case JsonErrorCode::END_OF_INPUT_EXPECTED:
            throw JsonEndOfInputExpectedError(error);
        case JsonErrorCode::INVALID_SYNTAX:
            throw JsonInvalidSyntaxError(error);
        case JsonErrorCode::INVALID_FORMAT:
            throw JsonInvalidFormatError(error);
        case JsonErrorCode::WRONG_TYPE:
            throw JsonWrongTypeError(error);
        case JsonErrorCode::NULL_CHARACTER:
            throw JsonNullCharacterError(error);
        case JsonErrorCode::NULL_VALUE:
            throw JsonNullValueError(error);
        case JsonErrorCode::NULL_BYTE_IN_KEY:
            throw JsonNullByteInKeyError(error);
        case JsonErrorCode::DUPLICATE_KEY:
            throw JsonDuplicateKeyError(error);
        case JsonErrorCode::NUMERIC_OVERFLOW:
            throw JsonNumericOverflowError(error);
        case JsonErrorCode::ITEM_NOT_FOUND:
            throw JsonItemNotFoundError(error);
        case JsonErrorCode::INDEX_OUT_OF_RANGE:
            throw JsonIndexOutOfRangeError(error);
        default:
            throw JsonException(error);
    }
}

} // namespace jansson
