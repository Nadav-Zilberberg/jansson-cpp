#include "jansson.h"
#include "json_factory.hpp"
#include "json_value.hpp"
#include <memory>
#include <string>

// Global storage for C interface objects
static std::shared_ptr<jasson::JsonValue> g_c_interface_objects[1000];
static int g_next_object_id = 0;

// Helper function to convert C++ object to C interface
static json_t* create_c_interface(std::shared_ptr<jasson::JsonValue> cpp_obj) {
    if (g_next_object_id >= 1000) {
        return nullptr; // Too many objects
    }
    int id = g_next_object_id++;
    g_c_interface_objects[id] = cpp_obj;
    return reinterpret_cast<json_t*>(id + 1); // +1 to avoid 0
}

// Helper function to get C++ object from C interface
static jasson::JsonValue* get_cpp_object(json_t* obj) {
    if (!obj) return nullptr;
    int id = reinterpret_cast<intptr_t>(obj) - 1;
    if (id < 0 || id >= g_next_object_id) return nullptr;
    return g_c_interface_objects[id].get();
}

extern "C" {

JANSSON_API json_t* json_object() {
    return create_c_interface(jasson::jasson_object());
}

JANSSON_API json_t* json_array() {
    return create_c_interface(jasson::jasson_array());
}

JANSSON_API json_t* json_string(const char* value) {
    return create_c_interface(jasson::jasson_string(value));
}

JANSSON_API json_t* json_integer(json_int_t value) {
    return create_c_interface(jasson::jasson_integer(value));
}

JANSSON_API json_t* json_real(json_real_t value) {
    return create_c_interface(jasson::jasson_real(value));
}

JANSSON_API json_t* json_true() {
    return create_c_interface(jasson::jasson_true());
}

JANSSON_API json_t* json_false() {
    return create_c_interface(jasson::jasson_false());
}

JANSSON_API json_t* json_null() {
    return create_c_interface(jasson::jasson_null());
}

JANSSON_API void json_incref(json_t* json) {
    // For now, just a no-op since we're using shared_ptr
}

JANSSON_API void json_decref(json_t* json) {
    if (!json) return;
    int id = reinterpret_cast<intptr_t>(json) - 1;
    if (id < 0 || id >= g_next_object_id) return;
    g_c_interface_objects[id].reset();
}

} // extern "C"
