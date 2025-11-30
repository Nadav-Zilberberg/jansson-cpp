/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "hashtable_seed.hpp"
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>

namespace jansson {

// Global hash table seed
std::atomic<uint32_t> hashtable_seed{0};

// Initialize the hash table seed
void json_object_seed(size_t seed) {
    if (seed == 0) {
        // Generate a random seed if none provided
        std::random_device rd;
        std::mt19937 gen(rd() ^ static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<uint32_t> dis;
        seed = dis(gen);
    }
    
    hashtable_seed.store(static_cast<uint32_t>(seed), std::memory_order_release);
}

// Static initialization
namespace {
    struct SeedInitializer {
        SeedInitializer() {
            json_object_seed(0); // Initialize with random seed
        }
    };
    
    // This will initialize the seed before main() is called
    SeedInitializer seed_initializer;
}

} // namespace jansson
