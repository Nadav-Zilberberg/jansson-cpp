#include "jansson_cpp.hpp"
#include <iostream>
#include <cassert>

using namespace jansson_cpp;

void test_basic_creation() {
    std::cout << "Testing basic JSON value creation..." << std::endl;
    
    // Test object creation
    JsonValue obj = make_object();
    assert(obj.is_object());
    assert(obj);
    std::cout << "✓ Object creation successful" << std::endl;
    
    // Test array creation
    JsonValue arr = make_array();
    assert(arr.is_array());
    assert(arr);
    std::cout << "✓ Array creation successful" << std::endl;
    
    // Test string creation
    JsonValue str = make_string("Hello, World!");
    assert(str.is_string());
    assert(str);
    std::cout << "✓ String creation successful" << std::endl;
    
    // Test integer creation
    JsonValue num = make_integer(42);
    assert(num.is_integer());
    assert(num);
    std::cout << "✓ Integer creation successful" << std::endl;
    
    // Test real creation
    JsonValue real = make_real(3.14);
    assert(real.is_real());
    assert(real);
    std::cout << "✓ Real number creation successful" << std::endl;
    
    // Test boolean creation
    JsonValue bool_true = make_true();
    assert(bool_true.is_boolean());
    assert(bool_true);
    std::cout << "✓ Boolean true creation successful" << std::endl;
    
    JsonValue bool_false = make_false();
    assert(bool_false.is_boolean());
    assert(bool_false);
    std::cout << "✓ Boolean false creation successful" << std::endl;
    
    // Test null creation
    JsonValue null_val = make_null();
    assert(null_val.is_null());
    assert(null_val);
    std::cout << "✓ Null creation successful" << std::endl;
    
    std::cout << "All basic creation tests passed!" << std::endl;
}

void test_reference_counting() {
    std::cout << "\nTesting reference counting..." << std::endl;
    
    // Test copy constructor
    JsonValue obj1 = make_object();
    JsonValue obj2 = obj1;
    assert(obj1.is_object());
    assert(obj2.is_object());
    std::cout << "✓ Copy constructor successful" << std::endl;
    
    // Test assignment
    JsonValue obj3;
    obj3 = obj1;
    assert(obj3.is_object());
    std::cout << "✓ Assignment operator successful" << std::endl;
    
    // Test move semantics
    JsonValue obj4 = make_object();
    JsonValue obj5 = std::move(obj4);
    assert(!obj4);  // obj4 should be empty after move
    assert(obj5.is_object());
    std::cout << "✓ Move semantics successful" << std::endl;
    
    std::cout << "All reference counting tests passed!" << std::endl;
}

int main() {
    try {
        test_basic_creation();
        test_reference_counting();
        std::cout << "\n🎉 All tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
