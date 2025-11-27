#ifndef JASSON_JSON_EXTENSIONS_HPP
#define JASSON_JSON_EXTENSIONS_HPP

#include "json_value.hpp"
#include "json_types.hpp"
#include <memory>

namespace jasson {

// Extension methods for JsonValue classes to add missing functionality
class JsonValueExtensions {
public:
    // Helper functions to implement the missing methods
    static bool is_true(const JsonValue& value) {
        return value.is_boolean() && static_cast<const JsonBoolean&>(value).value();
    }
    
    static bool is_false(const JsonValue& value) {
        return value.is_boolean() && !static_cast<const JsonBoolean&>(value).value();
    }
    
    static bool equals(const JsonValue& a, const JsonValue& b) {
        if (a.is_null() && b.is_null()) return true;
        if (a.is_null() || b.is_null()) return false;
        
        if (a.is_boolean() && b.is_boolean()) {
            return static_cast<const JsonBoolean&>(a).value() == static_cast<const JsonBoolean&>(b).value();
        }
        if (a.is_integer() && b.is_integer()) {
            return static_cast<const JsonInteger&>(a).value() == static_cast<const JsonInteger&>(b).value();
        }
        if (a.is_real() && b.is_real()) {
            return static_cast<const JsonReal&>(a).value() == static_cast<const JsonReal&>(b).value();
        }
        if (a.is_string() && b.is_string()) {
            return static_cast<const JsonString&>(a).value() == static_cast<const JsonString&>(b).value();
        }
        
        if (a.is_array() && b.is_array()) {
            const auto& arr_a = static_cast<const JsonArray&>(a);
            const auto& arr_b = static_cast<const JsonArray&>(b);
            if (arr_a.size() != arr_b.size()) return false;
            for (size_t i = 0; i < arr_a.size(); ++i) {
                if (!equals(*arr_a.at(i), *arr_b.at(i))) return false;
            }
            return true;
        }
        
        if (a.is_object() && b.is_object()) {
            const auto& obj_a = static_cast<const JsonObject&>(a);
            const auto& obj_b = static_cast<const JsonObject&>(b);
            if (obj_a.size() != obj_b.size()) return false;
            // Use the public API to iterate through object keys
            for (auto it = obj_a.begin(); it != obj_a.end(); ++it) {
                const std::string& key = it->first;
                auto it_b = obj_b.find(key);
                if (it_b == obj_b.end() || !equals(*it->second, *it_b->second)) return false;
            }
            return true;
        }
        
        return false;
    }
    
    static JsonValuePtr copy(const JsonValue& value) {
        if (value.is_null()) return std::make_shared<JsonNull>();
        if (value.is_boolean()) return std::make_shared<JsonBoolean>(static_cast<const JsonBoolean&>(value).value());
        if (value.is_integer()) return std::make_shared<JsonInteger>(static_cast<const JsonInteger&>(value).value());
        if (value.is_real()) return std::make_shared<JsonReal>(static_cast<const JsonReal&>(value).value());
        if (value.is_string()) return std::make_shared<JsonString>(static_cast<const JsonString&>(value).value());
        
        if (value.is_array()) {
            const auto& arr = static_cast<const JsonArray&>(value);
            auto result = std::make_shared<JsonArray>();
            for (size_t i = 0; i < arr.size(); ++i) {
                result->append(arr.at(i)->clone());
            }
            return result;
        }
        
        if (value.is_object()) {
            const auto& obj = static_cast<const JsonObject&>(value);
            auto result = std::make_shared<JsonObject>();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                result->set(it->first, it->second->clone());
            }
            return result;
        }
        
        return nullptr;
    }
    
    static JsonValuePtr deep_copy(const JsonValue& value) {
        if (value.is_null()) return std::make_shared<JsonNull>();
        if (value.is_boolean()) return std::make_shared<JsonBoolean>(static_cast<const JsonBoolean&>(value).value());
        if (value.is_integer()) return std::make_shared<JsonInteger>(static_cast<const JsonInteger&>(value).value());
        if (value.is_real()) return std::make_shared<JsonReal>(static_cast<const JsonReal&>(value).value());
        if (value.is_string()) return std::make_shared<JsonString>(static_cast<const JsonString&>(value).value());
        
        if (value.is_array()) {
            const auto& arr = static_cast<const JsonArray&>(value);
            auto result = std::make_shared<JsonArray>();
            for (size_t i = 0; i < arr.size(); ++i) {
                result->append(deep_copy(*arr.at(i))->clone());
            }
            return result;
        }
        
        if (value.is_object()) {
            const auto& obj = static_cast<const JsonObject&>(value);
            auto result = std::make_shared<JsonObject>();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                result->set(it->first, deep_copy(*it->second)->clone());
            }
            return result;
        }
        
        return nullptr;
    }
};

// Wrapper functions that can be used by the jasson.hpp functions
inline bool json_is_true(const JsonValue& value) {
    return JsonValueExtensions::is_true(value);
}

inline bool json_is_false(const JsonValue& value) {
    return JsonValueExtensions::is_false(value);
}

inline bool json_equals(const JsonValue& a, const JsonValue& b) {
    return JsonValueExtensions::equals(a, b);
}

inline JsonValuePtr json_copy(const JsonValue& value) {
    return JsonValueExtensions::copy(value);
}

inline JsonValuePtr json_deep_copy(const JsonValue& value) {
    return JsonValueExtensions::deep_copy(value);
}

} // namespace jasson

#endif // JASSON_JSON_EXTENSIONS_HPP
