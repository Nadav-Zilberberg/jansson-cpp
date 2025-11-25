/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_IMPL_H
#define JANSSON_HASHTABLE_IMPL_H

#include "hashtable.hpp"
#include "hashtable_seed.hpp"
#include "../src/lookup3.h"
#include <algorithm>
#include <utility>

namespace jansson {

// Iterator implementations for HashTable<json_t*, std::shared_mutex>
inline HashTable<json_t*, std::shared_mutex>::iterator::iterator(HashTable* table, typename ordered_container_type::const_iterator ordered_iter)
    : table_(table), ordered_iter_(ordered_iter) {}

inline HashTable<json_t*, std::shared_mutex>::iterator::reference 
HashTable<json_t*, std::shared_mutex>::iterator::operator*() const {
    if (!table_ || ordered_iter_ == table_->ordered_keys_.end()) {
        throw std::out_of_range("Iterator dereference out of range");
    }
    auto map_it = table_->container_.find(*ordered_iter_);
    if (map_it == table_->container_.end()) {
        throw std::runtime_error("Iterator inconsistency");
    }
    return *map_it;
}

inline HashTable<json_t*, std::shared_mutex>::iterator::pointer 
HashTable<json_t*, std::shared_mutex>::iterator::operator->() const {
    return &(operator*());
}

inline HashTable<json_t*, std::shared_mutex>::iterator& 
HashTable<json_t*, std::shared_mutex>::iterator::operator++() {
    if (ordered_iter_ != table_->ordered_keys_.end()) {
        ++ordered_iter_;
    }
    return *this;
}

inline HashTable<json_t*, std::shared_mutex>::iterator 
HashTable<json_t*, std::shared_mutex>::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

inline bool HashTable<json_t*, std::shared_mutex>::iterator::operator==(const iterator& other) const {
    return table_ == other.table_ && ordered_iter_ == other.ordered_iter_;
}

inline bool HashTable<json_t*, std::shared_mutex>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

// Const iterator implementations
inline HashTable<json_t*, std::shared_mutex>::const_iterator::const_iterator(const HashTable* table, typename ordered_container_type::const_iterator ordered_iter)
    : table_(table), ordered_iter_(ordered_iter) {}

inline HashTable<json_t*, std::shared_mutex>::const_iterator::const_iterator(const iterator& other)
    : table_(other.table_), ordered_iter_(other.ordered_iter_) {}

inline HashTable<json_t*, std::shared_mutex>::const_iterator::reference 
HashTable<json_t*, std::shared_mutex>::const_iterator::operator*() const {
    if (!table_ || ordered_iter_ == table_->ordered_keys_.end()) {
        throw std::out_of_range("Iterator dereference out of range");
    }
    auto map_it = table_->container_.find(*ordered_iter_);
    if (map_it == table_->container_.end()) {
        throw std::runtime_error("Iterator inconsistency");
    }
    return *map_it;
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator::pointer 
HashTable<json_t*, std::shared_mutex>::const_iterator::operator->() const {
    return &(operator*());
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator& 
HashTable<json_t*, std::shared_mutex>::const_iterator::operator++() {
    if (ordered_iter_ != table_->ordered_keys_.end()) {
        ++ordered_iter_;
    }
    return *this;
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator 
HashTable<json_t*, std::shared_mutex>::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

inline bool HashTable<json_t*, std::shared_mutex>::const_iterator::operator==(const const_iterator& other) const {
    return table_ == other.table_ && ordered_iter_ == other.ordered_iter_;
}

inline bool HashTable<json_t*, std::shared_mutex>::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

// Iterator factory functions
inline HashTable<json_t*, std::shared_mutex>::iterator 
HashTable<json_t*, std::shared_mutex>::begin() noexcept {
    return iterator(this, ordered_keys_.begin());
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator 
HashTable<json_t*, std::shared_mutex>::begin() const noexcept {
    return const_iterator(this, ordered_keys_.begin());
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator 
HashTable<json_t*, std::shared_mutex>::cbegin() const noexcept {
    return const_iterator(this, ordered_keys_.begin());
}

inline HashTable<json_t*, std::shared_mutex>::iterator 
HashTable<json_t*, std::shared_mutex>::end() noexcept {
    return iterator(this, ordered_keys_.end());
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator 
HashTable<json_t*, std::shared_mutex>::end() const noexcept {
    return const_iterator(this, ordered_keys_.end());
}

inline HashTable<json_t*, std::shared_mutex>::const_iterator 
HashTable<json_t*, std::shared_mutex>::cend() const noexcept {
    return const_iterator(this, ordered_keys_.end());
}

} // namespace jansson

#endif // JANSSON_HASHTABLE_IMPL_H
