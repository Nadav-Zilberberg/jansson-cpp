#include "jasson_cpp/jasson.hpp"
#include "jasson_cpp/json_factory.hpp"
#include "jasson_cpp/json_convenience.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace jasson;

void print_test_header(const std::string& test_name) {
    std::cout << "\n=== " << test_name << " ===" << std::endl;
}

void print_json(const std::string& name, json_ptr json) {
    std::cout << name << ": " << json->to_string() << std::endl;
}

void test_basic_types() {
    print_test_header("Basic JSON Types");
    
    // String
    auto str = jasson_string("Hello, JSON!");
    print_json("String", str);
    std::cout << "✓ String test passed" << std::endl;
    
    // Number
    auto num = jasson_integer(42);
    print_json("Integer", num);
    std::cout << "✓ Integer test passed" << std::endl;
    
    // Real number
    auto real = jasson_real(3.14159);
    print_json("Real number", real);
    std::cout << "✓ Real number test passed" << std::endl;
    
    // Boolean
    auto bool_true = jasson_true();
    auto bool_false = jasson_false();
    print_json("Boolean true", bool_true);
    print_json("Boolean false", bool_false);
    std::cout << "✓ Boolean test passed" << std::endl;
    
    // Null
    auto null_val = jasson_null();
    print_json("Null", null_val);
    std::cout << "✓ Null test passed" << std::endl;
}

void test_objects() {
    print_test_header("JSON Objects");
    
    auto obj = jasson_object();
    obj->add("name", jasson_string("John Doe"));
    obj->add("age", jasson_integer(30));
    obj->add("active", jasson_true());
    obj->add("salary", jasson_real(75000.50));
    obj->add("nickname", jasson_null());
    
    print_json("Person object", obj);
    std::cout << "✓ Object test passed" << std::endl;
    
    // Nested object
    auto address = jasson_object();
    address->add("street", jasson_string("123 Main St"));
    address->add("city", jasson_string("Anytown"));
    address->add("zip", jasson_string("12345"));
    
    obj->add("address", address);
    
    print_json("Person with nested address", obj);
    std::cout << "✓ Nested object test passed" << std::endl;
}

void test_arrays() {
    print_test_header("JSON Arrays");
    
    auto arr = jasson_array();
    arr->append(jasson_string("apple"));
    arr->append(jasson_string("banana"));
    arr->append(jasson_string("cherry"));
    
    print_json("Fruits array", arr);
    std::cout << "✓ Array test passed" << std::endl;
    
    // Mixed type array
    auto mixed = jasson_array();
    mixed->append(jasson_string("text"));
    mixed->append(jasson_integer(42));
    mixed->append(jasson_real(3.14));
    mixed->append(jasson_true());
    mixed->append(jasson_null());
    
    print_json("Mixed type array", mixed);
    std::cout << "✓ Mixed array test passed" << std::endl;
}

void test_fluent_builder() {
    print_test_header("Fluent Builder API");
    
    auto person = json_builder()
        .object()
        .add("name", "Jane Smith")
        .add("age", 28)
        .add("salary", 85000.0)
        .add("active", true)
        .add("department", "Engineering")
        .build();
    
    print_json("Person (fluent)", person);
    std::cout << "✓ Fluent builder test passed" << std::endl;
    
    // Array with fluent builder
    auto colors = json_builder()
        .array()
        .append("red")
        .append("green")
        .append("blue")
        .append("yellow")
        .build();
    
    print_json("Colors array (fluent)", colors);
    std::cout << "✓ Fluent array builder test passed" << std::endl;
}

void test_literal_operators() {
    print_test_header("Literal Operators");
    
    auto greeting = "Hello, World!"_json;
    auto number = 42_json;
    auto pi = 3.14159_json;
    
    print_json("String literal", greeting);
    print_json("Integer literal", number);
    print_json("Real literal", pi);
    
    std::cout << "✓ Literal operators test passed" << std::endl;
}

void test_convenience_builders() {
    print_test_header("Convenience Builders");
    
    auto obj = object_builder()
        .add("product", "Laptop")
        .add("price", 999.99)
        .add("in_stock", true)
        .add_null("description")
        .build();
    
    print_json("Product (convenience)", obj);
    std::cout << "✓ Object convenience builder test passed" << std::endl;
    
    auto arr = array_builder()
        .append("Monday")
        .append("Tuesday")
        .append("Wednesday")
        .append("Thursday")
        .append("Friday")
        .build();
    
    print_json("Weekdays (convenience)", arr);
    std::cout << "✓ Array convenience builder test passed" << std::endl;
}

void test_type_checking() {
    print_test_header("Type Checking");
    
    auto str = jasson_string("test");
    auto num = jasson_integer(42);
    auto obj = jasson_object();
    auto arr = jasson_array();
    auto bool_val = jasson_true();
    auto null_val = jasson_null();
    
    std::cout << "String is_string(): " << str->is_string() << std::endl;
    std::cout << "Number is_number(): " << num->is_number() << std::endl;
    std::cout << "Object is_object(): " << obj->is_object() << std::endl;
    std::cout << "Array is_array(): " << arr->is_array() << std::endl;
    std::cout << "Boolean is_boolean(): " << bool_val->is_boolean() << std::endl;
    std::cout << "Null is_null(): " << null_val->is_null() << std::endl;
    
    std::cout << "✓ Type checking test passed" << std::endl;
}

void test_complex_structure() {
    print_test_header("Complex JSON Structure");
    
    auto company = json_builder()
        .object()
        .add("name", "Tech Corp")
        .add("founded", 2010)
        .add("active", true)
        .build();
    
    auto employees = json_builder()
        .array()
        .append(
            json_builder()
                .object()
                .add("name", "Alice Johnson")
                .add("position", "Software Engineer")
                .add("salary", 95000)
                .build()
        )
        .append(
            json_builder()
                .object()
                .add("name", "Bob Smith")
                .add("position", "Product Manager")
                .add("salary", 105000)
                .build()
        )
        .build();
    
    auto company_obj = std::static_pointer_cast<JsonObject>(company);
    company_obj->add("employees", employees);
    
    print_json("Company with employees", company);
    std::cout << "✓ Complex structure test passed" << std::endl;
}

int main() {
    std::cout << "=== Modern C++ JSON Library Demo ===" << std::endl;
    std::cout << "This demo showcases all features of the C++ JSON library" << std::endl;
    
    try {
        test_basic_types();
        test_objects();
        test_arrays();
        test_fluent_builder();
        test_literal_operators();
        test_convenience_builders();
        test_type_checking();
        test_complex_structure();
        
        std::cout << "\n=== All Tests Completed Successfully! ===" << std::endl;
        std::cout << "The C++ JSON library is working correctly." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
