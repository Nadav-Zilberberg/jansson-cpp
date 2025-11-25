#include "jasson_cpp/jasson.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cassert>
#include <vector>

using namespace jasson;

void test_string_loading() {
    std::cout << "Testing string loading..." << std::endl;
    
    // Test basic object
    std::string json_str = R"({"name": "John", "age": 30, "active": true})";
    auto result = json_loads(json_str);
    assert(result->is_object());
    auto obj = std::static_pointer_cast<JsonObject>(result);
    assert(obj->get("name")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("name"))->value() == "John");
    assert(obj->get("age")->is_integer());
    assert(std::static_pointer_cast<JsonNumber>(obj->get("age"))->int_value() == 30);
    assert(obj->get("active")->is_true());
    
    // Test array
    std::string array_str = R"([1, 2, 3, "hello", null, true, false])";
    auto array_result = json_loads(array_str);
    assert(array_result->is_array());
    auto arr = std::static_pointer_cast<JsonArray>(array_result);
    assert(arr->size() == 7);
    assert(arr->get(0)->is_integer());
    assert(arr->get(3)->is_string());
    assert(arr->get(4)->is_null());
    assert(arr->get(5)->is_true());
    assert(arr->get(6)->is_false());
    
    std::cout << "✓ String loading tests passed!" << std::endl;
}

void test_stream_loading() {
    std::cout << "Testing stream loading..." << std::endl;
    
    std::string json_str = R"({"stream": "test", "value": 42})";
    std::istringstream stream(json_str);
    
    auto result = json_load_stream(stream);
    assert(result->is_object());
    auto obj = std::static_pointer_cast<JsonObject>(result);
    assert(obj->get("stream")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("stream"))->value() == "test");
    assert(obj->get("value")->is_integer());
    assert(std::static_pointer_cast<JsonNumber>(obj->get("value"))->int_value() == 42);
    
    std::cout << "✓ Stream loading tests passed!" << std::endl;
}

void test_file_loading() {
    std::cout << "Testing file loading..." << std::endl;
    
    // Create temporary file
    std::string json_content = R"({
        "file": "test.json",
        "nested": {
            "array": [1, 2, 3],
            "boolean": true
        }
    })";
    
    std::string filename = "test_temp.json";
    std::ofstream file(filename);
    file << json_content;
    file.close();
    
    // Test loading with string path
    auto result1 = json_load_file(filename);
    assert(result1->is_object());
    auto obj1 = std::static_pointer_cast<JsonObject>(result1);
    assert(obj1->get("file")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj1->get("file"))->value() == "test.json");
    
    // Test loading with filesystem path
    std::filesystem::path filepath(filename);
    auto result2 = json_load_file(filepath);
    assert(result2->is_object());
    assert(json_equal(result1, result2));
    
    // Clean up
    std::filesystem::remove(filename);
    
    std::cout << "✓ File loading tests passed!" << std::endl;
}

void test_iterator_loading() {
    std::cout << "Testing iterator-based loading..." << std::endl;
    
    std::string json_str = R"({"iterator": "test", "data": [1, 2, 3]})";
    
    JsonLoader loader;
    
    // Test with string iterators
    auto result1 = loader.load_from_iterators(json_str.begin(), json_str.end());
    assert(result1->is_object());
    auto obj1 = std::static_pointer_cast<JsonObject>(result1);
    assert(obj1->get("iterator")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj1->get("iterator"))->value() == "test");
    
    // Test with const char* iterators
    const char* cstr = json_str.c_str();
    auto result2 = loader.load_from_iterators(cstr, cstr + json_str.length());
    assert(result2->is_object());
    assert(json_equal(result1, result2));
    
    std::cout << "✓ Iterator loading tests passed!" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    JsonLoader loader;
    
    // Test invalid JSON
    try {
        std::string invalid_json = R"({"name": "John", "age":})";
        auto result = json_loads(invalid_json);
        assert(false); // Should not reach here
    } catch (const JsonLoadException& e) {
        assert(e.line() > 0 || e.column() > 0);
        assert(std::string(e.what()).find("Unexpected") != std::string::npos);
    }
    
    // Test empty input
    try {
        std::string empty_json = "";
        auto result = json_loads(empty_json);
        assert(false); // Should not reach here
    } catch (const JsonLoadException& e) {
        assert(std::string(e.what()).find("Empty") != std::string::npos);
    }
    
    // Test unexpected content after JSON
    try {
        std::string extra_content = R"({"valid": "json"} extra content)";
        auto result = json_loads(extra_content);
        assert(false); // Should not reach here
    } catch (const JsonLoadException& e) {
        assert(std::string(e.what()).find("Unexpected content") != std::string::npos);
    }
    
    std::cout << "✓ Error handling tests passed!" << std::endl;
}

