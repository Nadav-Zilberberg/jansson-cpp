#ifndef JANSSON_LOOKUP3_HPP
#define JANSSON_LOOKUP3_HPP

#include <cstdint>
#include <cstddef>

namespace jansson {

// Simple hash function implementation based on FNV-1a hash
// This replaces the lookup3 hash function from the original C implementation

inline uint32_t hashlittle(const void* key, size_t length, uint32_t initval = 0) {
    const uint8_t* data = static_cast<const uint8_t*>(key);
    uint32_t hash = initval;
    
    // FNV-1a hash algorithm
    const uint32_t FNV_prime = 16777619u;
    const uint32_t FNV_offset_basis = 2166136261u;
    
    if (hash == 0) {
        hash = FNV_offset_basis;
    }
    
    for (size_t i = 0; i < length; ++i) {
        hash ^= data[i];
        hash *= FNV_prime;
    }
    
    return hash;
}

// Additional hash functions for different data types
inline uint32_t hashlittle2(const void* key, size_t length, uint32_t* pc, uint32_t* pb) {
    uint32_t hash = hashlittle(key, length, *pc);
    *pc = hash;
    *pb = hash;
    return hash;
}

inline uint64_t hashlittle64(const void* key, size_t length, uint64_t initval = 0) {
    const uint8_t* data = static_cast<const uint8_t*>(key);
    uint64_t hash = initval;
    
    // FNV-1a 64-bit hash algorithm
    const uint64_t FNV_prime = 1099511628211ull;
    const uint64_t FNV_offset_basis = 14695981039346656037ull;
    
    if (hash == 0) {
        hash = FNV_offset_basis;
    }
    
    for (size_t i = 0; i < length; ++i) {
        hash ^= data[i];
        hash *= FNV_prime;
    }
    
    return hash;
}

// Hash function for integers
inline uint32_t hash_int(uint32_t key, uint32_t initval = 0) {
    // Simple integer hash using multiplication and XOR
    uint32_t hash = key ^ initval;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;
    return hash;
}

// Hash function for 64-bit integers
inline uint64_t hash_int64(uint64_t key, uint64_t initval = 0) {
    uint64_t hash = key ^ initval;
    hash = (~hash) + (hash << 18);
    hash = hash ^ (hash >> 31);
    hash = hash * 21;
    hash = hash ^ (hash >> 11);
    hash = hash + (hash << 6);
    hash = hash ^ (hash >> 22);
    return hash;
}

} // namespace jansson

#endif // JANSSON_LOOKUP3_HPP
