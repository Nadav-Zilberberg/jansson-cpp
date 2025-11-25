/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "hashtable_simple.hpp"
#include <algorithm>
#include <stdexcept>

namespace jansson {

// Constructor and destructor
HashTable::HashTable() : container_(), ordered_keys_(), size_(0) {
    container_.reserve(8);
    ordered_keys_.reserve(8);
}

HashTable::~HashTable() {
    decrement_refcounts();
}

// Move constructor and assignment
HashTable::HashTable(HashTable&& other) noexcept
    : container_(std::move(other.container_)),
      ordered_keys_(std::move(other.ordered_keys_)),
      size_(other.size_) {
    other.size_ = 0;
}

HashTable& HashTable::operator=(HashTable&& other) noexcept {
    if (this != &other) {
        std::unique_lock<std::shared_mutex> lock_this(mutex_);
        std::unique_lock<std::shared_mutex> lock_other(other.mutex_);
        
        decrement_refcounts();
        container_ = std::move(other.container_);
        ordered_keys_ = std::move(other.ordered_keys_);
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}

// Core operations
int HashTable::set(const char* key, size_t key_len, json_t* value) {
    try {
        std::string key_str(key, key_len);
        return set(key_str, value);
    } catch (...) {
        return -1;
    }
}

json_t* HashTable::get(const char* key, size_t key_len) const {
    std::string key_str(key, key_len);
    return get(key_str);
}

int HashTable::del(const char* key, size_t key_len) {
    std::string key_str(key, key_len);
    return del(key_str);
}

int HashTable::set(const std::string& key, json_t* value) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = container_.find(key);
    if (it != container_.end()) {
        // Update existing value
        if (it->second) {
            json_decref(it->second);
        }
        it->second = value;
        if (value) {
            json_incref(value);
        }
    } else {
        // Insert new value
        auto result = container_.emplace(key, value);
        if (result.second) {
            ordered_keys_.push_back(key);
            if (value) {
                json_incref(value);
            }
            size_++;
        }
    }
    return 0;
}

json_t* HashTable::get(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = container_.find(key);
    if (it == container_.end()) {
        return nullptr;
    }
    return it->second;
}

int HashTable::del(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = container_.find(key);
    if (it == container_.end()) {
        return -1;
    }
    
    if (it->second) {
        json_decref(it->second);
    }
    container_.erase(it);
    
    auto ordered_it = std::find(ordered_keys_.begin(), ordered_keys_.end(), key);
    if (ordered_it != ordered_keys_.end()) {
        ordered_keys_.erase(ordered_it);
    }
    size_--;
    
    return 0;
}

void HashTable::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    decrement_refcounts();
    container_.clear();
    ordered_keys_.clear();
    size_ = 0;
}

// Size and capacity
HashTable::size_type HashTable::size() const noexcept {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return size_;
}

bool HashTable::empty() const noexcept {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return size_ == 0;
}

// Thread safety
void HashTable::lock_shared() const {
    mutex_.lock_shared();
}

void HashTable::unlock_shared() const {
    mutex_.unlock_shared();
}

void HashTable::lock_exclusive() {
    mutex_.lock();
}

void HashTable::unlock_exclusive() {
    mutex_.unlock();
}

// Iterator implementations
HashTable::iterator::iterator(HashTable* table, size_type index)
    : table_(table), index_(index) {}

HashTable::iterator::reference HashTable::iterator::operator*() const {
    if (!table_ || index_ >= table_->ordered_keys_.size()) {
        throw std::out_of_range("Iterator dereference out of range");
    }
    const auto& key = table_->ordered_keys_[index_];
    auto it = table_->container_.find(key);
    if (it == table_->container_.end()) {
        throw std::runtime_error("Iterator inconsistency");
    }
    return *it;
}

HashTable::iterator::pointer HashTable::iterator::operator->() const {
    return &(operator*());
}

HashTable::iterator& HashTable::iterator::operator++() {
    if (index_ < table_->ordered_keys_.size()) {
        ++index_;
    }
    return *this;
}

