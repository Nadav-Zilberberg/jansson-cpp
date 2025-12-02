#ifndef JANSSON_TEST_UTILS_HPP
#define JANSSON_TEST_UTILS_HPP

#include "../src/json_value.hpp"
#include "../src/json_types.hpp"
#include "../src/json_loader.hpp"
#include <string>
#include <vector>
#include <memory>

namespace jasson {
namespace test {

class JsonTestUtils {
public:
    // Create sample JSON objects
    static JsonValuePtr createSampleObject() {
        auto obj = std::make_shared<JsonObject>();
        obj->set("name", std::make_shared<JsonString>("test"));
        obj->set("value", std::make_shared<JsonInteger>(42));
        obj->set("active", std::make_shared<JsonBoolean>(true));
        return obj;
    }

    // Create sample JSON arrays
    static JsonValuePtr createSampleArray() {
        auto arr = std::make_shared<JsonArray>();
        arr->append(std::make_shared<JsonInteger>(1));
        arr->append(std::make_shared<JsonInteger>(2));
        arr->append(std::make_shared<JsonInteger>(3));
        return arr;
    }

    // Create sample JSON strings
    static JsonValuePtr createSampleString() {
        return std::make_shared<JsonString>("test string");
    }

    // Create sample JSON numbers
    static JsonValuePtr createSampleNumber() {
        return std::make_shared<JsonReal>(42.5);
    }

    // Create sample JSON booleans
    static JsonValuePtr createSampleBoolean() {
        return std::make_shared<JsonBoolean>(true);
    }

    // Create sample JSON null
    static JsonValuePtr createSampleNull() {
        return std::make_shared<JsonNull>();
    }

    // Compare two JSON values
    static bool jsonEquals(const JsonValuePtr& a, const JsonValuePtr& b) {
        if (!a || !b) return a == b;
        return a->to_string() == b->to_string();
    }

    // Parse JSON string safely
    static JsonValuePtr safeParse(const std::string& json_str) {
        try {
            return JsonLoader::load(json_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }
    }

    // Generate random JSON strings for testing
    static std::vector<std::string> generateRandomJsonStrings(size_t count) {
        std::vector<std::string> result;
        for (size_t i = 0; i < count; ++i) {
            result.push_back(generateRandomJsonString());
        }
        return result;
    }

private:
    static std::string generateRandomJsonString() {
        // Simple random JSON generator
        static int counter = 0;
        return "{\"id\": " + std::to_string(counter++) + ", \"random\": " + std::to_string(rand() % 1000) + "}";
    }
};

} // namespace test
} // namespace jasson

#endif // JANSSON_TEST_UTILS_HPP
