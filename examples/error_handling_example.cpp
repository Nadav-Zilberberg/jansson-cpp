/**
 * Error Handling Example
 * 
 * This example demonstrates proper error handling techniques
 * when working with the JSON library.
 */

#include <iostream>
#include <string>
#include "src/jasson.hpp"
#include "src/json_value.hpp"

void demonstrateParseErrorHandling() {
    std::cout << "1. Parse Error Handling:" << std::endl;
    
    // Invalid JSON strings
    std::vector<std::string> invalidJsonStrings = {
        R"({"name": "test", "value": })",  // Missing value
        R"({"name": "test", "value": 123)",  // Missing closing brace
        R"({"name": "test", "value": 123,})",  // Trailing comma
        R"({"name": "test", "value": "unclosed string)",  // Unclosed string
        "invalid json text"  // Completely invalid
    };
    
    for (const auto& jsonString : invalidJsonStrings) {
        try {
            json::Value result = json::parse(jsonString);
            std::cout << "  Unexpectedly parsed: " << jsonString << std::endl;
        } catch (const json::ParseError& e) {
            std::cout << "  Parse error for '" << jsonString << "': " << e.what() << std::endl;
            std::cout << "  Error position: " << e.position() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  General error: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
}

void demonstrateTypeErrorHandling() {
    std::cout << "2. Type Error Handling:" << std::endl;
    
    // Create a JSON value
    json::Value data;
    data["name"] = "John";
    data["age"] = 30;
    data["active"] = true;
    data["scores"] = json::Array();
    data["scores"].push_back(95);
    data["scores"].push_back(87);
    
    // Try to access with wrong types
    try {
        // This should work
        std::string name = data["name"].asString();
        std::cout << "  Name: " << name << std::endl;
        
        // This should fail
        int nameAsInt = data["name"].asInt();
        std::cout << "  This shouldn't print" << std::endl;
    } catch (const json::TypeError& e) {
        std::cout << "  Type error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  General error: " << e.what() << std::endl;
    }
    
    // Try to access non-existent key
    try {
        std::string missing = data["missing_key"].asString();
        std::cout << "  This shouldn't print" << std::endl;
    } catch (const json::KeyError& e) {
        std::cout << "  Key error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  General error: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateArrayErrorHandling() {
    std::cout << "3. Array Error Handling:" << std::endl;
    
    json::Value arr = json::Array();
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    
    try {
        // Valid access
        std::cout << "  arr[1] = " << arr[1].asInt() << std::endl;
        
        // Invalid index
        std::cout << "  arr[10] = " << arr[10].asInt() << std::endl;
    } catch (const json::IndexError& e) {
        std::cout << "  Index error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  General error: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateSafeAccessPatterns() {
    std::cout << "4. Safe Access Patterns:" << std::endl;
    
    json::Value data;
    data["name"] = "Alice";
    data["age"] = 25;
    data["email"] = "alice@example.com";
    
    // Safe access with type checking
    if (data.has("name") && data["name"].isString()) {
        std::string name = data["name"].asString();
        std::cout << "  Safe access - Name: " << name << std::endl;
    }
    
    // Safe access with default values
    std::string phone = data.has("phone") && data["phone"].isString() 
                        ? data["phone"].asString() 
                        : "N/A";
    std::cout << "  Safe access - Phone: " << phone << std::endl;
    
    // Safe numeric conversion
    int age = 0;
    if (data.has("age") && (data["age"].isInt() || data["age"].isDouble())) {
        age = data["age"].isInt() ? data["age"].asInt() : static_cast<int>(data["age"].asDouble());
        std::cout << "  Safe access - Age: " << age << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateErrorRecovery() {
    std::cout << "5. Error Recovery:" << std::endl;
    
    // Function that tries to parse JSON and provides fallback
    auto parseWithFallback = [](const std::string& jsonStr, const json::Value& fallback) -> json::Value {
        try {
            return json::parse(jsonStr);
        } catch (const json::ParseError& e) {
            std::cout << "  Parse failed, using fallback: " << e.what() << std::endl;
            return fallback;
        }
    };
    
    // Try invalid JSON
    json::Value fallback;
    fallback["status"] = "error";
    fallback["message"] = "Invalid JSON provided";
    
    json::Value result = parseWithFallback("invalid json", fallback);
    std::cout << "  Result: " << json::dump(result) << std::endl;
    
    // Try valid JSON
    json::Value valid = parseWithFallback(R"({"status": "success", "data": 123})", fallback);
    std::cout << "  Valid result: " << json::dump(valid) << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "JSON Library Error Handling Example" << std::endl;
    std::cout << "====================================" << std::endl << std::endl;
    
    try {
        demonstrateParseErrorHandling();
        demonstrateTypeErrorHandling();
        demonstrateArrayErrorHandling();
        demonstrateSafeAccessPatterns();
        demonstrateErrorRecovery();
        
        std::cout << "Error handling example completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
