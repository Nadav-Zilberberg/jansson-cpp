#ifndef JANSSON_TEST_DATA_GENERATORS_HPP
#define JANSSON_TEST_DATA_GENERATORS_HPP

#include "../src/jasson.hpp"
#include "../src/json_types.hpp"
#include <string>
#include <vector>
#include <random>
#include <chrono>

namespace jasson {
namespace test {

class TestDataGenerator {
public:
    TestDataGenerator() : rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    // Generate random JSON objects
    JsonValuePtr generateRandomObject(int max_depth = 3, int max_properties = 5) {
        if (max_depth <= 0) {
            return generateRandomPrimitive();
        }

        auto obj = std::make_shared<JsonObject>();
        int num_properties = std::uniform_int_distribution<int>(1, max_properties)(rng_);
        
        for (int i = 0; i < num_properties; ++i) {
            std::string key = "prop_" + std::to_string(i);
            obj->set(key, generateRandomValue(max_depth - 1));
        }
        
        return obj;
    }

    // Generate random JSON arrays
    JsonValuePtr generateRandomArray(int max_depth = 3, int max_elements = 10) {
        if (max_depth <= 0) {
            return generateRandomPrimitive();
        }

        auto arr = std::make_shared<JsonArray>();
        int num_elements = std::uniform_int_distribution<int>(1, max_elements)(rng_);
        
        for (int i = 0; i < num_elements; ++i) {
            arr->append(generateRandomValue(max_depth - 1));
        }
        
        return arr;
    }

    // Generate random JSON strings
    JsonValuePtr generateRandomString(int min_length = 5, int max_length = 20) {
        int length = std::uniform_int_distribution<int>(min_length, max_length)(rng_);
        std::string result;
        
        for (int i = 0; i < length; ++i) {
            result += static_cast<char>(std::uniform_int_distribution<int>('a', 'z')(rng_));
        }
        
        return std::make_shared<JsonString>(result);
    }

    // Generate random JSON numbers
    JsonValuePtr generateRandomNumber(double min_val = -1000.0, double max_val = 1000.0) {
        std::uniform_real_distribution<double> dist(min_val, max_val);
        return std::make_shared<JsonReal>(dist(rng_));
    }

    // Generate random JSON booleans
    JsonValuePtr generateRandomBoolean() {
        return std::make_shared<JsonBoolean>(std::uniform_int_distribution<int>(0, 1)(rng_) == 1);
    }

    // Generate JSON null
    JsonValuePtr generateNull() {
        return std::make_shared<JsonNull>();
    }

    // Generate test cases for edge cases
    std::vector<JsonValuePtr> generateEdgeCases() {
        return {
            std::make_shared<JsonNull>(),  // null
            std::make_shared<JsonBoolean>(true),
            std::make_shared<JsonBoolean>(false),
            std::make_shared<JsonInteger>(0),
            std::make_shared<JsonInteger>(-1),
            std::make_shared<JsonInteger>(1),
            std::make_shared<JsonReal>(3.14159),
            std::make_shared<JsonReal>(-3.14159),
            std::make_shared<JsonString>(""),
            std::make_shared<JsonString>(" "),
            std::make_shared<JsonString>("null"),
            std::make_shared<JsonString>("true"),
            std::make_shared<JsonString>("false"),
            std::make_shared<JsonString>("0"),
            std::make_shared<JsonString>("1"),
            std::make_shared<JsonArray>(),
            std::make_shared<JsonObject>(),
            []() {
                auto arr = std::make_shared<JsonArray>();
                arr->append(std::make_shared<JsonNull>());
                return arr;
            }(),
            []() {
                auto obj = std::make_shared<JsonObject>();
                obj->set("key", std::make_shared<JsonNull>());
                return obj;
            }()
        };
    }

    // Generate large JSON objects for performance testing
    JsonValuePtr generateLargeObject(int num_properties = 1000) {
        auto obj = std::make_shared<JsonObject>();
        for (int i = 0; i < num_properties; ++i) {
            std::string key = "key_" + std::to_string(i);
            obj->set(key, generateRandomPrimitive());
        }
        return obj;
    }

    // Generate deeply nested JSON for stress testing
    JsonValuePtr generateDeeplyNested(int depth) {
        if (depth <= 0) {
            return generateRandomPrimitive();
        }
        
        auto obj = std::make_shared<JsonObject>();
        obj->set("nested", generateDeeplyNested(depth - 1));
        return obj;
    }

private:
    std::mt19937 rng_;

    JsonValuePtr generateRandomValue(int max_depth) {
        std::uniform_int_distribution<int> type_dist(0, 5);
        
        switch (type_dist(rng_)) {
            case 0: return generateRandomString();
            case 1: return generateRandomNumber();
            case 2: return generateRandomBoolean();
            case 3: return generateNull();
            case 4: return generateRandomArray(max_depth);
            case 5: return generateRandomObject(max_depth);
            default: return generateNull();
        }
    }

    JsonValuePtr generateRandomPrimitive() {
        std::uniform_int_distribution<int> type_dist(0, 2);
        
        switch (type_dist(rng_)) {
            case 0: return generateRandomString();
            case 1: return generateRandomNumber();
            case 2: return generateRandomBoolean();
            default: return generateNull();
        }
    }
};

} // namespace test
} // namespace jasson

#endif // JANSSON_TEST_DATA_GENERATORS_HPP
