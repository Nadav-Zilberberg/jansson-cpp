#include "jasson.hpp"
#include <iostream>
#include <string>

using namespace jasson;

// Helper function to print JSON values
void print_json(const std::string& name, JsonValuePtr json) {
    std::cout << name << ": " << JsonDumper::dumps(json, true) << std::endl;
}

// Test basic JSON types
void test_basic_types() {
    std::cout << "\n=== Testing Basic Types ===" << std::endl;
    
    // String
    auto str = jasson_string("Hello, World!");
    print_json("String", str);
    
    // Integer
    auto num = jasson_integer(42);
    print_json("Integer", num);
    
    // Real number
    auto real = jasson_real(3.14159);
    print_json("Real number", real);
    
    // Boolean
    auto bool_true = jasson_true();
    auto bool_false = jasson_false();
    print_json("Boolean true", bool_true);
    print_json("Boolean false", bool_false);
    
    // Null
    auto null_val = jasson_null();
    print_json("Null", null_val);
}

// Test JSON objects
void test_objects() {
    std::cout << "\n=== Testing Objects ===" << std::endl;
    
    // Simple object
    auto obj = jasson_object();
    obj->as_object()->add("name", "John Doe");
    obj->as_object()->add("age", json_int_t{30});
    obj->as_object()->add("active", true);
    obj->as_object()->add("salary", 75000.50);
    obj->as_object()->add_null("nickname");
    print_json("Person object", obj);
    
    // Nested object
    auto address = jasson_object();
    address->as_object()->add("street", "123 Main St");
    address->as_object()->add("city", "Anytown");
    address->as_object()->add("zip", "12345");
    obj->as_object()->add("address", address);
    print_json("Person with nested address", obj);
}

// Test JSON arrays
void test_arrays() {
    std::cout << "\n=== Testing Arrays ===" << std::endl;
    
    // Simple array
    auto arr = jasson_array();
    arr->as_array()->append(jasson_string("apple"));
    arr->as_array()->append(jasson_string("banana"));
    arr->as_array()->append(jasson_string("cherry"));
    print_json("Fruits array", arr);
    
    // Mixed type array
    auto mixed = jasson_array();
    mixed->as_array()->append(jasson_string("text"));
    mixed->as_array()->append(jasson_integer(42));
    mixed->as_array()->append(jasson_real(3.14));
    mixed->as_array()->append(jasson_true());
    mixed->as_array()->append(jasson_null());
    print_json("Mixed type array", mixed);
}

// Test fluent builder interface
void test_fluent_builder() {
    std::cout << "\n=== Testing Fluent Builder ===" << std::endl;
    
    // Build object with fluent interface
    auto person = json_builder()
        .object()
        .add("name", "Jane Smith")
        .add("age", 28)
        .add("active", true)
        .add("salary", 85000.0)
        .add_null("middle_name")
        .build();
    print_json("Person (fluent)", person);
    
    // Build array with fluent interface
    auto colors = json_builder()
        .array()
        .append("red")
        .append("green")
        .append("blue")
        .append(42)
        .append(true)
        .build();
    print_json("Colors array (fluent)", colors);
}

// Test literal operators
void test_literal_operators() {
    std::cout << "\n=== Testing Literal Operators ===" << std::endl;
    
    auto greeting = "Hello, World!"_json;
    auto number = 42_json;
    auto pi = 3.14159_json;
    
    print_json("String literal", greeting);
    print_json("Integer literal", number);
    print_json("Real literal", pi);
}

// Test convenience builders
void test_convenience_builders() {
    std::cout << "\n=== Testing Convenience Builders ===" << std::endl;
    
    // Object builder
    auto obj = object_builder()
        .add("type", "convenience")
        .add("value", 123)
        .add("active", true)
        .build();
    print_json("Convenience object", obj);
    
    // Array builder
    auto arr = array_builder()
        .append("one")
        .append("two")
        .append("three")
        .build();
    print_json("Convenience array", arr);
}

// Test complex nested structure
void test_complex_structure() {
    std::cout << "\n=== Testing Complex Structure ===" << std::endl;
    
    auto company = json_builder()
        .object()
        .add("name", "Tech Corp")
        .add("founded", 2010)
        .add("active", true)
        .add("departments", json_builder()
            .array()
            .append(json_builder()
                .object()
                .add("name", "Engineering")
                .add("budget", 1000000)
                .add("employees", json_builder()
                    .array()
                    .append(json_builder()
                        .object()
                        .add("name", "Alice Johnson")
                        .add("position", "Software Engineer")
                        .add("salary", 95000)
                        .build())
                    .append(json_builder()
                        .object()
                        .add("name", "Bob Smith")
                        .add("position", "Product Manager")
                        .add("salary", 105000)
                        .build())
                    .build())
                .build())
            .append(json_builder()
                .object()
                .add("name", "Sales")
                .add("budget", 500000)
                .add("employees", json_builder()
                    .array()
                    .append(json_builder()
                        .object()
                        .add("name", "Carol White")
                        .add("position", "Sales Manager")
                        .add("salary", 80000)
                        .build())
                    .build())
                .build())
            .build())
        .build();
    
    print_json("Complex company structure", company);
}

int main() {
    std::cout << "JSON Library Demo" << std::endl;
    std::cout << "=================" << std::endl;
    
    test_basic_types();
    test_objects();
    test_arrays();
    test_fluent_builder();
    test_literal_operators();
    test_convenience_builders();
    test_complex_structure();
    
    std::cout << "\nDemo completed successfully!" << std::endl;
    return 0;
}
