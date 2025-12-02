#ifndef JANSSON_TEST_BASE_HPP
#define JANSSON_TEST_BASE_HPP

#include <gtest/gtest.h>
#include "../src/json_loader.hpp"
#include "../src/json_value.hpp"
#include "../src/json_types.hpp"
#include "../src/error.hpp"
#include <memory>
#include <string>
#include <vector>

namespace jasson {
namespace test {

class JsonTestBase : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any common test setup
        error_buffer_.clear();
    }

    void TearDown() override {
        // Clean up any common test resources
    }

    // Common test utilities
    std::string getLastError() const {
        return error_buffer_;
    }

    void setError(const std::string& error) {
        error_buffer_ = error;
    }

    // Memory leak detection helper
    void checkForMemoryLeaks() {
        // This can be extended with actual memory leak detection
        // For now, it's a placeholder for future implementation
    }

    // JSON validation helper
    bool isValidJson(const std::string& json_str) {
        try {
            auto json = jasson::json_load(json_str);
            return true;
        } catch (...) {
            return false;
        }
    }

private:
    std::string error_buffer_;
};

} // namespace test
} // namespace jasson

#endif // JANSSON_TEST_BASE_HPP
