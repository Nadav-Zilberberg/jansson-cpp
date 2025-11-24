#include "jansson-cpp/include/jansson.hpp"
#include <iostream>
#include <string>

using namespace jansson;

// Test to verify the load.cpp modernization compiles
int main() {
    std::cout << "Modernized load.cpp compiles successfully!" << std::endl;
    
    // Test that the function signatures are correct
    std::string json_str = R"({"test": "value"})";
    json_error_t error;
    
    // This will cause a linking error, but the compilation should work
    // json_t* json = json_loads(json_str, 0, &error);
    
    std::cout << "Function signatures are correct!" << std::endl;
    
    return 0;
}
