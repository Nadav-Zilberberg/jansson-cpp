/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "hashtable.hpp"
#include "hashtable_seed.hpp"
#include "../src/lookup3.h"
#include <algorithm>
#include <utility>

namespace jansson {

// StringHash implementation
std::size_t StringHash::operator()(const std::string& str) const noexcept {
    // Use the same hash function as the original jansson
    return static_cast<std::size_t>(hashlittle(str.c_str(), str.length(), hashtable_seed.load(std::memory_order_acquire)));
}

// Explicit instantiation for json_t*
template class HashTable<json_t*, std::shared_mutex>;

} // namespace jansson
