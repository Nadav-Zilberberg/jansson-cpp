/**
 * Comprehensive JSON Library Example
 * 
 * This example demonstrates all major features of the modern C++ JSON library
 * including parsing, creation, modification, and serialization of JSON data.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"
#include "src/json_convenience.hpp"

void demonstrateBasicValueCreation() {
    std::cout << "=== Basic JSON Value Creation ===" << std::endl;
    
    // Create different types of JSON values
    json::Value nullValue;
    json::Value boolValue(true);
    json::Value intValue(42);
    json::Value doubleValue(3.14159);
    json::Value stringValue("Hello, JSON!");
    
    std::cout << "Null value: " << json::dump(nullValue) << std::endl;
    std::cout << "Boolean value: " << json::dump(boolValue) << std::endl;
    std::cout << "Integer value: " << json::dump(intValue) << std::endl;
    std::cout << "Double value: " << json::dump(doubleValue) << std::endl;
    std::cout << "String value: " << json::dump(stringValue) << std::endl;
    std::cout << std::endl;
}

void demonstrateParsing() {
    std::cout << "=== JSON Parsing ===" << std::endl;
    
    // Parse a simple JSON string
    std::string jsonString = R"({"name": "John", "age": 30, "city": "New York"})";
    
    try {
        json::Value parsed = json::parse(jsonString);
        std::cout << "Successfully parsed: " << json::dump(parsed) << std::endl;
        
        // Access parsed values
        std::string name = parsed["name"].asString();
        int age = parsed["age"].asInt();
        std::string city = parsed["city"].asString();
        
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "City: " << city << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateSerialization() {
    std::cout << "=== JSON Serialization ===" << std::endl;
    
    // Create a complex JSON structure
    json::Value person;
    person["name"] = "Alice";
    person["age"] = 25;
    person["skills"] = json::Array();
    person["skills"].push_back("C++");
    person["skills"].push_back("Python");
    person["skills"].push_back("JavaScript");
    
    // Serialize with pretty printing
    std::cout << "Pretty printed:" << std::endl;
    std::cout << json::dump(person, json::Format::Pretty) << std::endl;
    
    // Serialize compact
    std::cout << "Compact:" << std::endl;
    std::cout << json::dump(person, json::Format::Compact) << std::endl;
    std::cout << std::endl;
}

void demonstrateErrorHandling() {
    std::cout << "=== Error Handling ===" << std::endl;
    
    // Try to parse invalid JSON
    std::string invalidJson = R"({"name": "test", "value": })";
    
    try {
        json::Value result = json::parse(invalidJson);
        std::cout << "This shouldn't print" << std::endl;
    } catch (const json::ParseError& e) {
        std::cout << "Caught ParseError: " << e.what() << std::endl;
        std::cout << "Error at position: " << e.position() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateNestedStructures() {
    std::cout << "=== Nested JSON Structures ===" << std::endl;
    
    // Create a complex nested structure
    json::Value company;
    company["name"] = "Tech Corp";
    company["employees"] = json::Array();
    
    json::Value employee1;
    employee1["name"] = "Bob";
    employee1["department"] = "Engineering";
    employee1["salary"] = 75000;
    
    json::Value employee2;
    employee2["name"] = "Carol";
    employee2["department"] = "Marketing";
    employee2["salary"] = 65000;
    
    company["employees"].push_back(employee1);
    company["employees"].push_back(employee2);
    
    // Access nested data
    std::cout << "Company: " << company["name"].asString() << std::endl;
    std::cout << "Number of employees: " << company["employees"].size() << std::endl;
    
    for (const auto& emp : company["employees"]) {
        std::cout << "  - " << emp["name"].asString() 
                  << " (" << emp["department"].asString() << ")"
                  << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateTypeConversions() {
    std::cout << "=== Type Conversions ===" << std::endl;
    
    // Create JSON from C++ types
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::map<std::string, std::string> data = {
        {"key1", "value1"},
        {"key2", "value2"}
    };
    
    json::Value jsonNumbers = json::fromVector(numbers);
    json::Value jsonData = json::fromMap(data);
    
    std::cout << "Numbers array: " << json::dump(jsonNumbers) << std::endl;
    std::cout << "String map: " << json::dump(jsonData) << std::endl;
    
    // Convert back to C++ types
    std::vector<int> extractedNumbers = jsonNumbers.toVector<int>();
    std::cout << "Extracted numbers: ";
    for (int num : extractedNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl << std::endl;
}

void demonstrateArrayOperations() {
    std::cout << "=== JSON Array Operations ===" << std::endl;
    
    // Create and manipulate arrays
    json::Value fruits = json::Array();
    fruits.push_back("apple");
    fruits.push_back("banana");
    fruits.push_back("orange");
    
    std::cout << "Original array: " << json::dump(fruits) << std::endl;
    std::cout << "Array size: " << fruits.size() << std::endl;
    std::cout << "First element: " << fruits[0].asString() << std::endl;
    
    // Modify array
    fruits[1] = "grape";
    fruits.erase(2); // Remove orange
    
    std::cout << "Modified array: " << json::dump(fruits) << std::endl;
    std::cout << std::endl;
}

void demonstrateObjectOperations() {
    std::cout << "=== JSON Object Operations ===" << std::endl;
    
    // Create and manipulate objects
    json::Value config;
    config["debug"] = true;
    config["port"] = 8080;
    config["host"] = "localhost";
    
    std::cout << "Original object: " << json::dump(config) << std::endl;
    std::cout << "Has debug key: " << (config.has("debug") ? "yes" : "no") << std::endl;
    std::cout << "Port value: " << config["port"].asInt() << std::endl;
    
    // Modify object
    config["port"] = 9090;
    config.remove("host");
    config["timeout"] = 30;
    
    std::cout << "Modified object: " << json::dump(config) << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "Modern C++ JSON Library - Comprehensive Example" << std::endl;
    std::cout << "=================================================" << std::endl << std::endl;
    
    try {
        demonstrateBasicValueCreation();
        demonstrateParsing();
        demonstrateSerialization();
        demonstrateErrorHandling();
        demonstrateNestedStructures();
        demonstrateTypeConversions();
        demonstrateArrayOperations();
        demonstrateObjectOperations();
        
        std::cout << "All examples completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
