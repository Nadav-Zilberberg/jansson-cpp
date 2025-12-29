#include "jansson.h"
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

// Simple C interface implementation for testing purposes
// This is a minimal implementation that provides the basic C API

struct json_t {
    json_type type;
    union {
        json_int_t integer;
        json_real_t real;
        char* string;
        std::vector<json_t*>* array;
        std::map<std::string, json_t*>* object;
        bool boolean;
    } value;
};

// Global error structure for simplicity
static json_error_t g_error = {0, 0, 0, "", ""};

// Helper functions
static json_t* create_json(json_type type) {
    json_t* json = (json_t*)malloc(sizeof(json_t));
    if (json) {
        json->type = type;
        memset(&json->value, 0, sizeof(json->value));
    }
    return json;
}

// Basic value creation
JANSSON_API json_t* json_null() {
    return create_json(JSON_NULL);
}

JANSSON_API json_t* json_true() {
    json_t* json = create_json(JSON_TRUE);
    if (json) json->value.boolean = true;
    return json;
}

JANSSON_API json_t* json_false() {
    json_t* json = create_json(JSON_FALSE);
    if (json) json->value.boolean = false;
    return json;
}

JANSSON_API json_t* json_boolean(int value) {
    return value ? json_true() : json_false();
}

JANSSON_API json_t* json_integer(json_int_t value) {
    json_t* json = create_json(JSON_INTEGER);
    if (json) json->value.integer = value;
    return json;
}

JANSSON_API json_t* json_real(json_real_t value) {
    json_t* json = create_json(JSON_REAL);
    if (json) json->value.real = value;
    return json;
}

JANSSON_API json_t* json_string(const char* value) {
    if (!value) return nullptr;
    json_t* json = create_json(JSON_STRING);
    if (json) {
        json->value.string = (char*)malloc(strlen(value) + 1);
        if (json->value.string) {
            strcpy(json->value.string, value);
        } else {
            free(json);
            json = nullptr;
        }
    }
    return json;
}

JANSSON_API json_t* json_stringn(const char* value, size_t len) {
    if (!value) return nullptr;
    json_t* json = create_json(JSON_STRING);
    if (json) {
        json->value.string = (char*)malloc(len + 1);
        if (json->value.string) {
            strncpy(json->value.string, value, len);
            json->value.string[len] = '\0';
        } else {
            free(json);
            json = nullptr;
        }
    }
    return json;
}

JANSSON_API json_t* json_array(void) {
    json_t* json = create_json(JSON_ARRAY);
    if (json) {
        json->value.array = new std::vector<json_t*>();
        if (!json->value.array) {
            free(json);
            json = nullptr;
        }
    }
    return json;
}

JANSSON_API json_t* json_object(void) {
    json_t* json = create_json(JSON_OBJECT);
    if (json) {
        json->value.object = new std::map<std::string, json_t*>();
        if (!json->value.object) {
            free(json);
            json = nullptr;
        }
    }
    return json;
}

// Reference counting (simplified)
JANSSON_API json_t* json_incref(json_t* json) {
    return json; // For simplicity, no reference counting
}

JANSSON_API void json_decref(json_t* json) {
    if (!json) return;
    
    switch (json->type) {
        case JSON_STRING:
            free(json->value.string);
            break;
        case JSON_ARRAY: {
            if (json->value.array) {
                for (auto* item : *json->value.array) {
                    json_decref(item);
                }
                delete json->value.array;
            }
            break;
        }
        case JSON_OBJECT: {
            if (json->value.object) {
                for (auto& pair : *json->value.object) {
                    json_decref(pair.second);
                }
                delete json->value.object;
            }
            break;
        }
        default:
            break;
    }
    free(json);
}

JANSSON_API int json_equal(json_t* json1, json_t* json2) {
    if (!json1 || !json2) return json1 == json2;
    if (json1->type != json2->type) return 0;
    
    switch (json1->type) {
        case JSON_NULL:
            return 1;
        case JSON_TRUE:
        case JSON_FALSE:
            return json1->value.boolean == json2->value.boolean;
        case JSON_INTEGER:
            return json1->value.integer == json2->value.integer;
        case JSON_REAL:
            return json1->value.real == json2->value.real;
        case JSON_STRING:
            return strcmp(json1->value.string, json2->value.string) == 0;
        default:
            return 0; // Complex types not implemented for simplicity
    }
}

// Array functions
JANSSON_API size_t json_array_size(const json_t* array) {
    if (!array || array->type != JSON_ARRAY) return 0;
    return array->value.array ? array->value.array->size() : 0;
}

