/**
 * Performance Example
 * 
 * This example demonstrates performance optimization techniques
 * and benchmarks different approaches to working with JSON data.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"

using namespace std::chrono;

class Timer {
private:
    high_resolution_clock::time_point start_time;
    std::string name;
    
public:
    Timer(const std::string& timer_name) : name(timer_name) {
        start_time = high_resolution_clock::now();
    }
    
    ~Timer() {
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        std::cout << "  " << name << ": " << duration.count() << " microseconds" << std::endl;
    }
};

void demonstrateParsingPerformance() {
    std::cout << "1. JSON Parsing Performance:" << std::endl;
    
    // Create a moderately complex JSON string
    std::ostringstream jsonStream;
    jsonStream << "{";
    jsonStream << "\"users\":[";
    for (int i = 0; i < 100; ++i) {
        if (i > 0) jsonStream << ",";
        jsonStream << "{";
        jsonStream << "\"id\":" << i << ",";
        jsonStream << "\"name\":\"User" << i << "\",";
        jsonStream << "\"email\":\"user" << i << "@example.com\",";
        jsonStream << "\"active\":" << (i % 2 == 0 ? "true" : "false");
        jsonStream << "}";
    }
    jsonStream << "],";
    jsonStream << "\"metadata\":{";
    jsonStream << "\"total\":100,";
    jsonStream << "\"page\":1,";
    jsonStream << "\"per_page\":50";
    jsonStream << "}";
    jsonStream << "}";
    
    std::string jsonString = jsonStream.str();
    
    {
        Timer t("Parse JSON string");
        json::Value result = json::parse(jsonString);
    }
    
    {
        Timer t("Parse and access data");
        json::Value result = json::parse(jsonString);
        int totalUsers = result["users"].size();
        std::string firstUserName = result["users"][0]["name"].asString();
    }
    std::cout << std::endl;
}

void demonstrateSerializationPerformance() {
    std::cout << "2. JSON Serialization Performance:" << std::endl;
    
    // Create a large JSON structure
    json::Value data;
    data["users"] = json::Array();
    
    for (int i = 0; i < 100; ++i) {
        json::Value user;
        user["id"] = i;
        user["name"] = "User" + std::to_string(i);
        user["email"] = "user" + std::to_string(i) + "@example.com";
        user["active"] = (i % 2 == 0);
        user["scores"] = json::Array();
        
        for (int j = 0; j < 10; ++j) {
            user["scores"].push_back(rand() % 100);
        }
        
        data["users"].push_back(user);
    }
    
    {
        Timer t("Serialize to compact JSON");
        std::string compact = json::dump(data, json::Format::Compact);
    }
    
    {
        Timer t("Serialize to pretty JSON");
        std::string pretty = json::dump(data, json::Format::Pretty);
    }
    std::cout << std::endl;
}

void demonstrateMemoryEfficiency() {
    std::cout << "3. Memory Efficiency Techniques:" << std::endl;
    
    // Efficient array building
    {
        Timer t("Build array with reserve");
        json::Value arr = json::Array();
        // Note: In a real implementation, you might have a reserve method
        for (int i = 0; i < 1000; ++i) {
            arr.push_back(i);
        }
    }
    
    // Efficient object building
    {
        Timer t("Build object with direct assignment");
        json::Value obj;
        for (int i = 0; i < 100; ++i) {
            obj["key" + std::to_string(i)] = "value" + std::to_string(i);
        }
    }
    std::cout << std::endl;
}

void demonstrateAccessPatterns() {
    std::cout << "4. Access Pattern Performance:" << std::endl;
    
    // Create test data
    json::Value data;
    data["users"] = json::Array();
    
    for (int i = 0; i < 50; ++i) {
        json::Value user;
        user["id"] = i;
        user["name"] = "User" + std::to_string(i);
        user["email"] = "user" + std::to_string(i) + "@example.com";
        user["department"] = "Dept" + std::to_string(i % 5);
        user["salary"] = 50000 + (i * 1000);
        data["users"].push_back(user);
    }
    
    // Sequential access
    {
        Timer t("Sequential access to all users");
        for (size_t i = 0; i < data["users"].size(); ++i) {
            const json::Value& user = data["users"][i];
            std::string name = user["name"].asString();
            int salary = user["salary"].asInt();
        }
    }
    
    // Random access
    {
        Timer t("Random access to users");
        std::vector<int> indices = {5, 12, 23, 34, 45, 1, 8, 19, 30, 41};
        for (int idx : indices) {
            const json::Value& user = data["users"][idx];
            std::string name = user["name"].asString();
            std::string dept = user["department"].asString();
        }
    }
    std::cout << std::endl;
}

void demonstrateOptimizationTechniques() {
    std::cout << "5. Optimization Techniques:" << std::endl;
    
    // Technique 1: Minimize temporary objects
    {
        Timer t("Create with minimal temporaries");
        json::Value data;
        data["status"] = "success";
        data["count"] = 100;
        data["data"] = json::Array();
        
        for (int i = 0; i < 50; ++i) {
            json::Value item;
            item["id"] = i;
            item["value"] = i * 2;
            data["data"].push_back(item);
        }
    }
    
    // Technique 2: Reuse objects when possible
    {
        Timer t("Create with object reuse");
        json::Value data;
        data["items"] = json::Array();
        
        json::Value templateItem;
        templateItem["type"] = "standard";
        templateItem["status"] = "active";
        
        for (int i = 0; i < 50; ++i) {
            json::Value item = templateItem; // Copy template
            item["id"] = i;
            item["name"] = "Item" + std::to_string(i);
            data["items"].push_back(item);
        }
    }
    std::cout << std::endl;
}

void demonstrateStreamingPerformance() {
    std::cout << "6. Large Data Handling:" << std::endl;
    
    // Simulate processing large amounts of data
    {
        Timer t("Process 1000 records");
        json::Value batch;
        batch["records"] = json::Array();
        
        for (int i = 0; i < 1000; ++i) {
            json::Value record;
            record["id"] = i;
            record["timestamp"] = 1600000000 + i;
            record["value"] = rand() % 1000;
            record["status"] = (i % 10 == 0) ? "error" : "ok";
            
            batch["records"].push_back(record);
            
            // Simulate batch processing
            if (i % 100 == 99) {
                // Process batch (in real app, might write to file or send over network)
                std::string batchJson = json::dump(batch, json::Format::Compact);
                batch["records"] = json::Array(); // Reset for next batch
            }
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "JSON Library Performance Example" << std::endl;
    std::cout << "=================================" << std::endl << std::endl;
    
    try {
        demonstrateParsingPerformance();
        demonstrateSerializationPerformance();
        demonstrateMemoryEfficiency();
        demonstrateAccessPatterns();
        demonstrateOptimizationTechniques();
        demonstrateStreamingPerformance();
        
        std::cout << "Performance example completed successfully!" << std::endl;
        std::cout << std::endl;
        std::cout << "Performance Tips:" << std::endl;
        std::cout << "- Use compact format for network transmission" << std::endl;
        std::cout << "- Use pretty format only for debugging/human reading" << std::endl;
        std::cout << "- Minimize temporary object creation" << std::endl;
        std::cout << "- Consider streaming for very large datasets" << std::endl;
        std::cout << "- Profile your specific use case" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
