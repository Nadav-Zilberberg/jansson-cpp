/**
 * Type Conversion Example
 * 
 * This example demonstrates converting between JSON types
 * and C++ types, including handling of type safety.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"

void demonstrateBasicConversions() {
    std::cout << "1. Basic Type Conversions:" << std::endl;
    
    // Create JSON values from C++ primitives
    json::Value jsonInt(42);
    json::Value jsonDouble(3.14159);
    json::Value jsonString("Hello, JSON!");
    json::Value jsonBool(true);
    
    // Convert back to C++ types
    int cppInt = jsonInt.asInt();
    double cppDouble = jsonDouble.asDouble();
    std::string cppString = jsonString.asString();
    bool cppBool = jsonBool.asBool();
    
    std::cout << "  JSON int -> C++ int: " << cppInt << std::endl;
    std::cout << "  JSON double -> C++ double: " << cppDouble << std::endl;
    std::cout << "  JSON string -> C++ string: " << cppString << std::endl;
    std::cout << "  JSON bool -> C++ bool: " << (cppBool ? "true" : "false") << std::endl;
    std::cout << std::endl;
}

void demonstrateContainerConversions() {
    std::cout << "2. Container Conversions:" << std::endl;
    
    // Vector to JSON array
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    json::Value jsonNumbers = json::fromVector(numbers);
    std::cout << "  Vector to JSON: " << json::dump(jsonNumbers) << std::endl;
    
    // JSON array to vector
    std::vector<int> extractedNumbers = jsonNumbers.toVector<int>();
    std::cout << "  JSON to vector: ";
    for (int num : extractedNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Map to JSON object
    std::map<std::string, std::string> capitals = {
        {"USA", "Washington D.C."},
        {"UK", "London"},
        {"France", "Paris"}
    };
    json::Value jsonCapitals = json::fromMap(capitals);
    std::cout << "  Map to JSON: " << json::dump(jsonCapitals, json::Format::Pretty) << std::endl;
    
    // JSON object to map
    std::map<std::string, std::string> extractedCapitals = jsonCapitals.toMap<std::string>();
    std::cout << "  JSON to map:" << std::endl;
    for (const auto& pair : extractedCapitals) {
        std::cout << "    " << pair.first << " -> " << pair.second << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateComplexConversions() {
    std::cout << "3. Complex Type Conversions:" << std::endl;
    
    // Create a complex JSON structure
    json::Value person;
    person["name"] = "Alice";
    person["age"] = 30;
    person["scores"] = json::Array();
    person["scores"].push_back(95);
    person["scores"].push_back(87);
    person["scores"].push_back(92);
    
    // Convert to a C++ struct (conceptual example)
    struct Person {
        std::string name;
        int age;
        std::vector<int> scores;
    };
    
    Person alice;
    alice.name = person["name"].asString();
    alice.age = person["age"].asInt();
    alice.scores = person["scores"].toVector<int>();
    
    std::cout << "  Converted to struct: " << alice.name << ", age " << alice.age << std::endl;
    std::cout << "  Scores: ";
    for (int score : alice.scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl << std::endl;
}

void demonstrateTypeSafety() {
    std::cout << "4. Type Safety Demonstrations:" << std::endl;
    
    json::Value value;
    value["string_value"] = "hello";
    value["number_value"] = 42;
    value["array_value"] = json::Array();
    value["array_value"].push_back(1);
    value["array_value"].push_back(2);
    
    // Safe type checking
    if (value["string_value"].isString()) {
        std::string str = value["string_value"].asString();
        std::cout << "  Safe string conversion: " << str << std::endl;
    }
    
    if (value["number_value"].isInt()) {
        int num = value["number_value"].asInt();
        std::cout << "  Safe int conversion: " << num << std::endl;
    }
    
    if (value["array_value"].isArray()) {
        std::cout << "  Safe array access, size: " << value["array_value"].size() << std::endl;
    }
    
    // Type checking before conversion
    json::Value mixedArray = json::Array();
    mixedArray.push_back(1);
    mixedArray.push_back("string");
    mixedArray.push_back(true);
    
    std::cout << "  Mixed array elements:" << std::endl;
    for (size_t i = 0; i < mixedArray.size(); ++i) {
        const json::Value& elem = mixedArray[i];
        std::cout << "    Element " << i << ": ";
        if (elem.isInt()) {
            std::cout << "int(" << elem.asInt() << ")" << std::endl;
        } else if (elem.isString()) {
            std::cout << "string(\"" << elem.asString() << "\")" << std::endl;
        } else if (elem.isBool()) {
            std::cout << "bool(" << (elem.asBool() ? "true" : "false") << ")" << std::endl;
        }
    }
    std::cout << std::endl;
}

void demonstrateConversionErrors() {
    std::cout << "5. Conversion Error Handling:" << std::endl;
    
    json::Value data;
    data["string"] = "not a number";
    data["number"] = 42;
    
    // Attempt invalid conversion
    try {
        int invalid = data["string"].asInt();
        std::cout << "  This shouldn't print: " << invalid << std::endl;
    } catch (const json::TypeError& e) {
        std::cout << "  Caught TypeError: " << e.what() << std::endl;
    }
    
    // Safe conversion with fallback
    int value = 0;
    if (data["string"].isInt()) {
        value = data["string"].asInt();
    } else {
        std::cout << "  Using fallback value for string-to-int conversion" << std::endl;
        value = -1; // fallback
    }
    std::cout << "  Fallback result: " << value << std::endl;
    
    // Valid conversion
    if (data["number"].isInt()) {
        int validInt = data["number"].asInt();
        std::cout << "  Valid int conversion: " << validInt << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateCustomConversions() {
    std::cout << "6. Custom Conversion Functions:" << std::endl;
    
    // Create JSON from various container types
    std::list<std::string> colors = {"red", "green", "blue"};
    json::Value jsonColors = json::Array();
    for (const auto& color : colors) {
        jsonColors.push_back(color);
    }
    
    std::cout << "  List to JSON: " << json::dump(jsonColors) << std::endl;
    
    // Convert back to different container type
    std::vector<std::string> colorVector;
    for (size_t i = 0; i < jsonColors.size(); ++i) {
        colorVector.push_back(jsonColors[i].asString());
    }
    
    std::cout << "  JSON to vector: ";
    for (const auto& color : colorVector) {
        std::cout << color << " ";
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::cout << "JSON Library Type Conversion Example" << std::endl;
    std::cout << "=====================================" << std::endl << std::endl;
    
    try {
        demonstrateBasicConversions();
        demonstrateContainerConversions();
        demonstrateComplexConversions();
        demonstrateTypeSafety();
        demonstrateConversionErrors();
        demonstrateCustomConversions();
        
        std::cout << "Type conversion example completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