JANSSON_API json_t* json_array_get(const json_t* array, size_t index) {
    if (!array || array->type != JSON_ARRAY || !array->value.array) return nullptr;
    if (index >= array->value.array->size()) return nullptr;
    return (*array->value.array)[index];
}

JANSSON_API int json_array_append(json_t* array, json_t* item) {
    if (!array || array->type != JSON_ARRAY || !item) return -1;
    if (array->value.array) {
        array->value.array->push_back(item);
        return 0;
    }
    return -1;
}

JANSSON_API int json_array_insert(json_t* array, size_t index, json_t* item) {
    if (!array || array->type != JSON_ARRAY || !item) return -1;
    if (!array->value.array || index > array->value.array->size()) return -1;
    array->value.array->insert(array->value.array->begin() + index, item);
    return 0;
}

JANSSON_API int json_array_remove(json_t* array, size_t index) {
    if (!array || array->type != JSON_ARRAY || !array->value.array) return -1;
    if (index >= array->value.array->size()) return -1;
    json_t* item = (*array->value.array)[index];
    json_decref(item);
    array->value.array->erase(array->value.array->begin() + index);
    return 0;
}

JANSSON_API int json_array_clear(json_t* array) {
    if (!array || array->type != JSON_ARRAY || !array->value.array) return -1;
    for (auto* item : *array->value.array) {
        json_decref(item);
    }
    array->value.array->clear();
    return 0;
}

JANSSON_API int json_array_extend(json_t* array, json_t* other) {
    if (!array || array->type != JSON_ARRAY || !other || other->type != JSON_ARRAY) return -1;
    if (!array->value.array || !other->value.array) return -1;
    // Create copies to avoid double-free issues
    for (auto* item : *other->value.array) {
        json_t* copy = json_incref(item); // For now, just use the same pointer
        array->value.array->push_back(copy);
    }
    return 0;
}

// Object functions
JANSSON_API size_t json_object_size(const json_t* object) {
    if (!object || object->type != JSON_OBJECT) return 0;
    return object->value.object ? object->value.object->size() : 0;
}

JANSSON_API json_t* json_object_get(const json_t* object, const char* key) {
    if (!object || object->type != JSON_OBJECT || !key) return nullptr;
    if (!object->value.object) return nullptr;
    auto it = object->value.object->find(key);
    return (it != object->value.object->end()) ? it->second : nullptr;
}

JANSSON_API int json_object_set(json_t* object, const char* key, json_t* value) {
    if (!object || object->type != JSON_OBJECT || !key || !value) return -1;
    if (!object->value.object) return -1;
    (*object->value.object)[key] = value;
    return 0;
}

JANSSON_API int json_object_set_new(json_t* object, const char* key, json_t* value) {
    return json_object_set(object, key, value);
}

JANSSON_API int json_object_del(json_t* object, const char* key) {
    if (!object || object->type != JSON_OBJECT || !key) return -1;
    if (!object->value.object) return -1;
    auto it = object->value.object->find(key);
    if (it != object->value.object->end()) {
        json_decref(it->second);
        object->value.object->erase(it);
    }
    return 0;
}

JANSSON_API int json_object_clear(json_t* object) {
    if (!object || object->type != JSON_OBJECT || !object->value.object) return -1;
    for (auto& pair : *object->value.object) {
        json_decref(pair.second);
    }
    object->value.object->clear();
    return 0;
}

JANSSON_API int json_object_update(json_t* object, json_t* other) {
    if (!object || object->type != JSON_OBJECT || !other || other->type != JSON_OBJECT) return -1;
    if (!object->value.object || !other->value.object) return -1;
    for (auto& pair : *other->value.object) {
        (*object->value.object)[pair.first] = pair.second;
    }
    return 0;
}

JANSSON_API int json_object_update_existing(json_t* object, json_t* other) {
    if (!object || object->type != JSON_OBJECT || !other || other->type != JSON_OBJECT) return -1;
    if (!object->value.object || !other->value.object) return -1;
    for (auto& pair : *other->value.object) {
        if (object->value.object->find(pair.first) != object->value.object->end()) {
            (*object->value.object)[pair.first] = pair.second;
        }
    }
    return 0;
}

JANSSON_API int json_object_update_missing(json_t* object, json_t* other) {
    if (!object || object->type != JSON_OBJECT || !other || other->type != JSON_OBJECT) return -1;
    if (!object->value.object || !other->value.object) return -1;
    for (auto& pair : *other->value.object) {
        if (object->value.object->find(pair.first) == object->value.object->end()) {
            (*object->value.object)[pair.first] = pair.second;
        }
    }
    return 0;
}

// Utility functions
JANSSON_API json_type json_typeof(const json_t* json) {
    return json ? json->type : JSON_NULL;
}

