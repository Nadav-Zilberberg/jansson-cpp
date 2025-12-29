#include "jansson.h"
#include <algorithm>
#include <cctype>
#include <sstream>

#ifdef __cplusplus

namespace jansson {

// JsonString implementation
std::string JsonString::to_string() const {
    std::ostringstream oss;
    oss << "\"";
    for (char c : value_) {
        switch (c) {
            case '"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if (c >= 0x20 && c <= 0x7E) {
                    oss << c;
                } else {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (unsigned char)c;
                }
        }
    }
    oss << "\"";
    return oss.str();
}

json_t* JsonString::to_c_json() const {
    return json_string(value_.c_str());
}

JsonValuePtr JsonString::clone() const {
    return std::make_shared<JsonString>(value_);
}

// JsonInteger implementation
std::string JsonInteger::to_string() const {
    return std::to_string(value_);
}

json_t* JsonInteger::to_c_json() const {
    return json_integer(value_);
}

JsonValuePtr JsonInteger::clone() const {
    return std::make_shared<JsonInteger>(value_);
}

// JsonReal implementation
std::string JsonReal::to_string() const {
    std::ostringstream oss;
    oss << std::setprecision(15) << value_;
    std::string result = oss.str();
    // Remove trailing zeros
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') {
        result += '0';
    }
    return result;
}

json_t* JsonReal::to_c_json() const {
    return json_real(value_);
}

JsonValuePtr JsonReal::clone() const {
    return std::make_shared<JsonReal>(value_);
}

// JsonBoolean implementation
std::string JsonBoolean::to_string() const {
    return value_ ? "true" : "false";
}

json_t* JsonBoolean::to_c_json() const {
    return json_boolean(value_);
}

JsonValuePtr JsonBoolean::clone() const {
    return std::make_shared<JsonBoolean>(value_);
}

// JsonNull implementation
std::string JsonNull::to_string() const {
    return "null";
}

json_t* JsonNull::to_c_json() const {
    return json_null();
}

JsonValuePtr JsonNull::clone() const {
    return std::make_shared<JsonNull>();
}

// JsonArray implementation
void JsonArray::insert(size_t index, JsonValuePtr value) {
    if (index > elements_.size()) {
        throw JsonOutOfRangeException("Array index out of range");
    }
    elements_.insert(elements_.begin() + index, value);
}

void JsonArray::remove(size_t index) {
    if (index >= elements_.size()) {
        throw JsonOutOfRangeException("Array index out of range");
    }
    elements_.erase(elements_.begin() + index);
}

std::string JsonArray::to_string() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) oss << ",";
        oss << elements_[i]->to_string();
    }
    oss << "]";
    return oss.str();
}

json_t* JsonArray::to_c_json() const {
    json_t* array = json_array();
    for (const auto& element : elements_) {
        json_array_append(array, element->to_c_json());
    }
    return array;
}

JsonValuePtr JsonArray::clone() const {
    auto result = std::make_shared<JsonArray>();
    for (const auto& element : elements_) {
        result->append(element->clone());
    }
    return result;
}

// JsonObject implementation
std::string JsonObject::to_string() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& pair : members_) {
        if (!first) oss << ",";
        first = false;
        oss << "\"" << pair.first << "\":" << pair.second->to_string();
    }
    oss << "}";
    return oss.str();
}

json_t* JsonObject::to_c_json() const {
    json_t* object = json_object();
    for (const auto& pair : members_) {
        json_object_set(object, pair.first.c_str(), pair.second->to_c_json());
    }
    return object;
}

JsonValuePtr JsonObject::clone() const {
    auto result = std::make_shared<JsonObject>();
    for (const auto& pair : members_) {
        result->set(pair.first, pair.second->clone());
    }
    return result;
}

// Parsing and serialization functions
JsonValuePtr parse(const std::string& json_str) {
    json_error_t error;
    json_t* c_json = json_loads(json_str.c_str(), 0, &error);
    if (!c_json) {
        throw JsonParseException(std::string("Parse error: ") + error.text);
    }
    
    // Convert C JSON to C++ JSON (simplified implementation)
    // In a real implementation, this would recursively convert the entire structure
    JsonValuePtr result;
    
    switch (json_typeof(c_json)) {
        case JSON_NULL:
            result = make_null();
            break;
        case JSON_TRUE:
            result = make_boolean(true);
            break;
        case JSON_FALSE:
            result = make_boolean(false);
            break;
        case JSON_INTEGER:
            result = make_integer(json_integer_value(c_json));
            break;
        case JSON_REAL:
            result = make_real(json_real_value(c_json));
            break;
        case JSON_STRING:
            result = make_string(json_string_value(c_json));
            break;
        case JSON_ARRAY: {
            auto array = std::make_shared<JsonArray>();
            size_t size = json_array_size(c_json);
            for (size_t i = 0; i < size; ++i) {
                // This is a simplified implementation
                // In reality, we'd need to recursively parse each element
                array->append(make_null());
            }
            result = array;
            break;
        }
        case JSON_OBJECT: {
            auto object = std::make_shared<JsonObject>();
            // This is a simplified implementation
            // In reality, we'd need to recursively parse each member
            result = object;
            break;
        }
        default:
            json_decref(c_json);
            throw JsonParseException("Unknown JSON type");
    }
    
    json_decref(c_json);
    return result;
}

JsonValuePtr parse_file(const std::string& filename) {
    json_error_t error;
    json_t* c_json = json_load_file(filename.c_str(), 0, &error);
    if (!c_json) {
        throw JsonParseException(std::string("Parse error: ") + error.text);
    }
    
    // Convert C JSON to C++ JSON (simplified implementation)
    JsonValuePtr result = make_null(); // Placeholder
    
    json_decref(c_json);
    return result;
}

std::string stringify(const JsonValuePtr& value, bool pretty) {
    if (!value) {
        return "null";
    }
    return value->to_string();
}

} // namespace jansson

#endif // __cplusplus
