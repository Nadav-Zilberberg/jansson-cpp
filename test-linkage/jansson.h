#ifndef JANSSON_H
#define JANSSON_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Version */
#define JANSSON_MAJOR_VERSION  2
#define JANSSON_MINOR_VERSION  14
#define JANSSON_MICRO_VERSION  0

/* Version as a 3-byte hex number, e.g. 0x010201 == 1.2.1. Use this
   for numeric comparisons, e.g. #if JANSSON_VERSION_HEX >= ... */
#define JANSSON_VERSION_HEX  ((JANSSON_MAJOR_VERSION << 16) |   \
                              (JANSSON_MINOR_VERSION << 8)  |   \
                              (JANSSON_MICRO_VERSION << 0))

/* If __GNUC__ is defined, use the visibility attribute to control symbol
   export. Otherwise, use Windows DLL export/import. */
#if defined(__GNUC__) && (__GNUC__ >= 4)
  #define JANSSON_API __attribute__((visibility("default")))
#elif defined(_WIN32) || defined(__CYGWIN__)
  #ifdef DLL_EXPORT
    #define JANSSON_API __declspec(dllexport)
  #else
    #define JANSSON_API __declspec(dllimport)
  #endif
#else
  #define JANSSON_API
#endif

/* Types */
typedef long long json_int_t;
typedef double json_real_t;

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INTEGER,
    JSON_REAL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
} json_type;

/* Forward declarations */
typedef struct json_t json_t;
typedef struct json_error_t json_error_t;

/* Memory allocation functions */
typedef void* (*json_malloc_t)(size_t);
typedef void* (*json_realloc_t)(void*, size_t);
typedef void (*json_free_t)(void*);

/* Error handling */
struct json_error_t {
    int line;
    int column;
    int position;
    char source[80];
    char text[160];
};

/* Configuration */
typedef struct {
    json_malloc_t malloc_fn;
    json_realloc_t realloc_fn;
    json_free_t free_fn;
    size_t gc_threshold;
    int gc_enabled;
} json_config_t;

/* Core functions */
JANSSON_API void json_object_seed(size_t seed);
JANSSON_API json_t* json_null(void);
JANSSON_API json_t* json_true(void);
JANSSON_API json_t* json_false(void);
JANSSON_API json_t* json_boolean(int value);
JANSSON_API json_t* json_integer(json_int_t value);
JANSSON_API json_t* json_real(json_real_t value);
JANSSON_API json_t* json_string(const char* value);
JANSSON_API json_t* json_stringn(const char* value, size_t len);
JANSSON_API json_t* json_array(void);
JANSSON_API json_t* json_object(void);

/* Reference counting */
JANSSON_API json_t* json_incref(json_t* json);
JANSSON_API void json_decref(json_t* json);
JANSSON_API int json_equal(json_t* json1, json_t* json2);

/* Array functions */
JANSSON_API size_t json_array_size(const json_t* array);
JANSSON_API json_t* json_array_get(const json_t* array, size_t index);
JANSSON_API int json_array_append(json_t* array, json_t* item);
JANSSON_API int json_array_insert(json_t* array, size_t index, json_t* item);
JANSSON_API int json_array_remove(json_t* array, size_t index);
JANSSON_API int json_array_clear(json_t* array);
JANSSON_API int json_array_extend(json_t* array, json_t* other);

/* Object functions */
JANSSON_API size_t json_object_size(const json_t* object);
JANSSON_API json_t* json_object_get(const json_t* object, const char* key);
JANSSON_API int json_object_set(json_t* object, const char* key, json_t* value);
JANSSON_API int json_object_set_new(json_t* object, const char* key, json_t* value);
JANSSON_API int json_object_del(json_t* object, const char* key);
JANSSON_API int json_object_clear(json_t* object);
JANSSON_API int json_object_update(json_t* object, json_t* other);
JANSSON_API int json_object_update_existing(json_t* object, json_t* other);
JANSSON_API int json_object_update_missing(json_t* object, json_t* other);

