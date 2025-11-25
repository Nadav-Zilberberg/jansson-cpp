/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 * Copyright (c) 2011-2012 Basile Starynkevitch <basile@starynkevitch.net>
 *
 * Jansson is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_MEMORY_H
#define JANSSON_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include <memory>
#include <type_traits>
#include "jansson.h"

namespace jansson {

// Memory resource base class
class memory_resource {
public:
    virtual ~memory_resource() = default;
    
    virtual void* allocate(size_t bytes) = 0;
    virtual void deallocate(void* ptr, size_t bytes) = 0;
    virtual void* reallocate(void* ptr, size_t old_size, size_t new_size) = 0;
};

// Default memory resource using malloc/free
class default_memory_resource : public memory_resource {
public:
    void* allocate(size_t bytes) override {
        if (!bytes) return nullptr;
        return malloc(bytes);
    }
    
    void deallocate(void* ptr, size_t) override {
        free(ptr);
    }
    
    void* reallocate(void* ptr, size_t old_size, size_t new_size) override {
        if (new_size == 0) {
            free(ptr);
            return nullptr;
        }
        
        void* new_ptr = malloc(new_size);
        if (new_ptr && ptr) {
            memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
            free(ptr);
        }
        return new_ptr;
    }
};

// Get the default memory resource
inline default_memory_resource* get_default_memory_resource() {
    static default_memory_resource instance;
    return &instance;
}

// Custom memory resource wrapper
class custom_memory_resource : public memory_resource {
private:
    json_malloc_t malloc_fn;
    json_realloc_t realloc_fn;
    json_free_t free_fn;

public:
    custom_memory_resource(json_malloc_t m, json_realloc_t r, json_free_t f)
        : malloc_fn(m), realloc_fn(r), free_fn(f) {}
    
    void* allocate(size_t bytes) override {
        if (!bytes) return nullptr;
        return malloc_fn(bytes);
    }
    
    void deallocate(void* ptr, size_t) override {
        free_fn(ptr);
    }
    
    void* reallocate(void* ptr, size_t old_size, size_t new_size) override {
        if (realloc_fn) {
            return realloc_fn(ptr, new_size);
        }
        // Fallback to malloc/free emulation
        if (new_size == 0) {
            free_fn(ptr);
            return nullptr;
        }
        
        void* new_ptr = malloc_fn(new_size);
        if (new_ptr && ptr) {
            memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
            free_fn(ptr);
        }
        return new_ptr;
    }
};

// Smart pointer for JSON values
class json_ptr {
private:
    json_t* ptr;
    memory_resource* mem_res;

public:
    explicit json_ptr(json_t* j = nullptr, memory_resource* mr = get_default_memory_resource())
        : ptr(j), mem_res(mr) {}
    
    ~json_ptr() {
        if (ptr) {
            json_decref(ptr);
        }
    }
    
    json_ptr(const json_ptr& other) : ptr(other.ptr), mem_res(other.mem_res) {
        if (ptr) {
            json_incref(ptr);
        }
    }
    
    json_ptr(json_ptr&& other) noexcept : ptr(other.ptr), mem_res(other.mem_res) {
        other.ptr = nullptr;
    }
    
    json_ptr& operator=(const json_ptr& other) {
        if (this != &other) {
            if (ptr) {
                json_decref(ptr);
            }
            ptr = other.ptr;
            mem_res = other.mem_res;
            if (ptr) {
                json_incref(ptr);
            }
        }
        return *this;
    }
    
    json_ptr& operator=(json_ptr&& other) noexcept {
        if (this != &other) {
            if (ptr) {
                json_decref(ptr);
            }
            ptr = other.ptr;
            mem_res = other.mem_res;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    json_t* get() const { return ptr; }
    json_t* release() {
        json_t* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }
    
    void reset(json_t* j = nullptr) {
        if (ptr) {
            json_decref(ptr);
        }
        ptr = j;
    }
    
    json_t& operator*() const { return *ptr; }
    json_t* operator->() const { return ptr; }
    
    explicit operator bool() const { return ptr != nullptr; }
    
    bool operator!=(std::nullptr_t) const { return ptr != nullptr; }
    bool operator==(std::nullptr_t) const { return ptr == nullptr; }
    
    memory_resource* get_memory_resource() const { return mem_res; }
};

// Factory functions for creating JSON values with specific memory resource
template<typename T, typename... Args>
json_ptr make_json(memory_resource* mr, Args&&... args) {
    // This would need to be implemented in the main jansson library
    // For now, we'll just use the regular creation functions
    json_t* j = T::create(std::forward<Args>(args)...);
    return json_ptr(j, mr);
}

// Memory resource management
void set_memory_resource(memory_resource* mr);
memory_resource* get_memory_resource();

} // namespace jansson

#endif // JANSSON_MEMORY_H
