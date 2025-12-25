#include "jansson.h"
#include "json_value.hpp"
#include "json_dumper.hpp"
#include "json_loader.hpp"
#include <cstring>
#include <stdexcept>
#include <memory>

// Global storage for C interface objects
static std::vector<std::shared_ptr<jasson::JsonValue>> g_c_interface_objects;
static int g_next_object_id = 0;

// Helper function to create C interface handle
static json_t* create_c_interface(std::shared_ptr<jasson::JsonValue> value) {
    if (!value) return nullptr;
    
    // Find empty slot or add new one
    int id = -1;
    for (int i = 0; i < g_next_object_id; ++i) {
        if (!g_c_interface_objects[i]) {
            id = i;
            break;
        }
    }
    
    if (id == -1) {
        id = g_next_object_id++;
        g_c_interface_objects.resize(g_next_object_id);
    }
    
    g_c_interface_objects[id] = value;
    return reinterpret_cast<json_t*>(id + 1);
}

// Helper function to get C++ object from handle
static std::shared_ptr<jasson::JsonValue> get_cpp_object(json_t* obj) {
    if (!obj) return nullptr;
    int id = reinterpret_cast<intptr_t>(obj) - 1;
    if (id < 0 || id >= g_next_object_id) return nullptr;
    return g_c_interface_objects[id];
}

extern "C" {

// Basic value creation
JANSSON_API json_t* json_null() {
    return create_c_interface(std::make_shared<jasson::JsonNull>());
}

JANSSON_API json_t* json_true() {
    return create_c_interface(std::make_shared<jasson::JsonBoolean>(true));
}

JANSSON_API json_t* json_false() {
    return create_c_interface(std::make_shared<jasson::JsonBoolean>(false));
}

JANSSON_API json_t* json_integer(json_int_t value) {
    return create_c_interface(std::make_shared<jasson::JsonInteger>(value));
}

JANSSON_API json_t* json_real(json_real_t value) {
    return create_c_interface(std::make_shared<jasson::JsonReal>(value));
}

JANSSON_API json_t* json_string(const char* value) {
    if (!value) return nullptr;
    return create_c_interface(std::make_shared<jasson::JsonString>(value));
}

JANSSON_API json_t* json_array() {
    return create_c_interface(std::make_shared<jasson::JsonArray>());
}

JANSSON_API json_t* json_object() {
    return create_c_interface(std::make_shared<jasson::JsonObject>());
}

// Reference counting
JANSSON_API void json_incref(json_t* json) {
    // Not implemented - objects are managed by shared_ptr
}

JANSSON_API void json_decref(json_t* json) {
    if (!json) return;
    int id = reinterpret_cast<intptr_t>(json) - 1;
    if (id < 0 || id >= g_next_object_id) return;
    g_c_interface_objects[id].reset();
}

// Type checking functions
JANSSON_API int json_is_object(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_object() ? 1 : 0;
}

JANSSON_API int json_is_array(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_array() ? 1 : 0;
}

JANSSON_API int json_is_string(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_string() ? 1 : 0;
}

JANSSON_API int json_is_integer(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_integer() ? 1 : 0;
}

JANSSON_API int json_is_real(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_real() ? 1 : 0;
}

JANSSON_API int json_is_number(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_number() ? 1 : 0;
}

JANSSON_API int json_is_true(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    if (!obj || !obj->is_boolean()) return 0;
    return obj->as_boolean()->value() ? 1 : 0;
}

JANSSON_API int json_is_false(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    if (!obj || !obj->is_boolean()) return 0;
    return !obj->as_boolean()->value() ? 1 : 0;
}

JANSSON_API int json_is_boolean(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_boolean() ? 1 : 0;
}

JANSSON_API int json_is_null(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj && obj->is_null() ? 1 : 0;
}

// Array functions
JANSSON_API json_t* json_array_get(const json_t* array, size_t index) {
    if (!array || !json_is_array(array)) return nullptr;
    auto arr = get_cpp_object(const_cast<json_t*>(array));
    if (!arr) return nullptr;
    auto array_obj = arr->as_array();
    if (!array_obj || index >= array_obj->size()) return nullptr;
    return create_c_interface(array_obj->at(index));
}

JANSSON_API int json_array_set(json_t* array, size_t index, json_t* value) {
    if (!array || !json_is_array(array) || !value) return -1;
    auto arr = get_cpp_object(array);
    auto val = get_cpp_object(value);
    if (!arr || !val) return -1;
    auto array_obj = arr->as_array();
    if (!array_obj || index >= array_obj->size()) return -1;
    array_obj->erase(index); array_obj->insert(index, val);
    return 0;
}

JANSSON_API int json_array_append(json_t* array, json_t* value) {
    if (!array || !json_is_array(array) || !value) return -1;
    auto arr = get_cpp_object(array);
    auto val = get_cpp_object(value);
    if (!arr || !val) return -1;
    auto array_obj = arr->as_array();
    if (!array_obj) return -1;
    array_obj->append(val);
    return 0;
}

JANSSON_API size_t json_array_size(const json_t* array) {
    if (!array || !json_is_array(array)) return 0;
    auto arr = get_cpp_object(const_cast<json_t*>(array));
    if (!arr) return 0;
    auto array_obj = arr->as_array();
    return array_obj ? array_obj->size() : 0;
}

// Object functions
JANSSON_API json_t* json_object_get(const json_t* object, const char* key) {
    if (!object || !json_is_object(object) || !key) return nullptr;
    auto obj = get_cpp_object(const_cast<json_t*>(object));
    if (!obj) return nullptr;
    auto object_obj = obj->as_object();
    if (!object_obj || !object_obj->has_key(key)) return nullptr;
    return create_c_interface(object_obj->get(key));
}

JANSSON_API int json_object_set(json_t* object, const char* key, json_t* value) {
    if (!object || !json_is_object(object) || !key || !value) return -1;
    auto obj = get_cpp_object(object);
    auto val = get_cpp_object(value);
    if (!obj || !val) return -1;
    auto object_obj = obj->as_object();
    if (!object_obj) return -1;
    object_obj->set(key, val);
    return 0;
}

JANSSON_API size_t json_object_size(const json_t* object) {
    if (!object || !json_is_object(object)) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(object));
    if (!obj) return 0;
    auto object_obj = obj->as_object();
    return object_obj ? object_obj->size() : 0;
}