/* Serialization */
JANSSON_API char* json_dumps(const json_t* json, size_t flags);
JANSSON_API int json_dumpf(const json_t* json, FILE* output, size_t flags);
JANSSON_API int json_dump_file(const json_t* json, const char* path, size_t flags);
JANSSON_API json_t* json_loads(const char* input, size_t flags, json_error_t* error);
JANSSON_API json_t* json_loadf(FILE* input, size_t flags, json_error_t* error);
JANSSON_API json_t* json_load_file(const char* path, size_t flags, json_error_t* error);

/* Utility functions */
JANSSON_API json_type json_typeof(const json_t* json);
JANSSON_API json_int_t json_integer_value(const json_t* integer);
JANSSON_API json_real_t json_real_value(const json_t* real);
JANSSON_API double json_number_value(const json_t* json);
JANSSON_API const char* json_string_value(const json_t* string);
JANSSON_API size_t json_string_length(const json_t* string);

/* Configuration functions */
JANSSON_API void json_get_config(json_config_t* config);
JANSSON_API void json_set_config(const json_config_t* config);

#ifdef __cplusplus
}
#endif

/* C++ specific features */
#ifdef __cplusplus
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace jansson {

// Forward declarations
class JsonValue;
class JsonString;
class JsonInteger;
class JsonReal;
class JsonBoolean;
class JsonNull;
class JsonArray;
class JsonObject;
class JsonBuilder;
class JsonException;

// Type aliases
using JsonValuePtr = std::shared_ptr<JsonValue>;
using json_int_t = int64_t;
using json_real_t = double;

// Exception classes
class JsonException : public std::runtime_error {
public:
    explicit JsonException(const std::string& message) : std::runtime_error(message) {}
    explicit JsonException(const char* message) : std::runtime_error(message) {}
};

class JsonTypeException : public JsonException {
public:
    explicit JsonTypeException(const std::string& message) : JsonException(message) {}
};

class JsonParseException : public JsonException {
public:
    explicit JsonParseException(const std::string& message) : JsonException(message) {}
};

class JsonOutOfRangeException : public JsonException {
public:
    explicit JsonOutOfRangeException(const std::string& message) : JsonException(message) {}
};

// Base JSON value class
class JsonValue {
public:
    virtual ~JsonValue() = default;
    
    // Type checking
    virtual bool is_string() const { return false; }
    virtual bool is_integer() const { return false; }
    virtual bool is_real() const { return false; }
    virtual bool is_number() const { return false; }
    virtual bool is_boolean() const { return false; }
    virtual bool is_null() const { return false; }
    virtual bool is_array() const { return false; }
    virtual bool is_object() const { return false; }
    
    // Type casting
    virtual JsonString* as_string() { return nullptr; }
    virtual JsonInteger* as_integer() { return nullptr; }
    virtual JsonReal* as_real() { return nullptr; }
    virtual JsonBoolean* as_boolean() { return nullptr; }
    virtual JsonNull* as_null() { return nullptr; }
    virtual JsonArray* as_array() { return nullptr; }
    virtual JsonObject* as_object() { return nullptr; }
    
    // Common interface methods
    virtual size_t size() const { return 0; }
    virtual bool empty() const { return true; }
    virtual void clear() {}
    
    // Array-specific methods (throw if not array)
    virtual void append(JsonValuePtr value) { throw JsonTypeException("Cannot append to non-array value"); }
    virtual void insert(size_t index, JsonValuePtr value) { throw JsonTypeException("Cannot insert into non-array value"); }
    virtual void remove(size_t index) { throw JsonTypeException("Cannot remove from non-array value"); }
    virtual JsonValuePtr at(size_t index) const { throw JsonTypeException("Cannot access index of non-array value"); }
    
    // Object-specific methods (throw if not object)
    virtual void set(const std::string& key, JsonValuePtr value) { throw JsonTypeException("Cannot set property on non-object value"); }
    virtual void remove(const std::string& key) { throw JsonTypeException("Cannot remove property from non-object value"); }
    virtual JsonValuePtr get(const std::string& key) const { throw JsonTypeException("Cannot get property of non-object value"); }
    virtual bool has_key(const std::string& key) const { return false; }
    virtual std::vector<std::string> keys() const { return {}; }
    
    // Serialization
    virtual std::string to_string() const = 0;
    virtual json_t* to_c_json() const = 0;
    
    // Clone
    virtual JsonValuePtr clone() const = 0;
};

// JSON string class
class JsonString : public JsonValue {
private:
    std::string value_;
    
public:
    explicit JsonString(const std::string& value) : value_(value) {}
    explicit JsonString(const char* value) : value_(value ? value : "") {}
    
    bool is_string() const override { return true; }
    JsonString* as_string() override { return this; }
    
    const std::string& value() const { return value_; }
    std::string& value() { return value_; }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON integer class
class JsonInteger : public JsonValue {
private:
    json_int_t value_;
    
public:
    explicit JsonInteger(json_int_t value) : value_(value) {}
    
    bool is_integer() const override { return true; }
    bool is_number() const override { return true; }
    JsonInteger* as_integer() override { return this; }
    
    json_int_t value() const { return value_; }
    void set_value(json_int_t value) { value_ = value; }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON real class
class JsonReal : public JsonValue {
private:
    json_real_t value_;
    
public:
    explicit JsonReal(json_real_t value) : value_(value) {}
    
    bool is_real() const override { return true; }
    bool is_number() const override { return true; }
    JsonReal* as_real() override { return this; }
    
    json_real_t value() const { return value_; }
    void set_value(json_real_t value) { value_ = value; }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON boolean class
class JsonBoolean : public JsonValue {
private:
    bool value_;
    
public:
    explicit JsonBoolean(bool value) : value_(value) {}
    
    bool is_boolean() const override { return true; }
    JsonBoolean* as_boolean() override { return this; }
    
    bool value() const { return value_; }
    void set_value(bool value) { value_ = value; }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON null class
class JsonNull : public JsonValue {
public:
    bool is_null() const override { return true; }
    JsonNull* as_null() override { return this; }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON array class
class JsonArray : public JsonValue {
private:
    std::vector<JsonValuePtr> elements_;
    
public:
    JsonArray() = default;
    explicit JsonArray(std::initializer_list<JsonValuePtr> elements) : elements_(elements) {}
    
    bool is_array() const override { return true; }
    JsonArray* as_array() override { return this; }
    
    size_t size() const override { return elements_.size(); }
    bool empty() const override { return elements_.empty(); }
    
    JsonValuePtr at(size_t index) const override {
        if (index >= elements_.size()) {
            throw JsonOutOfRangeException("Array index out of range");
        }
        return elements_[index];
    }
    
    JsonValuePtr operator[](size_t index) const { return at(index); }
    
    void append(JsonValuePtr value) override { elements_.push_back(value); }
    void insert(size_t index, JsonValuePtr value) override;
    void remove(size_t index) override;
    void clear() override { elements_.clear(); }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON object class
class JsonObject : public JsonValue {
private:
    std::map<std::string, JsonValuePtr> members_;
    
public:
    JsonObject() = default;
    
    bool is_object() const override { return true; }
    JsonObject* as_object() override { return this; }
    
    size_t size() const override { return members_.size(); }
    bool empty() const override { return members_.empty(); }
    bool has_key(const std::string& key) const override { return members_.find(key) != members_.end(); }
    
    JsonValuePtr get(const std::string& key) const override {
        auto it = members_.find(key);
        if (it == members_.end()) {
            throw JsonOutOfRangeException("Key not found: " + key);
        }
        return it->second;
    }
    
    JsonValuePtr operator[](const std::string& key) const { return get(key); }
    
    void set(const std::string& key, JsonValuePtr value) override { members_[key] = value; }
    void remove(const std::string& key) override { members_.erase(key); }
    void clear() override { members_.clear(); }
    
    std::vector<std::string> keys() const override {
        std::vector<std::string> result;
        for (const auto& pair : members_) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    std::string to_string() const override;
    json_t* to_c_json() const override;
    JsonValuePtr clone() const override;
};

// JSON Builder class for fluent interface
class JsonBuilder {
private:
    JsonValuePtr value_;
    
public:
    JsonBuilder() = default;
    explicit JsonBuilder(JsonValuePtr value) : value_(value) {}
    
    // Factory methods
    static JsonBuilder null() { return JsonBuilder(std::make_shared<JsonNull>()); }
    static JsonBuilder boolean(bool value) { return JsonBuilder(std::make_shared<JsonBoolean>(value)); }
    static JsonBuilder integer(json_int_t value) { return JsonBuilder(std::make_shared<JsonInteger>(value)); }
    static JsonBuilder real(json_real_t value) { return JsonBuilder(std::make_shared<JsonReal>(value)); }
    static JsonBuilder string(const std::string& value) { return JsonBuilder(std::make_shared<JsonString>(value)); }
    static JsonBuilder array() { return JsonBuilder(std::make_shared<JsonArray>()); }
    static JsonBuilder object() { return JsonBuilder(std::make_shared<JsonObject>()); }
    
    // Array operations
    JsonBuilder& append(JsonValuePtr value) {
        if (!value_ || !value_->is_array()) {
            throw JsonTypeException("Cannot append to non-array value");
        }
        value_->append(value);
        return *this;
    }
    
    JsonBuilder& append(const JsonBuilder& builder) {
        return append(builder.build());
    }
    
    // Object operations
    JsonBuilder& set(const std::string& key, JsonValuePtr value) {
        if (!value_ || !value_->is_object()) {
            throw JsonTypeException("Cannot set property on non-object value");
        }
        value_->set(key, value);
        return *this;
    }
    
    JsonBuilder& set(const std::string& key, const JsonBuilder& builder) {
        return set(key, builder.build());
    }
    
    // Build
    JsonValuePtr build() const {
        if (!value_) {
            throw JsonTypeException("No value built");
        }
        return value_;
    }
    
    // Conversion
    std::string to_string() const {
        return value_ ? value_->to_string() : "null";
    }
};

// Factory functions
inline JsonValuePtr make_null() { return std::make_shared<JsonNull>(); }
inline JsonValuePtr make_boolean(bool value) { return std::make_shared<JsonBoolean>(value); }
inline JsonValuePtr make_integer(json_int_t value) { return std::make_shared<JsonInteger>(value); }
inline JsonValuePtr make_real(json_real_t value) { return std::make_shared<JsonReal>(value); }
inline JsonValuePtr make_string(const std::string& value) { return std::make_shared<JsonString>(value); }
inline JsonValuePtr make_array() { return std::make_shared<JsonArray>(); }
inline JsonValuePtr make_object() { return std::make_shared<JsonObject>(); }

// Parsing functions
JsonValuePtr parse(const std::string& json_str);
JsonValuePtr parse_file(const std::string& filename);

// Serialization functions
std::string stringify(const JsonValuePtr& value, bool pretty = false);

// Literal operators for JSON creation
namespace literals {
    inline JsonValuePtr operator""_json(const char* str, size_t len) {
        return parse(std::string(str, len));
    }
    
    inline JsonValuePtr operator""_json_int(unsigned long long value) {
        return make_integer(static_cast<json_int_t>(value));
    }
    
    inline JsonValuePtr operator""_json_real(long double value) {
        return make_real(static_cast<json_real_t>(value));
    }
}

} // namespace jansson

#endif // __cplusplus

#endif // JANSSON_H
