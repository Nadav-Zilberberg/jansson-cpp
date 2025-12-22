/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_SIMPLE_H
#define JANSSON_HASHTABLE_SIMPLE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>
#include <vector>
#include <algorithm>
#include "jansson.h"
#include "hashtable_seed.hpp"
#include "lookup3.hpp"

namespace jansson {

// Hash function wrapper for std::string
struct StringHash {
    std::size_t operator()(const std::string& str) const noexcept {
        return static_cast<std::size_t>(
            hashlittle(str.c_str(), str.length(), get_hashtable_seed())
        );
    }
};

// Thread-safe HashTable implementation that replaces the C hashtable_t
class HashTable {
public:
    using key_type = std::string;
    using mapped_type = json_t*;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using hasher = StringHash;

private:
    // Internal storage type
    using container_type = std::unordered_map<key_type, mapped_type, hasher>;
    
    // Container for ordered iteration (to maintain compatibility)
    using ordered_container_type = std::vector<key_type>;

public:
    // Constructor and destructor
    HashTable();
    ~HashTable();

    // Non-copyable but movable
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(HashTable&& other) noexcept;

    // Core operations (compatible with C API)
    int set(const char* key, size_t key_len, json_t* value);
    json_t* get(const char* key, size_t key_len) const;
    int del(const char* key, size_t key_len);
    void clear();
    
    // C++ friendly overloads
    int set(const std::string& key, json_t* value);
    json_t* get(const std::string& key) const;
    int del(const std::string& key);

    // Size and capacity
    size_type size() const noexcept;
    bool empty() const noexcept;

    // Thread safety
    void lock_shared() const;
    void unlock_shared() const;
    void lock_exclusive();
    void unlock_exclusive();

    // Iterator support (for compatibility)
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = HashTable::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = HashTable::value_type*;
        using reference = HashTable::value_type&;

        iterator() : table_(nullptr), index_(0) {}
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
        
        reference operator*() const;
        pointer operator->() const;
        iterator& operator++();
        iterator operator++(int);
        
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        friend class HashTable;
        iterator(HashTable* table, size_type index);
        
        HashTable* table_;
        size_type index_;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = HashTable::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const HashTable::value_type*;
        using reference = const HashTable::value_type&;

        const_iterator() : table_(nullptr), index_(0) {}
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        
        const_iterator(const iterator& other);
        
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        
        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

    private:
        friend class HashTable;
        const_iterator(const HashTable* table, size_type index);
        
        const HashTable* table_;
        size_type index_;
    };

    // Iterator functions (for compatibility)
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // Legacy iterator functions
    iterator iter_at(const char* key, size_t key_len);
    const_iterator iter_at(const char* key, size_t key_len) const;
    
    static const char* iter_key(const const_iterator& iter);
    static json_t* iter_value(const const_iterator& iter);

    // Friend functions
    friend void swap(HashTable& lhs, HashTable& rhs) noexcept;

private:
    void update_ordered_keys();
    void decrement_refcounts();
    void increment_refcounts();
    
    mutable std::shared_mutex mutex_;
    container_type container_;
    ordered_container_type ordered_keys_;
    size_type size_;
};

// Free functions
void swap(HashTable& lhs, HashTable& rhs) noexcept;

} // namespace jansson

#endif // JANSSON_HASHTABLE_SIMPLE_H
