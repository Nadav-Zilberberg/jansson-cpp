#include "src/error.h"
#include <iostream>

int main() {
    try {
        jansson::JsonError error(jansson::JsonErrorCode::INVALID_SYNTAX,
                                "Test error: invalid syntax",
                                "test.cpp", 1, 2, 3);
        jansson::throwJsonException(error);
    } catch (const jansson::JsonInvalidSyntaxError& e) {
        std::cout << "Caught JsonInvalidSyntaxError: " << e.what() << std::endl;
        std::cout << "Error source: " << e.error().source() << std::endl;
        std::cout << "Error line: " << e.error().line() << std::endl;
        std::cout << "Error column: " << e.error().column() << std::endl;
        std::cout << "Error position: " << e.error().position() << std::endl;
        std::cout << "Error code: " << static_cast<int>(e.error().code()) << std::endl;
    } catch (const jansson::JsonException& e) {
        std::cout << "Caught JsonException: " << e.what() << std::endl;
    }

    return 0;
}