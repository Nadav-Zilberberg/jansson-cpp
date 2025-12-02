#include "test_base.hpp"
#include "test_utils.hpp"
#include "test_data_generators.hpp"
#include "memory_leak_detector.hpp"

#include <gtest/gtest.h>
#include <chrono>
#include <iostream>

using namespace jasson::test;

class JsonLibraryTest : public JsonTestBase {
protected:
    void SetUp() override {
        JsonTestBase::SetUp();
        // Start memory leak monitoring
        MemoryLeakDetector::getInstance().startMonitoring();
    }

    void TearDown() override {
        // Check for memory leaks
        if (MemoryLeakDetector::getInstance().hasLeaks()) {
            std::cout << MemoryLeakDetector::getInstance().getLeakReport() << std::endl;
        }
        MemoryLeakDetector::getInstance().stopMonitoring();
        JsonTestBase::TearDown();
    }
};

TEST_F(JsonLibraryTest, BasicJsonOperations) {
    // Test basic JSON operations
    auto obj = JsonTestUtils::createSampleObject();
    auto arr = JsonTestUtils::createSampleArray();
    
    EXPECT_TRUE(obj->is_object());
    EXPECT_TRUE(arr->is_array());
    EXPECT_EQ(arr->as_array()->size(), 3);
}

TEST_F(JsonLibraryTest, RandomJsonGeneration) {
    TestDataGenerator generator;
    
    auto random_obj = generator.generateRandomObject();
    auto random_arr = generator.generateRandomArray();
    
    EXPECT_TRUE(random_obj->is_object());
    EXPECT_TRUE(random_arr->is_array());
}

TEST_F(JsonLibraryTest, EdgeCases) {
    TestDataGenerator generator;
    auto edge_cases = generator.generateEdgeCases();
    
    for (const auto& test_case : edge_cases) {
        EXPECT_NO_THROW(test_case->to_string());
    }
}

TEST_F(JsonLibraryTest, JsonParsing) {
    // Test JSON parsing
    std::string json_str = R"({"name": "test", "value": 42, "active": true})";
    
    EXPECT_TRUE(isValidJson(json_str));
    
    auto parsed = JsonTestUtils::safeParse(json_str);
    EXPECT_TRUE(parsed->is_object());
}

TEST_F(JsonLibraryTest, JsonSerialization) {
    // Test JSON serialization
    auto obj = JsonTestUtils::createSampleObject();
    auto arr = JsonTestUtils::createSampleArray();
    
    std::string obj_str = obj->to_string();
    std::string arr_str = arr->to_string();
    
    EXPECT_FALSE(obj_str.empty());
    EXPECT_FALSE(arr_str.empty());
}

TEST_F(JsonLibraryTest, LargeJsonPerformance) {
    TestDataGenerator generator;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto large_obj = generator.generateLargeObject(100);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_TRUE(large_obj->is_object());
    EXPECT_EQ(large_obj->as_object()->size(), 100);
    
    // Performance should be reasonable (less than 1 second for 100 properties)
    EXPECT_LT(duration.count(), 1000);
}

TEST_F(JsonLibraryTest, DeeplyNestedJson) {
    TestDataGenerator generator;
    
    auto nested = generator.generateDeeplyNested(10);
    
    EXPECT_TRUE(nested->is_object());
}

TEST_F(JsonLibraryTest, MemoryLeakDetection) {
    // Test memory leak detection
    MemoryLeakDetector::getInstance().startMonitoring();
    
    {
        auto obj = JsonTestUtils::createSampleObject();
        auto arr = JsonTestUtils::createSampleArray();
        
        EXPECT_TRUE(obj->is_object());
        EXPECT_TRUE(arr->is_array());
    }
    
    MemoryLeakDetector::getInstance().stopMonitoring();
    
    // No leaks should be detected
    EXPECT_FALSE(MemoryLeakDetector::getInstance().hasLeaks());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
