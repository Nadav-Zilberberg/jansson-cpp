#include "src/jansson_private.h"
#include <iostream>

int main() {
    json_error_t error;
    jsonp_error_init(&error, "test.cpp");
    jsonp_error_set(&error, 1, 2, 3, json_error_invalid_syntax, "Test error: %s", "invalid syntax");
    
    std::cout << "Error source: " << error.source << std::endl;
    std::cout << "Error line: " << error.line << std::endl;
    std::cout << "Error column: " << error.column << std::endl;
    std::cout << "Error position: " << error.position << std::endl;
    std::cout << "Error text: " << error.text << std::endl;
    std::cout << "Error code: " << json_error_code(&error) << std::endl;
    
    return 0;
}
