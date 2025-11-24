#include "include/jansson.hpp"
#include <iostream>

int main() {
    // Create a simple JSON object
    auto* obj = jansson::json_object();
    auto* str = jansson::json_string("Hello, World!");
    jansson::json_object_set_new(obj, "message", str);
    
    // Dump to string using the modernized function
    std::string result = jansson::json_dumps(obj, 0);
    std::cout << "JSON: " << result << std::endl;
    
    // Test with pretty printing
    std::string pretty = jansson::json_dumps(obj, jansson::JSON_INDENT(2));
    std::cout << "Pretty JSON: " << pretty << std::endl;
    
    // Cleanup
    jansson::json_decref(obj);
    
    return 0;
}