JANSSON_API json_int_t json_integer_value(const json_t* integer) {
    if (!integer || integer->type != JSON_INTEGER) return 0;
    return integer->value.integer;
}

JANSSON_API json_real_t json_real_value(const json_t* real) {
    if (!real || real->type != JSON_REAL) return 0.0;
    return real->value.real;
}

JANSSON_API double json_number_value(const json_t* json) {
    if (!json) return 0.0;
    switch (json->type) {
        case JSON_INTEGER:
            return (double)json->value.integer;
        case JSON_REAL:
            return json->value.real;
        default:
            return 0.0;
    }
}

JANSSON_API const char* json_string_value(const json_t* string) {
    if (!string || string->type != JSON_STRING) return nullptr;
    return string->value.string;
}

JANSSON_API size_t json_string_length(const json_t* string) {
    if (!string || string->type != JSON_STRING || !string->value.string) return 0;
    return strlen(string->value.string);
}

// Configuration functions (stubs)
JANSSON_API void json_get_config(json_config_t* config) {
    if (config) {
        config->malloc_fn = malloc;
        config->realloc_fn = realloc;
        config->free_fn = free;
        config->gc_threshold = 0;
        config->gc_enabled = 0;
    }
}

JANSSON_API void json_set_config(const json_config_t* config) {
    // Stub implementation
}

// Serialization functions (stubs for now)
JANSSON_API char* json_dumps(const json_t* json, size_t flags) {
    if (!json) return nullptr;
    // Simple implementation - just return a basic string representation
    std::string result;
    switch (json->type) {
        case JSON_NULL:
            result = "null";
            break;
        case JSON_TRUE:
            result = "true";
            break;
        case JSON_FALSE:
            result = "false";
            break;
        case JSON_INTEGER:
            result = std::to_string(json->value.integer);
            break;
        case JSON_REAL:
            result = std::to_string(json->value.real);
            break;
        case JSON_STRING:
            result = "\"" + std::string(json->value.string) + "\"";
            break;
        default:
            result = "{}";
            break;
    }
    char* str = (char*)malloc(result.length() + 1);
    if (str) {
        strcpy(str, result.c_str());
    }
    return str;
}

JANSSON_API int json_dumpf(const json_t* json, FILE* output, size_t flags) {
    if (!json || !output) return -1;
    char* str = json_dumps(json, flags);
    if (!str) return -1;
    fprintf(output, "%s", str);
    free(str);
    return 0;
}

JANSSON_API int json_dump_file(const json_t* json, const char* path, size_t flags) {
    if (!json || !path) return -1;
    FILE* file = fopen(path, "w");
    if (!file) return -1;
    int result = json_dumpf(json, file, flags);
    fclose(file);
    return result;
}

// Parsing functions (stubs for now)
JANSSON_API json_t* json_loads(const char* input, size_t flags, json_error_t* error) {
    if (!input) {
        if (error) {
            strcpy(error->text, "No input");
        }
        return nullptr;
    }
    // Very simple parsing - just handle basic cases
    std::string str(input);
    if (str == "null") return json_null();
    if (str == "true") return json_true();
    if (str == "false") return json_false();
    if (str.length() > 2 && str[0] == '"' && str[str.length()-1] == '"') {
        std::string content = str.substr(1, str.length()-2);
        return json_string(content.c_str());
    }
    // Try to parse as number
    try {
        if (str.find('.') != std::string::npos) {
            double val = std::stod(str);
            return json_real(val);
        } else {
            json_int_t val = std::stoll(str);
            return json_integer(val);
        }
    } catch (...) {
        if (error) {
            strcpy(error->text, "Parse error");
        }
        return nullptr;
    }
}

JANSSON_API json_t* json_loadf(FILE* input, size_t flags, json_error_t* error) {
    if (!input) {
        if (error) {
            strcpy(error->text, "No input");
        }
        return nullptr;
    }
    // Read entire file
    fseek(input, 0, SEEK_END);
    long size = ftell(input);
    fseek(input, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) return nullptr;
    size_t read = fread(buffer, 1, size, input);
    buffer[read] = '\0';
    json_t* result = json_loads(buffer, flags, error);
    free(buffer);
    return result;
}

JANSSON_API json_t* json_load_file(const char* path, size_t flags, json_error_t* error) {
    if (!path) {
        if (error) {
            strcpy(error->text, "No path");
        }
        return nullptr;
    }
    FILE* file = fopen(path, "r");
    if (!file) {
        if (error) {
            strcpy(error->text, "Cannot open file");
        }
        return nullptr;
    }
    json_t* result = json_loadf(file, flags, error);
    fclose(file);
    return result;
}

// Seed function (stub)
JANSSON_API void json_object_seed(size_t seed) {
    // Stub implementation
}
