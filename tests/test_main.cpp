#include "test_base.hpp"
#include "test_utils.hpp"
#include "test_data_generators.hpp"
#include "memory_leak_detector.hpp"
#include <gtest/gtest.h>

using namespace jansson::test;

// Test fixture that uses the base test class
class JsonLibraryTest : public JsonTestBase {
protected:
    void SetUp() override {
        JsonTestBase::SetUp();
        // Additional setup for JSON library tests
        generator_ = std::make_unique<TestDataGenerator>();
    }

    void TearDown() override {
        // Check for memory leaks after each test
        checkForMemoryLeaks();
        JsonTestBase::TearDown();
    }

    TestDataGenerator& getGenerator() {
        return *generator_;
    }

private:
    std::unique_ptr<TestDataGenerator> generator_;
};

// Basic test using the test utilities
TEST_F(JsonLibraryTest, BasicJsonOperations) {
    auto obj = JsonTestUtils::createSampleObject();
    EXPECT_TRUE(obj.is_object());
    
    auto arr = JsonTestUtils::createSampleArray();
    EXPECT_TRUE(arr.is_array());
    EXPECT_EQ(arr.size(), 3);
}

// Test using the data generator
TEST_F(JsonLibraryTest, RandomJsonGeneration) {
    auto random_obj = getGenerator().generateRandomObject();
    EXPECT_TRUE(random_obj.is_object());
    
    auto random_arr = getGenerator().generateRandomArray();
    EXPECT_TRUE(random_arr.is_array());
}

// Test edge cases
TEST_F(JsonLibraryTest, EdgeCases) {
    auto edge_cases = getGenerator().generateEdgeCases();
    for (const auto& test_case : edge_cases) {
        EXPECT_NO_THROW(test_case.serialize());
    }
}

// Test memory leak detection
TEST_F(JsonLibraryTest, MemoryLeakDetection) {
    // This test would use the memory leak detector
    // For now, it's a placeholder showing how it would be used
    EXPECT_FALSE(hasLeaks());
}

// Test JSON validation
TEST_F(JsonLibraryTest, JsonValidation) {
    EXPECT_TRUE(isValidJson("{}"));
    EXPECT_TRUE(isValidJson("[]"));
    EXPECT_TRUE(isValidJson("\"test\""));
    EXPECT_TRUE(isValidJson("123"));
    EXPECT_TRUE(isValidJson("true"));
    EXPECT_TRUE(isValidJson("null"));
}

// Performance test with large JSON
TEST_F(JsonLibraryTest, LargeJsonPerformance) {
    auto large_obj = getGenerator().generateLargeObject(100);
    EXPECT_TRUE(large_obj.is_object());
    EXPECT_EQ(large_obj.size(), 100);
}

// Stress test with deeply nested JSON
TEST_F(JsonLibraryTest, DeeplyNestedJson) {
    auto nested = getGenerator().generateDeeplyNested(10);
    EXPECT_TRUE(nested.is_object());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