HashTable::iterator HashTable::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool HashTable::iterator::operator==(const iterator& other) const {
    return table_ == other.table_ && index_ == other.index_;
}

bool HashTable::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

HashTable::const_iterator::const_iterator(const HashTable* table, size_type index)
    : table_(table), index_(index) {}

HashTable::const_iterator::const_iterator(const iterator& other)
    : table_(other.table_), index_(other.index_) {}

HashTable::const_iterator::reference HashTable::const_iterator::operator*() const {
    if (!table_ || index_ >= table_->ordered_keys_.size()) {
        throw std::out_of_range("Iterator dereference out of range");
    }
    const auto& key = table_->ordered_keys_[index_];
    auto it = table_->container_.find(key);
    if (it == table_->container_.end()) {
        throw std::runtime_error("Iterator inconsistency");
    }
    return *it;
}

HashTable::const_iterator::pointer HashTable::const_iterator::operator->() const {
    return &(operator*());
}

HashTable::const_iterator& HashTable::const_iterator::operator++() {
    if (index_ < table_->ordered_keys_.size()) {
        ++index_;
    }
    return *this;
}

HashTable::const_iterator HashTable::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool HashTable::const_iterator::operator==(const const_iterator& other) const {
    return table_ == other.table_ && index_ == other.index_;
}

bool HashTable::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

// Iterator functions
HashTable::iterator HashTable::begin() {
    return iterator(this, 0);
}

HashTable::const_iterator HashTable::begin() const {
    return const_iterator(this, 0);
}

HashTable::iterator HashTable::end() {
    return iterator(this, ordered_keys_.size());
}

HashTable::const_iterator HashTable::end() const {
    return const_iterator(this, ordered_keys_.size());
}

HashTable::iterator HashTable::iter_at(const char* key, size_t key_len) {
    std::string key_str(key, key_len);
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = std::find(ordered_keys_.begin(), ordered_keys_.end(), key_str);
    if (it == ordered_keys_.end()) {
        return end();
    }
    return iterator(this, std::distance(ordered_keys_.begin(), it));
}

HashTable::const_iterator HashTable::iter_at(const char* key, size_t key_len) const {
    std::string key_str(key, key_len);
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = std::find(ordered_keys_.begin(), ordered_keys_.end(), key_str);
    if (it == ordered_keys_.end()) {
        return end();
    }
    return const_iterator(this, std::distance(ordered_keys_.begin(), it));
}

const char* HashTable::iter_key(const const_iterator& iter) {
    if (!iter.table_ || iter.index_ >= iter.table_->ordered_keys_.size()) {
        return nullptr;
    }
    return iter.table_->ordered_keys_[iter.index_].c_str();
}

json_t* HashTable::iter_value(const const_iterator& iter) {
    if (!iter.table_ || iter.index_ >= iter.table_->ordered_keys_.size()) {
        return nullptr;
    }
    const auto& key = iter.table_->ordered_keys_[iter.index_];
    auto it = iter.table_->container_.find(key);
    if (it == iter.table_->container_.end()) {
        return nullptr;
    }
    return it->second;
}

// Private helper methods
void HashTable::decrement_refcounts() {
    for (auto& pair : container_) {
        if (pair.second) {
            json_decref(pair.second);
        }
    }
}

void HashTable::increment_refcounts() {
    for (auto& pair : container_) {
        if (pair.second) {
            json_incref(pair.second);
        }
    }
}

// Free functions
void swap(HashTable& lhs, HashTable& rhs) noexcept {
    if (&lhs != &rhs) {
        std::unique_lock<std::shared_mutex> lock_lhs(lhs.mutex_);
        std::unique_lock<std::shared_mutex> lock_rhs(rhs.mutex_);
        
        lhs.container_.swap(rhs.container_);
        lhs.ordered_keys_.swap(rhs.ordered_keys_);
        std::swap(lhs.size_, rhs.size_);
    }
}

} // namespace jansson
