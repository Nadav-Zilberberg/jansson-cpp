#ifndef JSON_EXTENSIONS_HPP
#define JSON_EXTENSIONS_HPP

#include "json_value.hpp"
#include <algorithm>
#include <numeric>

namespace jasson {

// JSON Path operations
class JsonPath {
public:
    static JsonValuePtr get(const JsonValuePtr& root, const std::string& path) {
        if (!root) return nullptr;
        
        std::vector<std::string> tokens = split_path(path);
        JsonValuePtr current = root;
        
        for (const auto& token : tokens) {
            if (!current) return nullptr;
            
            if (current->is_object()) {
                auto obj = current->as_object();
                if (obj->has_key(token)) {
                    current = obj->get(token);
                } else {
                    return nullptr;
                }
            } else if (current->is_array()) {
                auto arr = current->as_array();
                try {
                    size_t index = std::stoull(token);
                    if (index < arr->size()) {
                        current = arr->at(index);
                    } else {
                        return nullptr;
                    }
                } catch (...) {
                    return nullptr;
                }
            } else {
                return nullptr;
            }
        }
        
        return current;
    }
    
    static bool set(JsonValuePtr& root, const std::string& path, JsonValuePtr value) {
        if (!root || !value) return false;
        
        std::vector<std::string> tokens = split_path(path);
        if (tokens.empty()) return false;
        
        std::string last_token = tokens.back();
        tokens.pop_back();
        
        JsonValuePtr current = root;
        for (const auto& token : tokens) {
            if (!current) return false;
            
            if (current->is_object()) {
                auto obj = current->as_object();
                if (obj->has_key(token)) {
                    current = obj->get(token);
                } else {
                    return false;
                }
            } else if (current->is_array()) {
                auto arr = current->as_array();
                try {
                    size_t index = std::stoull(token);
                    if (index < arr->size()) {
                        current = arr->at(index);
                    } else {
                        return false;
                    }
                } catch (...) {
                    return false;
                }
            } else {
                return false;
            }
        }
        
        if (current->is_object()) {
            current->as_object()->set(last_token, value);
            return true;
        } else if (current->is_array()) {
            auto arr = current->as_array();
            try {
                size_t index = std::stoull(last_token);
                if (index < arr->size()) {
                    // Note: We can't modify array elements directly due to clone() semantics
                    return false;
                }
            } catch (...) {
                return false;
            }
        }
        
        return false;
    }
    
private:
    static std::vector<std::string> split_path(const std::string& path) {
        std::vector<std::string> tokens;
        std::string current;
        
        for (char c : path) {
            if (c == '.') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            } else {
                current += c;
            }
        }
        
        if (!current.empty()) {
            tokens.push_back(current);
        }
        
        return tokens;
    }
};

// JSON Merge operations
class JsonMerge {
public:
    static JsonValuePtr merge(const JsonValuePtr& base, const JsonValuePtr& overlay) {
        if (!base) return overlay->deep_clone();
        if (!overlay) return base->deep_clone();
        
        if (base->is_object() && overlay->is_object()) {
            auto result = std::make_unique<JsonObject>();
            auto base_obj = base->as_object();
            auto overlay_obj = overlay->as_object();
            
            // Copy all fields from base
            for (const auto& [key, value] : *base_obj) {
                result->set(key, value);
            }
            
            // Merge or add fields from overlay
            for (const auto& [key, value] : *overlay_obj) {
                if (result->has_key(key) && value->is_object() && result->get(key)->is_object()) {
                    // Recursive merge for objects
                    result->set(key, merge(result->get(key), value));
                } else {
                    // Overwrite with overlay value
                    result->set(key, value);
                }
            }
            
            return result;
        }
        
        // For non-objects, overlay wins
        return overlay->deep_clone();
    }
};

// JSON Validation
class JsonValidator {
public:
    struct ValidationResult {
        bool valid;
        std::string error_message;
        std::string error_path;
        
        ValidationResult() : valid(true) {}
        ValidationResult(const std::string& msg, const std::string& path)
            : valid(false), error_message(msg), error_path(path) {}
    };
    
    static ValidationResult validate(const JsonValuePtr& value, const JsonValuePtr& schema) {
        if (!value || !schema) {
            return ValidationResult("Invalid value or schema", "");
        }
        
        return validate_value(value, schema, "");
    }
    
private:
    static ValidationResult validate_value(const JsonValuePtr& value, const JsonValuePtr& schema, const std::string& path) {
        if (!schema->is_object()) {
            return ValidationResult("Schema must be an object", path);
        }
        
        auto schema_obj = schema->as_object();
        
        // Type validation
        if (schema_obj->has_key("type")) {
            auto type_value = schema_obj->get("type");
            if (type_value->is_string()) {
                auto type_str = type_value->as_string()->value();
                if (!validate_type(value, type_str)) {
                    return ValidationResult("Type mismatch: expected " + type_str, path);
                }
            }
        }
        
        // Required fields validation
        if (schema_obj->has_key("required") && value->is_object()) {
            auto required = schema_obj->get("required");
            if (required->is_array()) {
                auto required_array = required->as_array();
                auto obj = value->as_object();
                for (size_t i = 0; i < required_array->size(); ++i) {
                    auto req = required_array->at(i);
                    if (req->is_string()) {
                        auto req_str = req->as_string()->value();
                        if (!obj->has_key(req_str)) {
                            return ValidationResult("Missing required field: " + req_str, path);
                        }
                    }
                }
            }
        }
        
        // Properties validation
        if (schema_obj->has_key("properties") && value->is_object()) {
            auto properties = schema_obj->get("properties");
            if (properties->is_object()) {
                auto props_obj = properties->as_object();
                auto obj = value->as_object();
                
                for (const auto& [key, prop_schema] : *props_obj) {
                    if (obj->has_key(key)) {
                        auto result = validate_value(obj->get(key), prop_schema, path + "." + key);
                        if (!result.valid) {
                            return result;
                        }
                    }
                }
            }
        }
        
        return ValidationResult();
    }
    
    static bool validate_type(const JsonValuePtr& value, const std::string& type) {
        if (type == "null") return value->is_null();
        if (type == "boolean") return value->is_boolean();
        if (type == "number") return value->is_number();
        if (type == "string") return value->is_string();
        if (type == "array") return value->is_array();
        if (type == "object") return value->is_object();
        return false;
    }
};

} // namespace jasson

#endif // JSON_EXTENSIONS_HPP
