#ifndef JASSON_JSON_CONVENIENCE_HPP
#define JASSON_JSON_CONVENIENCE_HPP

#include "json_value.hpp"
#include "json_types.hpp"
#include "json_extensions.hpp"
#include "json_loader.hpp"
#include "json_dumper.hpp"
#include "type_traits.hpp"
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <string_view>
#include <concepts>
#include <type_traits>

namespace jasson {

// Convenience functions for creating JSON values - return unique_ptr to work with existing API
inline std::unique_ptr<JsonObject> jasson_object() {
    return std::make_unique<JsonObject>();
}

inline std::unique_ptr<JsonArray> jasson_array() {
    return std::make_unique<JsonArray>();
}

inline std::unique_ptr<JsonString> jasson_string(const std::string& value) {
    return std::make_unique<JsonString>(value);
}

inline std::unique_ptr<JsonString> jasson_string(const char* value) {
    return std::make_unique<JsonString>(std::string(value));
}

inline std::unique_ptr<JsonInteger> jasson_integer(json_int_t value) {
    return std::make_unique<JsonInteger>(value);
}

inline std::unique_ptr<JsonReal> jasson_real(double value) {
    return std::make_unique<JsonReal>(value);
}

inline std::unique_ptr<JsonBoolean> jasson_true() {
    return std::make_unique<JsonBoolean>(true);
}

inline std::unique_ptr<JsonBoolean> jasson_false() {
    return std::make_unique<JsonBoolean>(false);
}

inline std::unique_ptr<JsonNull> jasson_null() {
    return std::make_unique<JsonNull>();
}

// Also provide shared_ptr versions for the convenience API
inline JsonObjectPtr json_object() {
    return std::make_shared<JsonObject>();
}

inline JsonArrayPtr json_array() {
    return std::make_shared<JsonArray>();
}

inline JsonStringPtr json_string(const std::string& value) {
    return std::make_shared<JsonString>(value);
}

inline JsonStringPtr json_string(const char* value) {
    return std::make_shared<JsonString>(std::string(value));
}

inline JsonIntegerPtr json_integer(json_int_t value) {
    return std::make_shared<JsonInteger>(value);
}

inline JsonRealPtr json_real(double value) {
    return std::make_shared<JsonReal>(value);
}

inline JsonBooleanPtr json_true() {
    return std::make_shared<JsonBoolean>(true);
}

inline JsonBooleanPtr json_false() {
    return std::make_shared<JsonBoolean>(false);
}

inline JsonNullPtr json_null() {
    return std::make_shared<JsonNull>();
}

// C++20 concepts for JSON compatibility
template<typename T>
concept JsonSerializable = requires(const T& t) {
    { to_json(t) } -> std::convertible_to<JsonValuePtr>;
};

template<typename T>
concept JsonDeserializable = requires(const JsonValue& j) {
    { from_json<T>(j) } -> std::convertible_to<T>;
};

template<typename T>
concept JsonCompatible = JsonSerializable<T> && JsonDeserializable<T>;

// Type traits for JSON compatibility
template<typename T>
struct is_json_serializable : std::false_type {};

template<typename T>
struct is_json_deserializable : std::false_type {};

template<typename T>
struct is_json_compatible : std::false_type {};

// Specializations for built-in types
template<>
struct is_json_serializable<bool> : std::true_type {};

template<>
struct is_json_serializable<int> : std::true_type {};

template<>
struct is_json_serializable<long> : std::true_type {};

template<>
struct is_json_serializable<long long> : std::true_type {};

template<>
struct is_json_serializable<unsigned int> : std::true_type {};

template<>
struct is_json_serializable<unsigned long> : std::true_type {};

template<>
struct is_json_serializable<unsigned long long> : std::true_type {};

template<>
struct is_json_serializable<float> : std::true_type {};

template<>
struct is_json_serializable<double> : std::true_type {};

template<>
struct is_json_serializable<std::string> : std::true_type {};

template<>
struct is_json_serializable<const char*> : std::true_type {};

template<>
struct is_json_deserializable<bool> : std::true_type {};

template<>
struct is_json_deserializable<int> : std::true_type {};

template<>
struct is_json_deserializable<long> : std::true_type {};

template<>
struct is_json_deserializable<long long> : std::true_type {};

template<>
struct is_json_deserializable<unsigned int> : std::true_type {};

template<>
struct is_json_deserializable<unsigned long> : std::true_type {};

template<>
struct is_json_deserializable<unsigned long long> : std::true_type {};

template<>
struct is_json_deserializable<float> : std::true_type {};

template<>
struct is_json_deserializable<double> : std::true_type {};

template<>
struct is_json_deserializable<std::string> : std::true_type {};

// Helper variable templates
template<typename T>
inline constexpr bool is_json_serializable_v = is_json_serializable<T>::value;

template<typename T>
inline constexpr bool is_json_deserializable_v = is_json_deserializable<T>::value;

template<typename T>
inline constexpr bool is_json_compatible_v = is_json_compatible<T>::value;

// Default to_json implementations for built-in types
inline JsonValuePtr to_json(bool value) {
    return value ? json_true() : json_false();
}

inline JsonValuePtr to_json(int value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(long long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(unsigned int value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(unsigned long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(unsigned long long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonValuePtr to_json(float value) {
    return json_real(static_cast<double>(value));
}

inline JsonValuePtr to_json(double value) {
    return json_real(value);
}

inline JsonValuePtr to_json(const std::string& value) {
    return json_string(value);
}

inline JsonValuePtr to_json(const char* value) {
    return json_string(value);
}

inline JsonValuePtr to_json(std::string_view value) {
    return json_string(std::string(value));
}

// Default from_json implementations for built-in types
inline bool from_json(const JsonValue& j, bool& value) {
    if (!j.is_boolean()) return false;
    value = static_cast<const JsonBoolean&>(j).value();
    return true;
}

inline bool from_json(const JsonValue& j, int& value) {
    if (!j.is_integer()) return false;
    value = static_cast<int>(static_cast<const JsonInteger&>(j).value());
    return true;
}

inline bool from_json(const JsonValue& j, long& value) {
    if (!j.is_integer()) return false;
    value = static_cast<long>(static_cast<const JsonInteger&>(j).value());
    return true;
}

inline bool from_json(const JsonValue& j, long long& value) {
    if (!j.is_integer()) return false;
    value = static_cast<const JsonInteger&>(j).value();
    return true;
}

inline bool from_json(const JsonValue& j, unsigned int& value) {
    if (!j.is_integer()) return false;
    json_int_t v = static_cast<const JsonInteger&>(j).value();
    if (v < 0) return false;
    value = static_cast<unsigned int>(v);
    return true;
}

inline bool from_json(const JsonValue& j, unsigned long& value) {
    if (!j.is_integer()) return false;
    json_int_t v = static_cast<const JsonInteger&>(j).value();
    if (v < 0) return false;
    value = static_cast<unsigned long>(v);
    return true;
}

inline bool from_json(const JsonValue& j, unsigned long long& value) {
    if (!j.is_integer()) return false;
    json_int_t v = static_cast<const JsonInteger&>(j).value();
    if (v < 0) return false;
    value = static_cast<unsigned long long>(v);
    return true;
}

inline bool from_json(const JsonValue& j, float& value) {
    if (!j.is_real()) return false;
    value = static_cast<float>(static_cast<const JsonReal&>(j).value());
    return true;
}

inline bool from_json(const JsonValue& j, double& value) {
    if (!j.is_real()) return false;
    value = static_cast<const JsonReal&>(j).value();
    return true;
}

inline bool from_json(const JsonValue& j, std::string& value) {
    if (!j.is_string()) return false;
    value = static_cast<const JsonString&>(j).value();
    return true;
}

// Template versions for type safety
template<typename T>
std::enable_if_t<is_json_deserializable_v<T>, T> from_json_value(const JsonValue& j) {
    T value{};
    if (!from_json(j, value)) {
        throw std::runtime_error("Type mismatch in JSON conversion");
    }
    return value;
}

// Convenience parse and stringify functions
inline JsonValuePtr parse(std::string_view json_str, JsonLoadFlags flags = JsonLoadFlags::NONE) {
    JsonLoader loader(flags);
    return loader.load_from_string(std::string(json_str));
}

inline JsonValuePtr parse(const std::string& json_str, JsonLoadFlags flags = JsonLoadFlags::NONE) {
    JsonLoader loader(flags);
    return loader.load_from_string(json_str);
}

inline JsonValuePtr parse(const char* json_str, JsonLoadFlags flags = JsonLoadFlags::NONE) {
    JsonLoader loader(flags);
    return loader.load_from_string(json_str);
}

inline std::string stringify(const JsonValue& json, JsonDumpFormat format = JsonDumpFormat::COMPACT) {
    return json_dump(json, format);
}

// Template versions for custom types
template<JsonSerializable T>
JsonValuePtr to_json(const T& value) {
    return value.to_json();
}

template<JsonDeserializable T>
T from_json_value(const JsonValue& j) {
    return T::from_json(j);
}

// Support for containers
template<typename T>
JsonValuePtr to_json(const std::vector<T>& vec) {
    auto arr = json_array();
    for (const auto& item : vec) {
        arr->append(to_json(item)->clone());
    }
    return arr;
}

template<typename T>
bool from_json(const JsonValue& j, std::vector<T>& vec) {
    if (!j.is_array()) return false;
    const auto& arr = static_cast<const JsonArray&>(j);
    vec.clear();
    vec.reserve(arr.size());
    for (size_t i = 0; i < arr.size(); ++i) {
        T item{};
        if (!from_json(*arr.at(i), item)) return false;
        vec.push_back(std::move(item));
    }
    return true;
}

template<typename T>
std::enable_if_t<is_json_deserializable_v<T>, std::vector<T>> from_json_vector(const JsonValue& j) {
    if (!j.is_array()) {
        throw std::runtime_error("Expected JSON array");
    }
    const auto& arr = static_cast<const JsonArray&>(j);
    std::vector<T> vec;
    vec.reserve(arr.size());
    for (size_t i = 0; i < arr.size(); ++i) {
        vec.push_back(from_json_value<T>(*arr.at(i)));
    }
    return vec;
}

// Support for maps
template<typename V>
JsonValuePtr to_json(const std::unordered_map<std::string, V>& map) {
    auto obj = json_object();
    for (const auto& [key, value] : map) {
        obj->set(key, to_json(value)->clone());
    }
    return obj;
}

template<typename V>
bool from_json(const JsonValue& j, std::unordered_map<std::string, V>& map) {
    if (!j.is_object()) return false;
    const auto& obj = static_cast<const JsonObject&>(j);
    map.clear();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        V value{};
        if (!from_json(*it->second, value)) return false;
        map[it->first] = std::move(value);
    }
    return true;
}

template<typename V>
std::enable_if_t<is_json_deserializable_v<V>, std::unordered_map<std::string, V>> from_json_map(const JsonValue& j) {
    if (!j.is_object()) {
        throw std::runtime_error("Expected JSON object");
    }
    const auto& obj = static_cast<const JsonObject&>(j);
    std::unordered_map<std::string, V> map;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        map[it->first] = from_json_value<V>(*it->second);
    }
    return map;
}

// Support for optional
template<typename T>
JsonValuePtr to_json(const std::optional<T>& opt) {
    if (opt.has_value()) {
        return to_json(opt.value());
    }
    return json_null();
}

template<typename T>
bool from_json(const JsonValue& j, std::optional<T>& opt) {
    if (j.is_null()) {
        opt.reset();
        return true;
    }
    T value{};
    if (!from_json(j, value)) return false;
    opt = std::move(value);
    return true;
}

template<typename T>
std::enable_if_t<is_json_deserializable_v<T>, std::optional<T>> from_json_optional(const JsonValue& j) {
    if (j.is_null()) {
        return std::nullopt;
    }
    return from_json_value<T>(j);
}

// Support for variant
template<typename... Ts>
JsonValuePtr to_json(const std::variant<Ts...>& var) {
    return std::visit([](const auto& value) { return to_json(value); }, var);
}

// Fluent API for JSON construction
class JsonBuilder {
public:
    JsonBuilder() = default;
    
    // Object construction
    JsonBuilder& object() {
        current_ = json_object();
        return *this;
    }
    
    JsonBuilder& array() {
        current_ = json_array();
        return *this;
    }
    
    // Add key-value pair to object
    JsonBuilder& add(const std::string& key, JsonValuePtr value) {
        if (!current_ || !current_->is_object()) {
            throw std::runtime_error("Current value is not an object");
        }
        // Convert shared_ptr to unique_ptr for the set method
        std::static_pointer_cast<JsonObject>(current_)->set(key, value->clone());
        return *this;
    }
    
    // Add key-value pair with convenience constructors
    JsonBuilder& add(const std::string& key, const std::string& value) {
        return add(key, json_string(value));
    }
    
    JsonBuilder& add(const std::string& key, const char* value) {
        return add(key, json_string(value));
    }
    
    JsonBuilder& add(const std::string& key, json_int_t value) {
        return add(key, json_integer(value));
    }
    
    JsonBuilder& add(const std::string& key, double value) {
        return add(key, json_real(value));
    }
    
    JsonBuilder& add(const std::string& key, bool value) {
        return add(key, value ? json_true() : json_false());
    }
    
    // Append to array
    JsonBuilder& append(JsonValuePtr value) {
        if (!current_ || !current_->is_array()) {
            throw std::runtime_error("Current value is not an array");
        }
        // Convert shared_ptr to unique_ptr for the append method
        std::static_pointer_cast<JsonArray>(current_)->append(value->clone());
        return *this;
    }
    
    // Append with convenience constructors
    JsonBuilder& append(const std::string& value) {
        return append(json_string(value));
    }
    
    JsonBuilder& append(const char* value) {
        return append(json_string(value));
    }
    
    JsonBuilder& append(json_int_t value) {
        return append(json_integer(value));
    }
    
    JsonBuilder& append(double value) {
        return append(json_real(value));
    }
    
    JsonBuilder& append(bool value) {
        return append(value ? json_true() : json_false());
    }
    
    // Range-based constructors
    template<typename Container>
    JsonBuilder& from_container(const Container& container) {
        array();
        for (const auto& item : container) {
            append(item);
        }
        return *this;
    }
    
    // Initializer list support for objects
    JsonBuilder& from_pairs(std::initializer_list<std::pair<std::string, JsonValuePtr>> pairs) {
        object();
        for (const auto& [key, value] : pairs) {
            add(key, value);
        }
        return *this;
    }
    
    // Initializer list support for arrays
    JsonBuilder& from_values(std::initializer_list<JsonValuePtr> values) {
        array();
        for (const auto& value : values) {
            append(value);
        }
        return *this;
    }
    
    // Get the result
    JsonValuePtr build() {
        return current_;
    }
    
    // Implicit conversion
    operator JsonValuePtr() {
        return current_;
    }
    
private:
    JsonValuePtr current_;
};

// Convenience builder function
inline JsonBuilder json_builder() {
    return JsonBuilder();
}

// Operator overloading for intuitive syntax
inline JsonStringPtr operator""_json(const char* str, size_t len) {
    return json_string(std::string(str, len));
}

inline JsonIntegerPtr operator""_json(unsigned long long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonRealPtr operator""_json(long double value) {
    return json_real(static_cast<double>(value));
}

// Modern iterator support
class JsonIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = JsonValuePtr;
    using difference_type = std::ptrdiff_t;
    using pointer = JsonValuePtr*;
    using reference = JsonValuePtr&;
    
    JsonIterator() = default;
    
    // Array iterator
    explicit JsonIterator(const JsonArrayPtr& array) 
        : array_(array), is_array_(true), array_index_(0) {}
    
    // Object iterator
    explicit JsonIterator(const JsonObjectPtr& object)
        : object_(object), is_array_(false), object_it_(object->begin()) {}
    
    reference operator*() {
        if (is_array_) {
            current_value_ = array_->at(array_index_)->clone();
        } else {
            current_value_ = object_it_->second->clone();
        }
        return current_value_;
    }
    
    pointer operator->() {
        if (is_array_) {
            current_value_ = array_->at(array_index_)->clone();
        } else {
            current_value_ = object_it_->second->clone();
        }
        return &current_value_;
    }
    
    JsonIterator& operator++() {
        if (is_array_) {
            ++array_index_;
        } else {
            ++object_it_;
        }
        return *this;
    }
    
    JsonIterator operator++(int) {
        JsonIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const JsonIterator& other) const {
        if (is_array_ != other.is_array_) return false;
        if (is_array_) {
            return array_index_ == other.array_index_;
        } else {
            return object_it_ == other.object_it_;
        }
    }
    
    bool operator!=(const JsonIterator& other) const {
        return !(*this == other);
    }
    
    // Get key for object iteration
    const std::string& key() const {
        if (is_array_) {
            throw std::runtime_error("Cannot get key from array iterator");
        }
        return object_it_->first;
    }
    
private:
    JsonArrayPtr array_;
    JsonObjectPtr object_;
    bool is_array_ = false;
    size_t array_index_ = 0;
    std::unordered_map<std::string, std::unique_ptr<JsonValue>>::const_iterator object_it_;
    mutable JsonValuePtr current_value_;
};

// Range-based iteration support
inline JsonIterator json_begin(const JsonValuePtr& value) {
    if (value->is_array()) {
        return JsonIterator(std::static_pointer_cast<JsonArray>(value));
    } else if (value->is_object()) {
        return JsonIterator(std::static_pointer_cast<JsonObject>(value));
    } else {
        throw std::runtime_error("Value is not iterable");
    }
}

inline JsonIterator json_end(const JsonValuePtr& value) {
    return JsonIterator(); // Default constructor creates end iterator
}

} // namespace jasson

#endif // JASSON_JSON_CONVENIENCE_HPP
