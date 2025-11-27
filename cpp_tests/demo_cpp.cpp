#include "jasson_cpp/jasson.hpp"
#include <iostream>
#include <vector>

using namespace jasson;

int main() {
    std::cout << "=== Modern C++ JSON API Demo ===" << std::endl;
    
    // 1. Fluent API for JSON construction
    std::cout << "\n1. Fluent API Builder:" << std::endl;
    auto person = json_builder()
        .object()
        .add("name", "Alice")
        .add("age", static_cast<int64_t>(25))
        .add("active", true)
        .add("skills", json_builder()
            .array()
            .append("C++")
            .append("Python")
            .append("JavaScript")
            .build())
        .build();
    
    std::cout << "Created person object with fluent API" << std::endl;
    
    // 2. Operator overloading for intuitive syntax
    std::cout << "\n2. Literal Operators:" << std::endl;
    auto greeting = "Hello, World!"_json;
    auto number = 42_json;
    auto pi = 3.14159_json;
    
    std::cout << "String literal: " << greeting->value() << std::endl;
    std::cout << "Integer literal: " << number->value() << std::endl;
    std::cout << "Real literal: " << pi->value() << std::endl;
    
    // 3. Range-based constructors
    std::cout << "\n3. Range-based Constructors:" << std::endl;
    std::vector<std::string> fruits = {"apple", "banana", "orange"};
    auto fruit_array = json_builder()
        .from_container(fruits)
        .build();
    
    std::cout << "Created array from vector with " << std::static_pointer_cast<JsonArray>(fruit_array)->size() << " elements" << std::endl;
    
    // 4. Type safety and smart pointers
    std::cout << "\n4. Type Safety and Smart Pointers:" << std::endl;
    auto mixed_array = jasson_array();
    mixed_array->append(jasson_string("text"));
    mixed_array->append(jasson_integer(static_cast<int64_t>(123)));
    mixed_array->append(jasson_true());
    mixed_array->append(jasson_null());
    
    std::cout << "Mixed array with " << mixed_array->size() << " elements of different types" << std::endl;
    
    // 5. Exception safety with RAII
    std::cout << "\n5. RAII and Exception Safety:" << std::endl;
    try {
        auto builder = json_builder();
        builder.object();  // Start building an object
        builder.add("key", "value");  // This should work
        // builder.append("this will fail");  // This would throw an exception
        auto result = builder.build();
        std::cout << "Successfully built object with RAII" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
