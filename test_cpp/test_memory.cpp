#include "src/jansson_cpp.h"
#include "src/jansson.h"
#include <iostream>
#include <cassert>

// Global memory resource for testing
jansson::memory_resource* test_mr = nullptr;

extern "C" {
    void* test_malloc(size_t size) {
        return test_mr->allocate(size);
    }
    
    void test_free(void* ptr) {
        test_mr->deallocate(ptr, 0);
    }
}

int main() {
    // Test 1: Basic memory resource operations
    {
        auto* default_mr = jansson::get_default_memory_resource();
        void* ptr = default_mr->allocate(100);
        assert(ptr != nullptr);
        default_mr->deallocate(ptr, 100);
        std::cout << "Test 1 passed: Basic memory resource operations\n";
    }
    
    // Test 2: Custom memory resource
    {
        static int alloc_count = 0;
        static int free_count = 0;
        
        auto custom_malloc = [](size_t size) -> void* {
            alloc_count++;
            return malloc(size);
        };
        
        auto custom_free = [](void* ptr) {
            free_count++;
            free(ptr);
        };
        
        jansson::custom_memory_resource custom_mr(custom_malloc, nullptr, custom_free);
        void* ptr = custom_mr.allocate(50);
        assert(ptr != nullptr);
        assert(alloc_count == 1);
        custom_mr.deallocate(ptr, 50);
        assert(free_count == 1);
        std::cout << "Test 2 passed: Custom memory resource\n";
    }
    
    // Test 3: JSON smart pointer
    {
        jansson::json_ptr json(json_string("test"));
        assert(json != nullptr);
        assert(json_string_value(json.get()) == std::string("test"));
        std::cout << "Test 3 passed: JSON smart pointer\n";
    }
    
    // Test 4: Memory resource switching via C API
    {
        auto* new_mr = new jansson::default_memory_resource();
        test_mr = new_mr;
        
        // Set custom allocators through C API
        json_set_alloc_funcs(test_malloc, test_free);
        
        json_t* obj = json_object();
        assert(obj != nullptr);
        json_decref(obj);
        
        // Reset to default
        json_set_alloc_funcs(malloc, free);
        delete new_mr;
        test_mr = nullptr;
        std::cout << "Test 4 passed: Memory resource switching\n";
    }
    
    std::cout << "All tests passed!\n";
    return 0;
}
