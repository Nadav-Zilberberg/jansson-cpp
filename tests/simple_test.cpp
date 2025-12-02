#include "../src/json_value.hpp"
#include "../src/json_types.hpp"
#include <iostream>
#include <memory>

using namespace jasson;

int main() {
    std::cout << "Testing JSON library base fixtures..." << std::endl;
    
    try {
        // Test basic JSON value creation
        auto null_val = std::make_shared<JsonNull>();
        std::cout << "✓ JsonNull created successfully" << std::endl;
        
        auto bool_val = std::make_shared<JsonBoolean>(true);
        std::cout << "✓ JsonBoolean created successfully" << std::endl;
        
        auto int_val = std::make_shared<JsonInteger>(42);
        std::cout << "✓ JsonInteger created successfully" << std::endl;
        
        auto real_val = std::make_shared<JsonReal>(3.14);
        std::cout << "✓ JsonReal created successfully" << std::endl;
        
        auto str_val = std::make_shared<JsonString>("test");
        std::cout << "✓ JsonString created successfully" << std::endl;
        
        // Test array creation
        auto arr = std::make_shared<JsonArray>();
        arr->append(int_val);
        arr->append(str_val);
        std::cout << "✓ JsonArray created with " << arr->size() << " elements" << std::endl;
        
        // Test object creation
        auto obj = std::make_shared<JsonObject>();
        obj->set("name", str_val);
        obj->set("age", int_val);
        obj->set("active", bool_val);
        std::cout << "✓ JsonObject created with " << obj->size() << " properties" << std::endl;
        
        // Test serialization
        std::cout << "✓ Array serialization: " << arr->to_string() << std::endl;
        std::cout << "✓ Object serialization: " << obj->to_string() << std::endl;
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
