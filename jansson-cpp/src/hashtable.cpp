/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifdef HAVE_CONFIG_H
#include <jansson_private_config.h>
#endif

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "jansson-cpp/include/hashtable.hpp"
#include "jansson-cpp/include/jansson_private.hpp" /* for container_of() */
#include <jansson_config.h>  /* for JSON_INLINE */

#ifndef INITIAL_HASHTABLE_ORDER
#define INITIAL_HASHTABLE_ORDER 3
#endif

extern volatile uint32_t hashtable_seed;

/* Implementation of the hash function */
#include "jansson-cpp/src/lookup3.hpp"

namespace jansson {

// Helper functions
static size_t hash_str(const std::string& key) {
    return static_cast<size_t>(lookup3::hashlittle(key.c_str(), key.length(), hashtable_seed));
}

static size_t hashsize(size_t order) {
    return static_cast<size_t>(1) << order;
}

// hashtable implementation
hashtable::hashtable() : size_(0), order_(INITIAL_ORDER) {
    size_t bucket_count = hashsize(order_);
    buckets_.resize(bucket_count);
    
    // Initialize list pointers
    list_.prev = &list_;
    list_.next = &list_;
    ordered_list_.prev = &ordered_list_;
    ordered_list_.next = &ordered_list_;
    
    // Initialize buckets
    for (auto& bucket : buckets_) {
        bucket.first = bucket.last = &list_;
    }
}

hashtable::~hashtable() {
    clear();
}

hashtable::hashtable(hashtable&& other) noexcept 
    : size_(other.size_), buckets_(std::move(other.buckets_)), 
      order_(other.order_), list_(other.list_), ordered_list_(other.ordered_list_) {
    // Reset other
    other.size_ = 0;
    other.order_ = INITIAL_ORDER;
    other.buckets_.clear();
    other.list_.prev = &other.list_;
    other.list_.next = &other.list_;
    other.ordered_list_.prev = &other.ordered_list_;
    other.ordered_list_.next = &other.ordered_list_;
}

hashtable& hashtable::operator=(hashtable&& other) noexcept {
    if (this != &other) {
        clear();
        
        size_ = other.size_;
        buckets_ = std::move(other.buckets_);
        order_ = other.order_;
        list_ = other.list_;
        ordered_list_ = other.ordered_list_;
        
        // Reset other
        other.size_ = 0;
        other.order_ = INITIAL_ORDER;
        other.buckets_.clear();
        other.list_.prev = &other.list_;
        other.list_.next = &other.list_;
        other.ordered_list_.prev = &other.ordered_list_;
        other.ordered_list_.next = &other.ordered_list_;
    }
    return *this;
}

// Helper function to find a pair
hashtable_pair* hashtable::find_pair(const std::string& key, size_t hash) {
    size_t index = hash & (hashsize(order_) - 1);
    auto& bucket = buckets_[index];
    
    if (bucket.first == &list_) {
        return nullptr; // Empty bucket
    }
    
    hashtable_list* current = bucket.first;
    while (true) {
        auto* pair = container_of(current, hashtable_pair, list);
        if (pair->hash == hash && pair->key == key) {
            return pair;
        }
        if (current == bucket.last) {
            break;
        }
        current = current->next;
    }
    
    return nullptr;
}

// Helper function to insert into bucket
void hashtable::insert_to_bucket(size_t index, hashtable_pair* pair) {
    auto& bucket = buckets_[index];
    
    if (bucket.first == &list_) {
        // Empty bucket
        bucket.first = bucket.last = &pair->list;
        pair->list.next = &list_;
        pair->list.prev = list_.prev;
        list_.prev->next = &pair->list;
        list_.prev = &pair->list;
    } else {
        // Insert at the beginning of the bucket
        pair->list.next = bucket.first;
        pair->list.prev = bucket.first->prev;
        bucket.first->prev->next = &pair->list;
        bucket.first->prev = &pair->list;
        bucket.first = &pair->list;
    }
    
    // Add to ordered list
    pair->ordered_list.next = &ordered_list_;
    pair->ordered_list.prev = ordered_list_.prev;
    ordered_list_.prev->next = &pair->ordered_list;
    ordered_list_.prev = &pair->ordered_list;
}

int hashtable::set(const std::string& key, json_t* value) {
    size_t hash = hash_str(key);
    auto* existing_pair = find_pair(key, hash);
    
    if (existing_pair) {
        // Replace existing value
        json_decref(existing_pair->value);
        existing_pair->value = value;
        return 0;
    }
    
    // Check if we need to resize
    if (size_ >= hashsize(order_) * 2) {
        // Resize needed - for simplicity, we'll just grow the buckets
        size_t new_order = order_ + 1;
        size_t new_bucket_count = hashsize(new_order);
        std::vector<hashtable_bucket> new_buckets(new_bucket_count);
        
        // Initialize new buckets
        for (auto& bucket : new_buckets) {
            bucket.first = bucket.last = &list_;
        }
        
        // Rehash all existing pairs
        hashtable_list* current = ordered_list_.next;
        while (current != &ordered_list_) {
            auto* pair = container_of(current, hashtable_pair, ordered_list);
            current = current->next;
            
            size_t new_index = pair->hash & (new_bucket_count - 1);
            auto& new_bucket = new_buckets[new_index];
            
            // Insert into new bucket
            if (new_bucket.first == &list_) {
                new_bucket.first = new_bucket.last = &pair->list;
                pair->list.next = &list_;
                pair->list.prev = list_.prev;
                list_.prev->next = &pair->list;
                list_.prev = &pair->list;
            } else {
                pair->list.next = new_bucket.first;
                pair->list.prev = new_bucket.first->prev;
                new_bucket.first->prev->next = &pair->list;
                new_bucket.first->prev = &pair->list;
                new_bucket.first = &pair->list;
            }
        }
        
        buckets_ = std::move(new_buckets);
        order_ = new_order;
    }
    
    // Create new pair
    try {
        auto* new_pair = new hashtable_pair(key, hash, value);
        size_t index = hash & (hashsize(order_) - 1);
        insert_to_bucket(index, new_pair);
        size_++;
        return 0;
    } catch (...) {
        return -1;
    }
}

json_t* hashtable::get(const std::string& key) {
    size_t hash = hash_str(key);
    auto* pair = find_pair(key, hash);
    return pair ? pair->value : nullptr;
}

int hashtable::del(const std::string& key) {
    size_t hash = hash_str(key);
    size_t index = hash & (hashsize(order_) - 1);
    auto& bucket = buckets_[index];
    
    if (bucket.first == &list_) {
        return -1; // Key not found
    }
    
    hashtable_list* current = bucket.first;
    while (true) {
        auto* pair = container_of(current, hashtable_pair, list);
        if (pair->hash == hash && pair->key == key) {
            // Remove from bucket
            if (bucket.first == bucket.last) {
                // Only element in bucket
                bucket.first = bucket.last = &list_;
            } else if (current == bucket.first) {
                bucket.first = current->next;
            } else if (current == bucket.last) {
                bucket.last = current->prev;
            }
            
            // Remove from lists
            current->prev->next = current->next;
            current->next->prev = current->prev;
            
            pair->ordered_list.prev->next = pair->ordered_list.next;
            pair->ordered_list.next->prev = pair->ordered_list.prev;
            
            // Free memory
            json_decref(pair->value);
            delete pair;
            size_--;
            return 0;
        }
        
        if (current == bucket.last) {
            break;
        }
        current = current->next;
    }
    
    return -1; // Key not found
}

void hashtable::clear() {
    hashtable_list* current = ordered_list_.next;
    while (current != &ordered_list_) {
        auto* pair = container_of(current, hashtable_pair, ordered_list);
        current = current->next;
        json_decref(pair->value);
        delete pair;
    }
    
    size_ = 0;
    
    // Reset buckets
    for (auto& bucket : buckets_) {
        bucket.first = bucket.last = &list_;
    }
    
    // Reset lists
    list_.prev = &list_;
    list_.next = &list_;
    ordered_list_.prev = &ordered_list_;
    ordered_list_.next = &ordered_list_;
}

// Iterator implementation
hashtable::iterator hashtable::begin() {
    if (ordered_list_.next == &ordered_list_) {
        return end();
    }
    return iterator(ordered_list_.next);
}

hashtable::iterator hashtable::end() {
    return iterator(&ordered_list_);
}

hashtable::iterator hashtable::find(const std::string& key) {
    size_t hash = hash_str(key);
    auto* pair = find_pair(key, hash);
    return pair ? iterator(&pair->ordered_list) : end();
}

hashtable::iterator::value_type hashtable::iterator::operator*() const {
    auto* pair = container_of(current_, hashtable_pair, ordered_list);
    return std::make_pair(pair->key, pair->value);
}

hashtable::iterator& hashtable::iterator::operator++() {
    current_ = current_->next;
    return *this;
}

hashtable::iterator hashtable::iterator::operator++(int) {
    iterator temp = *this;
    current_ = current_->next;
    return temp;
}

std::string hashtable::iterator::key() const {
    auto* pair = container_of(current_, hashtable_pair, ordered_list);
    return pair->key;
}

json_t* hashtable::iterator::value() const {
    auto* pair = container_of(current_, hashtable_pair, ordered_list);
    return pair->value;
}

void hashtable::iterator::set_value(json_t* value) {
    auto* pair = container_of(current_, hashtable_pair, ordered_list);
    json_decref(pair->value);
    pair->value = value;
}

// Legacy iterator interface
void* hashtable::iter_next(void* iter) {
    if (!iter) return nullptr;
    auto* list = static_cast<hashtable_list*>(iter);
    list = list->next;
    return (list == &ordered_list_) ? nullptr : list;
}

void* hashtable::iter_key(void* iter) {
    if (!iter) return nullptr;
    auto* pair = container_of(static_cast<hashtable_list*>(iter), hashtable_pair, ordered_list);
    return const_cast<char*>(pair->key.c_str());
}

void* hashtable::iter_value(void* iter) {
    if (!iter) return nullptr;
    auto* pair = container_of(static_cast<hashtable_list*>(iter), hashtable_pair, ordered_list);
    return pair->value;
}

void hashtable::iter_set(void* iter, json_t* value) {
    if (!iter) return;
    auto* pair = container_of(static_cast<hashtable_list*>(iter), hashtable_pair, ordered_list);
    json_decref(pair->value);
    pair->value = value;
}

// Legacy C-style interface implementation
int hashtable_init(hashtable_t *hashtable) {
    try {
        new(hashtable) hashtable();
        return 0;
    } catch (...) {
        return -1;
    }
}

void hashtable_close(hashtable_t *hashtable) {
    static_cast<hashtable*>(hashtable)->~hashtable();
}

int hashtable_set(hashtable_t *hashtable, const char *key, size_t key_len, json_t *value) {
    return static_cast<hashtable*>(hashtable)->set(key, key_len, value);
}

void* hashtable_get(hashtable_t *hashtable, const char *key, size_t key_len) {
    return static_cast<hashtable*>(hashtable)->get(key, key_len);
}

int hashtable_del(hashtable_t *hashtable, const char *key, size_t key_len) {
    return static_cast<hashtable*>(hashtable)->del(key, key_len);
}

void hashtable_clear(hashtable_t *hashtable) {
    static_cast<hashtable*>(hashtable)->clear();
}

void* hashtable_iter(hashtable_t *hashtable) {
    return static_cast<hashtable*>(hashtable)->iter();
}

void* hashtable_iter_at(hashtable_t *hashtable, const char *key, size_t key_len) {
    return static_cast<hashtable*>(hashtable)->iter_at(key, key_len);
}

void* hashtable_iter_next(hashtable_t *hashtable, void *iter) {
    return static_cast<hashtable*>(hashtable)->iter_next(iter);
}

void* hashtable_iter_key(void *iter) {
    return hashtable::iterator(static_cast<hashtable_list*>(iter)).key().c_str();
}

size_t hashtable_iter_key_len(void *iter) {
    return hashtable::iterator(static_cast<hashtable_list*>(iter)).key().length();
}

void* hashtable_iter_value(void *iter) {
    return hashtable::iterator(static_cast<hashtable_list*>(iter)).value();
}

void hashtable_iter_set(void *iter, json_t *value) {
    hashtable::iterator(static_cast<hashtable_list*>(iter)).set_value(value);
}

} // namespace jansson
