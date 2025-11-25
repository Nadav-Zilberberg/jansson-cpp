/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_SEED_H
#define JANSSON_HASHTABLE_SEED_H

#include <cstdint>
#include <atomic>
#include <mutex>

namespace jansson {

// Global hash table seed for consistent hashing across the application
extern std::atomic<uint32_t> hashtable_seed;

// Initialize the hash table seed
void json_object_seed(size_t seed = 0);

// Get the current hash table seed
inline uint32_t get_hashtable_seed() {
    return hashtable_seed.load(std::memory_order_acquire);
}

// RAII wrapper for hash seed initialization
class HashSeedGuard {
public:
    explicit HashSeedGuard(size_t seed = 0) {
        json_object_seed(seed);
    }
    
    ~HashSeedGuard() = default;
    
    // Non-copyable
    HashSeedGuard(const HashSeedGuard&) = delete;
    HashSeedGuard& operator=(const HashSeedGuard&) = delete;
    
    // Non-movable
    HashSeedGuard(HashSeedGuard&&) = delete;
    HashSeedGuard& operator=(HashSeedGuard&&) = delete;
};

} // namespace jansson

#endif // JANSSON_HASHTABLE_SEED_H
