#include "jansson-cpp.hpp"
#include <iostream>
#include <cassert>

using namespace jasson;

void test_json_object_creation() {
    std::cout << "Testing JSON Object Creation..." << std::endl;
    
    auto object = std::make_shared<JsonObject>();
    assert(object->is_object());
    assert(!object->is_array());
    assert(!object->is_string());
    assert(object->size() == 0);
    
    std::cout << "✓ JSON Object creation test passed" << std::endl;
}

void test_json_object_set_and_get() {
    std::cout << "Testing JSON Object Set and Get..." << std::endl;
    
    auto object = std::make_shared<JsonObject>();
    
    // Set different types
    object->set("integer", static_cast<json_int_t>(42));
    object->set("real", 3.14);
    object->set("string", std::string("hello"));
    object->set("boolean", true);
    object->set_null("null_value");
    
    assert(object->size() == 5);
    assert(object->has("integer"));
    assert(object->has("real"));
    assert(object->has("string"));
    assert(object->has("boolean"));
    assert(object->has("null_value"));
    assert(!object->has("nonexistent"));
    
    // Get and check values
    auto int_val = object->get("integer");
    assert(int_val->is_integer());
    assert(int_val->as_integer()->value() == 42);
    
    auto real_val = object->get("real");
    assert(real_val->is_real());
    assert(real_val->as_real()->value() == 3.14);
    
    auto str_val = object->get("string");
    assert(str_val->is_string());
    assert(str_val->as_string()->value() == "hello");
    
    auto bool_val = object->get("boolean");
    assert(bool_val->is_boolean());
    assert(bool_val->as_boolean()->value() == true);
    
    auto null_val = object->get("null_value");
    assert(null_val->is_null());
    
    std::cout << "✓ JSON Object set and get test passed" << std::endl;
}

void test_json_object_nested() {
    std::cout << "Testing JSON Object Nested..." << std::endl;
    
    auto outer_object = std::make_shared<JsonObject>();
    auto inner_array = std::make_shared<JsonArray>();
    auto inner_object = std::make_shared<JsonObject>();
    
    // Populate inner array
    inner_array->append(static_cast<json_int_t>(1));
    inner_array->append(static_cast<json_int_t>(2));
    inner_array->append(static_cast<json_int_t>(3));
    
    // Populate inner object
    inner_object->set("name", std::string("test"));
    inner_object->set("value", static_cast<json_int_t>(42));
    
    // Add to outer object
    outer_object->set("array", inner_array);
    outer_object->set("object", inner_object);
    outer_object->set("simple", std::string("string value"));
    
    assert(outer_object->size() == 3);
    
    // Check array member
    auto array_member = outer_object->get("array");
    assert(array_member->is_array());
    assert(array_member->as_array()->size() == 3);
    
    // Check object member
    auto object_member = outer_object->get("object");
    assert(object_member->is_object());
    assert(object_member->as_object()->has("name"));
    assert(object_member->as_object()->has("value"));
    
    // Check string member
    auto string_member = outer_object->get("simple");
    assert(string_member->is_string());
    assert(string_member->as_string()->value() == "string value");
    
    std::cout << "✓ JSON Object nested test passed" << std::endl;
}

void test_json_object_missing_key() {
    std::cout << "Testing JSON Object Missing Key..." << std::endl;
    
    auto object = std::make_shared<JsonObject>();
    object->set("existing", std::string("value"));
    
    // Valid access
    auto existing = object->get("existing");
    assert(existing->is_string());
    
    // Invalid access should throw
    bool exception_thrown = false;
    try {
        object->get("nonexistent");
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    
    std::cout << "✓ JSON Object missing key test passed" << std::endl;
}

void test_json_object_serialization() {
    std::cout << "Testing JSON Object Serialization..." << std::endl;
    
    auto object = std::make_shared<JsonObject>();
    object->set("name", std::string("test object"));
    object->set("count", static_cast<json_int_t>(42));
    object->set("valid", true);
    object->set_null("optional");
    
    std::string compact = object->dump();
    std::cout << "Compact object: " << compact << std::endl;
    assert(compact.find("\"name\":\"test object\"") != std::string::npos);
    assert(compact.find("\"count\":42") != std::string::npos);
    assert(compact.find("\"valid\":true") != std::string::npos);
    assert(compact.find("\"optional\":null") != std::string::npos);
    
    std::string pretty = object->dump(2);
    std::cout << "Pretty object: " << pretty << std::endl;
    assert(pretty.find("\n") != std::string::npos);
    assert(pretty.find("  ") != std::string::npos);
    
    std::cout << "✓ JSON Object serialization test passed" << std::endl;
}

void test_json_object_complex() {
    std::cout << "Testing JSON Object Complex..." << std::endl;
    
    auto root = std::make_shared<JsonObject>();
    auto users = std::make_shared<JsonArray>();
    auto user1 = std::make_shared<JsonObject>();
    auto user2 = std::make_shared<JsonObject>();
    
    // Build user1
    user1->set("id", static_cast<json_int_t>(1));
    user1->set("name", std::string("Alice"));
    user1->set("active", true);
    
    // Build user2
    user2->set("id", static_cast<json_int_t>(2));
    user2->set("name", std::string("Bob"));
    user2->set("active", false);
    
    // Add users to array
    users->append(user1);
    users->append(user2);
    
    // Add array to root object
    root->set("users", users);
    root->set("total", static_cast<json_int_t>(2));
    root->set("version", std::string("1.0"));
    
    // Verify structure
    assert(root->has("users"));
    assert(root->has("total"));
    assert(root->has("version"));
    
    auto users_array = root->get("users");
    assert(users_array->is_array());
    assert(users_array->as_array()->size() == 2);
    
    auto first_user = users_array->as_array()->at(0);
    assert(first_user->is_object());
    assert(first_user->as_object()->has("id"));
    assert(first_user->as_object()->has("name"));
    assert(first_user->as_object()->has("active"));
    
    std::string json_str = root->dump(2);
    std::cout << "Complex object: " << json_str << std::endl;
    
    std::cout << "✓ JSON Object complex test passed" << std::endl;
}

int main() {
    std::cout << "=== Running JSON Object Tests ===" << std::endl;
    
    try {
        test_json_object_creation();
        test_json_object_set_and_get();
        test_json_object_nested();
        test_json_object_missing_key();
        test_json_object_serialization();
        test_json_object_complex();
        
        std::cout << "\n🎉 All JSON object tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
