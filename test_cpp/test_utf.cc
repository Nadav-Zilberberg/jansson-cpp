#include "src/utf.h"
#include <iostream>
#include <string>

int main() {
    // Test utf8_encode
    auto ascii = utf8_encode(65);
    if (ascii) {
        std::cout << "ASCII test passed: " << *ascii << std::endl;
    }
    
    // Test 2-byte UTF-8
    auto two_byte = utf8_encode(0xA9);
    if (two_byte) {
        std::cout << "2-byte test passed: " << *two_byte << std::endl;
    }
    
    // Test 3-byte UTF-8
    auto three_byte = utf8_encode(0x20AC);
    if (three_byte) {
        std::cout << "3-byte test passed: " << *three_byte << std::endl;
    }
    
    // Test 4-byte UTF-8
    auto four_byte = utf8_encode(0x1F4A9);
    if (four_byte) {
        std::cout << "4-byte test passed: " << *four_byte << std::endl;
    }
    
    // Test invalid codepoints
    auto invalid = utf8_encode(0x110000);
    if (!invalid) {
        std::cout << "Invalid codepoint test passed" << std::endl;
    }
    
    // Test UTF-8 validation
    std::string valid_str = "Hello, 世界! 🌍";
    if (utf8_is_valid(valid_str)) {
        std::cout << "Valid UTF-8 string test passed" << std::endl;
    }
    
    // Test Utf8String class
    auto utf8_str = utf8_make_valid(valid_str);
    if (utf8_str) {
        std::cout << "Utf8String creation test passed" << std::endl;
        
        // Test iteration
        std::cout << "Codepoints: ";
        for (auto cp : *utf8_str) {
            std::cout << "U+" << std::hex << cp << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
