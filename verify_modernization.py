#!/usr/bin/env python3

import os
import re

def check_file(filepath):
    """Check if the file has been modernized according to requirements"""
    with open(filepath, 'r') as f:
        content = f.read()
    
    checks = {
        "C++ headers": bool(re.search(r'#include <(iostream|memory|string|vector|functional)>', content)),
        "Classes instead of structs": bool(re.search(r'class (Stream|Lexer|CallbackData)', content)),
        "std::string usage": bool(re.search(r'std::string', content)),
        "std::function usage": bool(re.search(r'std::function', content)),
        "constexpr usage": bool(re.search(r'constexpr', content)),
        "auto usage": bool(re.search(r'\bauto\b', content)),
        "std::vector usage": bool(re.search(r'std::vector', content)),
        "Namespace usage": bool(re.search(r'namespace jansson', content)),
        "No malloc/free": not bool(re.search(r'\b(malloc|free)\b', content)),
        "No C-style strings": not bool(re.search(r'char\s*\*.*\[', content)),
    }
    
    return checks

def main():
    filepath = "./jansson-cpp/src/load.cpp"
    
    if not os.path.exists(filepath):
        print(f"Error: {filepath} not found")
        return 1
    
    checks = check_file(filepath)
    
    print("=== Modernization Verification Results ===")
    print(f"File: {filepath}")
    print()
    
    passed = 0
    total = len(checks)
    
    for check, result in checks.items():
        status = "✅ PASS" if result else "❌ FAIL"
        print(f"{check:25} {status}")
        if result:
            passed += 1
    
    print()
    print(f"Results: {passed}/{total} checks passed")
    
    if passed == total:
        print("🎉 All modernization requirements satisfied!")
        return 0
    else:
        print("⚠️  Some modernization requirements not met")
        return 1

if __name__ == "__main__":
    exit(main())
