#include <iostream>
#include <cassert>
#include <string>

// Forward declare the functions we need
namespace jansson {
namespace strconv {
    int jsonp_strtod(const std::string& str, double& out);
    int jsonp_dtostr(char* buffer, size_t size, double value, int precision);
}
}

#include "jansson-cpp/src/strconv.cpp"

int main() {
    using namespace jansson::strconv;
    
    // Test jsonp_strtod
    double result;
    assert(jsonp_strtod("123.45", result) == 0);
    assert(result == 123.45);
    
    assert(jsonp_strtod("-987.654", result) == 0);
    assert(result == -987.654);
    
    assert(jsonp_strtod("invalid", result) == -1);
    
    // Test jsonp_dtostr
    char buffer[100];
    assert(jsonp_dtostr(buffer, sizeof(buffer), 123.45, 2) > 0);
    std::cout << "123.45 -> " << buffer << std::endl;
    
    assert(jsonp_dtostr(buffer, sizeof(buffer), -987.654, 3) > 0);
    std::cout << "-987.654 -> " << buffer << std::endl;
    
    assert(jsonp_dtostr(buffer, sizeof(buffer), 1e10, 0) > 0);
    std::cout << "1e10 -> " << buffer << std::endl;
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
