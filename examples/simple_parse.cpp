/**
 * Simple JSON Parsing Example
 * 
 * This is a minimal example showing how to parse JSON strings
 * and handle basic errors. Perfect for beginners!
 */

#include <iostream>
#include <string>
#include "src/jasson.hpp"
#include "src/json_value.hpp"

int main() {
    std::cout << "Simple JSON Parsing Example" << std::endl;
    std::cout << "===========================" << std::endl << std::endl;
    
    // Simple JSON string to parse
    std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "email": "john@example.com"
    })";
    
    try {
        // Parse the JSON string
        json::Value person = json::parse(jsonString);
        
        std::cout << "Successfully parsed JSON!" << std::endl;
        std::cout << "Parsed JSON: " << json::dump(person) << std::endl << std::endl;
        
        // Access individual fields
        std::cout << "Name: " << person["name"].asString() << std::endl;
        std::cout << "Age: " << person["age"].asInt() << std::endl;
        std::cout << "Email: " << person["email"].asString() << std::endl;
        
    } catch (const json::ParseError& e) {
        std::cout << "JSON parsing failed: " << e.what() << std::endl;
        std::cout << "Error at position: " << e.position() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl << "Example completed successfully!" << std::endl;
    return 0;
}
