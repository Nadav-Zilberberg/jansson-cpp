/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_H
#define JANSSON_HASHTABLE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <iterator>
#include <optional>
#include <stdexcept>
#include "jansson.h"
#include "memory.hpp"
#include <vector>

namespace jansson {

// Forward declaration
class json_ptr;

// Hash function wrapper for std::string
struct StringHash {
    std::size_t operator()(const std::string& str) const noexcept;
};

// Thread-safe HashTable implementation using std::unordered_map
template<typename ValueType = json_t*, typename LockType = std::shared_mutex>
class HashTable {
public:
    using key_type = std::string;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using hasher = StringHash;
    using key_equal = std::equal_to<key_type>;
    using allocator_type = std::allocator<value_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

private:
    // Internal storage type
    using container_type = std::unordered_map<key_type, mapped_type, hasher, key_equal, allocator_type>;
    
    // Container for ordered iteration
    using ordered_container_type = std::vector<key_type>;

public:
    // Forward iterator class
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = HashTable::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = HashTable::pointer;
        using reference = HashTable::reference;

        iterator() = default;
        
        iterator(const iterator& other) = default;
        iterator& operator=(const iterator& other) = default;
        
        reference operator*() const;
        pointer operator->() const;
        iterator& operator++();
        iterator operator++(int);
        
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        friend class HashTable;
        
        iterator(HashTable* table, typename ordered_container_type::const_iterator ordered_iter);
        
        HashTable* table_;
        typename ordered_container_type::const_iterator ordered_iter_;
    };

    // Const iterator class
    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = HashTable::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = HashTable::const_pointer;
        using reference = HashTable::const_reference;

        const_iterator() = default;
        
        const_iterator(const const_iterator& other) = default;
        const_iterator& operator=(const const_iterator& other) = default;
        
        // Convert from iterator to const_iterator
        const_iterator(const iterator& other);
        
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        
        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

    private:
        friend class HashTable;
        
        const_iterator(const HashTable* table, typename ordered_container_type::const_iterator ordered_iter);
        
        const HashTable* table_;
        typename ordered_container_type::const_iterator ordered_iter_;
    };

    // Constructors and destructor
    explicit HashTable(size_type bucket_count = 8,
                       memory_resource* mr = get_default_memory_resource());
    
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    
    ~HashTable();

    // Assignment operators
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    // Element access
    mapped_type& at(const key_type& key);
    const mapped_type& at(const key_type& key) const;
    
    mapped_type& operator[](const key_type& key);
    mapped_type& operator[](key_type&& key);

    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // Modifiers
    void clear() noexcept;
    
    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);
    
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    
    iterator erase(const_iterator pos);
    size_type erase(const key_type& key);
    
    void swap(HashTable& other) noexcept;

    // Lookup
    size_type count(const key_type& key) const;
    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;
    
    std::pair<iterator, iterator> equal_range(const key_type& key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

    // Hash policy
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);
    
    void rehash(size_type count);
    void reserve(size_type count);

    // Thread safety
    void lock_shared() const;
    void unlock_shared() const;
    void lock_exclusive();
    void unlock_exclusive();
    
    // Legacy API compatibility
    int set(const key_type& key, mapped_type value);
    mapped_type get(const key_type& key) const;
    int del(const key_type& key);
    
    // Iterator-based legacy API
    iterator iter_at(const key_type& key);
    const_iterator iter_at(const key_type& key) const;
    
    static key_type iter_key(const_iterator iter);
    static mapped_type iter_value(const_iterator iter);
    static void iter_set(const_iterator iter, mapped_type value);

    // Memory resource access
    memory_resource* get_memory_resource() const { return memory_resource_; }

private:
    void update_ordered_keys();
    void decrement_refcounts();
    void increment_refcounts();
    
    container_type container_;
    ordered_container_type ordered_keys_;
    mutable LockType mutex_;
    memory_resource* memory_resource_;
    size_type size_;
};

// Specialization for json_t* to handle reference counting
template<>
class HashTable<json_t*, std::shared_mutex> {
    // ... (same interface as above, but with json_t* specific implementations)
};

// Type aliases for common use cases
using json_hashtable = HashTable<json_t*>;
using json_hashtable_mt = HashTable<json_t*, std::shared_mutex>;

// Free functions
void swap(json_hashtable& lhs, json_hashtable& rhs) noexcept;

} // namespace jansson

#endif // JANSSON_HASHTABLE_H