void test_configuration_flags() {
    std::cout << "Testing configuration flags..." << std::endl;
    
    // Test REJECT_DUPLICATES flag
    try {
        std::string dup_keys = R"({"key": "value1", "key": "value2"})";
        auto result = json_loads(dup_keys, JsonLoadFlags::REJECT_DUPLICATES);
        assert(false); // Should not reach here
    } catch (const JsonLoadException& e) {
        assert(std::string(e.what()).find("Duplicate key") != std::string::npos);
    }
    
    // Test DISABLE_EOF_CHECK flag
    std::string extra_content = R"({"valid": "json"} extra content)";
    auto result1 = json_loads(extra_content, JsonLoadFlags::DISABLE_EOF_CHECK);
    assert(result1->is_object());
    auto obj1 = std::static_pointer_cast<JsonObject>(result1);
    assert(obj1->get("valid")->is_string());
    
    // Test DECODE_INT_AS_REAL flag
    std::string int_json = R"({"number": 42})";
    auto result2 = json_loads(int_json, JsonLoadFlags::DECODE_INT_AS_REAL);
    auto result2_obj = std::static_pointer_cast<JsonObject>(result2);    assert(result2_obj->get("number")->is_real());    assert(std::static_pointer_cast<JsonNumber>(result2_obj->get("number"))->real_value() == 42.0);
    
    std::cout << "✓ Configuration flags tests passed!" << std::endl;
}

void test_complex_nesting() {
    std::cout << "Testing complex nesting..." << std::endl;
    
    std::string complex_json = R"({
        "users": [
            {
                "id": 1,
                "name": "Alice",
                "profile": {
                    "age": 25,
                    "interests": ["reading", "hiking"],
                    "address": {
                        "street": "123 Main St",
                        "city": "Anytown"
                    }
                }
            },
            {
                "id": 2,
                "name": "Bob",
                "profile": {
                    "age": 30,
                    "interests": ["coding", "gaming"],
                    "address": {
                        "street": "456 Oak Ave",
                        "city": "Somewhere"
                    }
                }
            }
        ],
        "total": 2,
        "active": true
    })";
    
    auto result = json_loads(complex_json);
    assert(result->is_object());
    auto obj = std::static_pointer_cast<JsonObject>(result);
    assert(obj->get("users")->is_array());
    auto users = std::static_pointer_cast<JsonArray>(obj->get("users"));
    assert(users->size() == 2);
    assert(users->get(0)->is_object());
    auto user0 = std::static_pointer_cast<JsonObject>(users->get(0));
    assert(user0->get("profile")->is_object());
    auto profile = std::static_pointer_cast<JsonObject>(user0->get("profile"));
    assert(profile->get("interests")->is_array());
    auto interests = std::static_pointer_cast<JsonArray>(profile->get("interests"));
    assert(interests->size() == 2);
    
    std::cout << "✓ Complex nesting tests passed!" << std::endl;
}

void test_unicode_and_escapes() {
    std::cout << "Testing Unicode and escape sequences..." << std::endl;
    
    // Test escape sequences
    std::string escapes_json = R"({
        "newline": "line1\nline2",
        "tab": "col1\tcol2",
        "quote": "She said \"Hello\"",
        "backslash": "path\\to\\file",
        "unicode": "\u0048\u0065\u006C\u006C\u006F"
    })";
    
    auto result = json_loads(escapes_json);
    assert(result->is_object());
    auto obj = std::static_pointer_cast<JsonObject>(result);
    assert(obj->get("newline")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("newline"))->value().find('\n') != std::string::npos);
    assert(obj->get("tab")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("tab"))->value().find('\t') != std::string::npos);
    assert(obj->get("quote")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("quote"))->value().find('"') != std::string::npos);
    assert(obj->get("backslash")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("backslash"))->value().find('\\') != std::string::npos);
    assert(obj->get("unicode")->is_string());
    assert(std::static_pointer_cast<JsonString>(obj->get("unicode"))->value() == "Hello");
    
    std::cout << "✓ Unicode and escape sequences tests passed!" << std::endl;
}

int main() {
    std::cout << "=== JsonLoader Comprehensive Test Suite ===" << std::endl;
    
    test_string_loading();
    test_stream_loading();
    test_file_loading();
    test_iterator_loading();
    test_error_handling();
    test_configuration_flags();
    test_complex_nesting();
    test_unicode_and_escapes();
    
    std::cout << "\n=== All comprehensive tests passed! ===" << std::endl;
    return 0;
}
