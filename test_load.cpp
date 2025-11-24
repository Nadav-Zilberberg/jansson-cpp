#include "jansson-cpp/include/jansson.hpp"
#include <iostream>
#include <string>

using namespace jansson;

int main() {
    // Test basic JSON parsing
    std::string json_str = R"({"name": "test", "value": 42, "active": true})";
    
    json_error_t error;
    json_t* json = json_loads(json_str, 0, &error);
    
    if (!json) {
        std::cerr << "Failed to parse JSON: " << error.text << std::endl;
        return 1;
    }
    
    // Test that we can access the parsed data
    json_t* name = json_object_get(json, "name");
    json_t* value = json_object_get(json, "value");
    json_t* active = json_object_get(json, "active");
    
    if (name && json_is_string(name)) {
        std::cout << "Name: " << json_string_value(name) << std::endl;
    }
    
    if (value && json_is_integer(value)) {
        std::cout << "Value: " << json_integer_value(value) << std::endl;
    }
    
    if (active && json_is_boolean(active)) {
        std::cout << "Active: " << (json_is_true(active) ? "true" : "false") << std::endl;
    }
    
    json_decref(json);
    return 0;
}