// String functions
JANSSON_API const char* json_string_value(const json_t* string) {
    if (!string || !json_is_string(string)) return nullptr;
    auto obj = get_cpp_object(const_cast<json_t*>(string));
    if (!obj) return nullptr;
    auto string_obj = obj->as_string();
    if (!string_obj) return nullptr;
    return string_obj->value().c_str();
}

// Integer functions
JANSSON_API json_int_t json_integer_value(const json_t* integer) {
    if (!integer || !json_is_integer(integer)) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(integer));
    if (!obj) return 0;
    auto integer_obj = obj->as_integer();
    if (!integer_obj) return 0;
    return integer_obj->value();
}

// Real functions
JANSSON_API json_real_t json_real_value(const json_t* real) {
    if (!real || !json_is_real(real)) return 0.0;
    auto obj = get_cpp_object(const_cast<json_t*>(real));
    if (!obj) return 0.0;
    auto real_obj = obj->as_real();
    if (!real_obj) return 0.0;
    return real_obj->value();
}

// Reference counting
JANSSON_API size_t json_refcount(const json_t* json) {
    if (!json) return 0;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    return obj ? obj.use_count() : 0;
}

// Equality
JANSSON_API int json_equal(json_t* json1, json_t* json2) {
    if (!json1 || !json2) return 0;
    auto obj1 = get_cpp_object(json1);
    auto obj2 = get_cpp_object(json2);
    if (!obj1 || !obj2) return 0;
    return obj1->equals(*obj2) ? 1 : 0;
}

// Serialization
JANSSON_API char* json_dumps(const json_t* json, size_t flags) {
    if (!json) return nullptr;
    auto obj = get_cpp_object(const_cast<json_t*>(json));
    if (!obj) return nullptr;
    std::string result = jasson::JsonDumper::dumps(obj);
    char* buffer = (char*)malloc(result.length() + 1);
    if (!buffer) return nullptr;
    strcpy(buffer, result.c_str());
    return buffer;
}

// Deserialization
JANSSON_API json_t* json_loads(const char* input, size_t flags, json_error_t* error) {
    if (!input) return nullptr;
    try {
        auto value = jasson::JsonLoader::load(input);
        return create_c_interface(value);
    } catch (const std::exception& e) {
        if (error) {
            strncpy(error->text, e.what(), 160 - 1);  // JSON_ERROR_TEXT_LENGTH = 160
            error->text[160 - 1] = '\0';
        }
        return nullptr;
    }
}

} // extern "C"
