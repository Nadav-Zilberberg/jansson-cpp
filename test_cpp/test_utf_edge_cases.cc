#include "src/utf.h"
#include <iostream>
#include <string>
#include <vector>

void test_invalid_sequences() {
    std::cout << "\nTesting invalid UTF-8 sequences:\n";
    
    // Continuation byte without start
    std::string invalid1 = "\x80";
    if (!utf8_is_valid(invalid1)) {
        std::cout << "✓ Continuation byte without start detected\n";
    }
    
    // Overlong encoding
    std::string invalid2 = "\xC0\x80";
    if (!utf8_is_valid(invalid2)) {
        std::cout << "✓ Overlong encoding detected\n";
    }
    
    // Invalid code point (surrogate)
    std::string invalid3 = "\xED\xA0\x80";
    if (!utf8_is_valid(invalid3)) {
        std::cout << "✓ UTF-16 surrogate detected\n";
    }
    
    // Beyond Unicode range
    std::string invalid4 = "\xF4\x90\x80\x80";
    if (!utf8_is_valid(invalid4)) {
        std::cout << "✓ Code point beyond Unicode range detected\n";
    }
    
    // Truncated sequence
    std::string invalid5 = "\xF0";
    if (!utf8_is_valid(invalid5)) {
        std::cout << "✓ Truncated sequence detected\n";
    }
}

void test_boundary_conditions() {
    std::cout << "\nTesting boundary conditions:\n";
    
    // Empty string
    if (utf8_is_valid("")) {
        std::cout << "✓ Empty string handled correctly\n";
    }
    
    // ASCII
    if (utf8_is_valid("Hello")) {
        std::cout << "✓ ASCII string validated\n";
    }
    
    // Maximum valid code points
    auto max_2byte = utf8_encode(0x7FF);
    auto max_3byte = utf8_encode(0xFFFF);
    auto max_4byte = utf8_encode(0x10FFFF);
    
    if (max_2byte && max_3byte && max_4byte) {
        std::cout << "✓ Maximum valid code points encoded\n";
    }
    
    // Invalid code points
    auto invalid_cp1 = utf8_encode(0xD800);
    auto invalid_cp2 = utf8_encode(0x110000);
    
    if (!invalid_cp1 && !invalid_cp2) {
        std::cout << "✓ Invalid code points rejected\n";
    }
}

void test_mixed_strings() {
    std::cout << "\nTesting mixed valid/invalid strings:\n";
    
    // Valid string with invalid byte appended
    std::string mixed1 = "Hello\xFF";
    if (!utf8_is_valid(mixed1)) {
        std::cout << "✓ Valid string with invalid byte detected\n";
    }
    
    // Invalid sequence in middle of valid string
    std::string mixed2 = "Hello\xC0\x80World";
    if (!utf8_is_valid(mixed2)) {
        std::cout << "✓ Invalid sequence in middle detected\n";
    }
}

int main() {
    test_invalid_sequences();
    test_boundary_conditions();
    test_mixed_strings();
    return 0;
}
