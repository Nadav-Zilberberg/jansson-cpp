#include "jansson-cpp/include/jansson.hpp"
#include <iostream>
#include <string>

using namespace jansson;

// Simple test to verify the load.cpp modernization
int main() {
    std::cout << "Testing modernized load.cpp..." << std::endl;
    
    // Test basic JSON parsing
    std::string json_str = R"({"test": "value"})";
    
    json_error_t error;
    json_t* json = json_loads(json_str, 0, &error);
    
    if (!json) {
        std::cerr << "Failed to parse JSON: " << error.text << std::endl;
        return 1;
    }
    
    std::cout << "JSON parsing successful!" << std::endl;
    
    // Clean up
    json_decref(json);
    
    return 0;
}
