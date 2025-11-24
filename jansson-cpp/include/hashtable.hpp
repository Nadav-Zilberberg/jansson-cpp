/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "jansson.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace jansson {

// Forward declarations
struct hashtable_list;
struct hashtable_pair;
struct hashtable_bucket;

// Modernized list structure using C++ features
struct hashtable_list {
    hashtable_list *prev;
    hashtable_list *next;
    
    // Constructor for easy initialization
    hashtable_list() : prev(this), next(this) {}
};

// Modernized pair structure
struct hashtable_pair {
    hashtable_list list;
    hashtable_list ordered_list;
    size_t hash;
    json_t *value;
    size_t key_len;
    std::string key;
    
    // Constructor
    hashtable_pair(const std::string& k, size_t h, json_t* v) 
        : hash(h), value(v), key_len(k.length()), key(k) {
        // Initialize list pointers
        list.prev = &list;
        list.next = &list;
        ordered_list.prev = &ordered_list;
        ordered_list.next = &ordered_list;
    }
};

// Modernized bucket structure
struct hashtable_bucket {
    hashtable_list *first;
    hashtable_list *last;
    
    hashtable_bucket() : first(nullptr), last(nullptr) {}
};

// Legacy typedef for backward compatibility
typedef struct hashtable_t hashtable_t;

// Modernized hashtable class
class hashtable {
private:
    size_t size_;
    std::vector<hashtable_bucket> buckets_;
    size_t order_; // hashtable has pow(2, order_) buckets
    hashtable_list list_;
    hashtable_list ordered_list_;
    
    static constexpr size_t INITIAL_ORDER = 3;
    
public:
    // Constructor
    hashtable();
    
    // Destructor
    ~hashtable();
    
    // Delete copy constructor and assignment operator
    hashtable(const hashtable&) = delete;
    hashtable& operator=(const hashtable&) = delete;
    
    // Move constructor and assignment operator
    hashtable(hashtable&& other) noexcept;
    hashtable& operator=(hashtable&& other) noexcept;
    
    // Modernized interface using std::string
    int set(const std::string& key, json_t* value);
    json_t* get(const std::string& key);
    int del(const std::string& key);
    void clear();
    
    // Size and capacity
    size_t size() const { return size_; }
    size_t bucket_count() const { return buckets_.size(); }
    bool empty() const { return size_ == 0; }
    
    // Iterator class for modern C++ iteration
    class iterator {
    private:
        hashtable_list* current_;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<std::string, json_t*>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        
        iterator() : current_(nullptr) {}
        explicit iterator(hashtable_list* list) : current_(list) {}
        
        value_type operator*() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return current_ == other.current_; }
        bool operator!=(const iterator& other) const { return current_ != other.current_; }
        
        // Get key and value directly
        std::string key() const;
        json_t* value() const;
        void set_value(json_t* value);
        
        // For legacy compatibility
        hashtable_list* get_current() const { return current_; }
    };
    
    // Iterator interface
    iterator begin();
    iterator end();
    iterator find(const std::string& key);
    
    // Compatibility interface for existing code
    int set(const char* key, size_t key_len, json_t* value) {
        return set(std::string(key, key_len), value);
    }
    
    void* get(const char* key, size_t key_len) {
        return get(std::string(key, key_len));
    }
    
    int del(const char* key, size_t key_len) {
        return del(std::string(key, key_len));
    }
    
    // Legacy iterator interface for compatibility
    void* iter() { return begin().get_current(); }
    void* iter_at(const char* key, size_t key_len) { return find(std::string(key, key_len)).get_current(); }
    void* iter_next(void* iter);
    void* iter_key(void* iter);
    size_t iter_key_len(void* iter);
    void* iter_value(void* iter);
    void iter_set(void* iter, json_t* value);
};

// Legacy C-style interface for backward compatibility
int hashtable_init(hashtable_t *hashtable);
void hashtable_close(hashtable_t *hashtable);
int hashtable_set(hashtable_t *hashtable, const char *key, size_t key_len, json_t *value);
void* hashtable_get(hashtable_t *hashtable, const char *key, size_t key_len);
int hashtable_del(hashtable_t *hashtable, const char *key, size_t key_len);
void hashtable_clear(hashtable_t *hashtable);
void* hashtable_iter(hashtable_t *hashtable);
void* hashtable_iter_at(hashtable_t *hashtable, const char *key, size_t key_len);
void* hashtable_iter_next(hashtable_t *hashtable, void *iter);
void* hashtable_iter_key(void *iter);
size_t hashtable_iter_key_len(void *iter);
void* hashtable_iter_value(void *iter);
void hashtable_iter_set(void *iter, json_t *value);

} // namespace jansson

// Macro for backward compatibility
#define hashtable_key_to_iter(key_) \
    (&(container_of(key_, jansson::hashtable_pair, key)->ordered_list))

#endif // HASHTABLE_HPP
