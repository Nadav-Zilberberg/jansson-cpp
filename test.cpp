#include "jansson-cpp/include/jansson.hpp"
#include <iostream>

int main() {
    try {
        // Test basic JSON operations
        jansson::Json obj = jansson::Json::object();
        obj.objectSet("test", jansson::Json::string("value"));
        
        std::string result = obj.dump();
        std::cout << "JSON: " << result << std::endl;
        
        // Test parsing
        jansson::Json parsed;
        parsed.parse("{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}");
        std::cout << "Parsed JSON: " << parsed.dump() << std::endl;
        
        // Test array
        jansson::Json arr = jansson::Json::array();
        arr.arrayAppend(jansson::Json::integer(1));
        arr.arrayAppend(jansson::Json::integer(2));
        arr.arrayAppend(jansson::Json::integer(3));
        std::cout << "Array: " << arr.dump() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
