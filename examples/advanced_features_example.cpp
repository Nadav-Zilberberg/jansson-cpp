/**
 * Advanced Features Example
 * 
 * This example demonstrates advanced features of the JSON library
 * including custom serialization, extensions, and integration patterns.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"
#include "src/json_extensions.hpp"

// Example of a custom class that can be serialized to/from JSON
class Product {
private:
    std::string id;
    std::string name;
    double price;
    std::vector<std::string> categories;
    bool inStock;

public:
    Product() : price(0.0), inStock(false) {}
    Product(const std::string& _id, const std::string& _name, double _price, 
            const std::vector<std::string>& _categories, bool _inStock)
        : id(_id), name(_name), price(_price), categories(_categories), inStock(_inStock) {}

    // Convert to JSON
    json::Value toJson() const {
        json::Value result;
        result["id"] = id;
        result["name"] = name;
        result["price"] = price;
        result["categories"] = json::fromVector(categories);
        result["inStock"] = inStock;
        return result;
    }

    // Convert from JSON
    static Product fromJson(const json::Value& json) {
        Product product;
        product.id = json["id"].asString();
        product.name = json["name"].asString();
        product.price = json["price"].asDouble();
        product.categories = json["categories"].toVector<std::string>();
        product.inStock = json["inStock"].asBool();
        return product;
    }

    // Getters
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    double getPrice() const { return price; }
    const std::vector<std::string>& getCategories() const { return categories; }
    bool isInStock() const { return inStock; }
};

void demonstrateCustomSerialization() {
    std::cout << "1. Custom Serialization:" << std::endl;
    
    // Create products
    std::vector<Product> products = {
        Product("P001", "Laptop", 999.99, {"Electronics", "Computers"}, true),
        Product("P002", "Mouse", 29.99, {"Electronics", "Accessories"}, true),
        Product("P003", "Keyboard", 79.99, {"Electronics", "Accessories"}, false)
    };
    
    // Convert to JSON
    json::Value productsJson = json::Array();
    for (const auto& product : products) {
        productsJson.push_back(product.toJson());
    }
    
    std::cout << "  Products as JSON:" << std::endl;
    std::cout << json::dump(productsJson, json::Format::Pretty) << std::endl;
    
    // Convert back from JSON
    std::vector<Product> restoredProducts;
    for (size_t i = 0; i < productsJson.size(); ++i) {
        restoredProducts.push_back(Product::fromJson(productsJson[i]));
    }
    
    std::cout << "  Restored products:" << std::endl;
    for (const auto& product : restoredProducts) {
        std::cout << "    " << product.getId() << ": " << product.getName() 
                  << " ($" << product.getPrice() << ")" << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateJsonExtensions() {
    std::cout << "2. JSON Extensions:" << std::endl;
    
    // Create sample data
    json::Value data;
    data["users"] = json::Array();
    
    for (int i = 0; i < 10; ++i) {
        json::Value user;
        user["id"] = i;
        user["name"] = "User" + std::to_string(i);
        user["score"] = rand() % 100;
        user["active"] = (i % 3 == 0);
        data["users"].push_back(user);
    }
    
    // Use extension functions (these would be provided by json_extensions.hpp)
    std::cout << "  Total users: " << data["users"].size() << std::endl;
    
    // Filter users with high scores
    json::Value highScorers = json::Array();
    for (size_t i = 0; i < data["users"].size(); ++i) {
        if (data["users"][i]["score"].asInt() > 70) {
            highScorers.push_back(data["users"][i]);
        }
    }
    std::cout << "  High scorers (>70): " << highScorers.size() << std::endl;
    
    // Find active users
    json::Value activeUsers = json::Array();
    for (size_t i = 0; i < data["users"].size(); ++i) {
        if (data["users"][i]["active"].asBool()) {
            activeUsers.push_back(data["users"][i]);
        }
    }
    std::cout << "  Active users: " << activeUsers.size() << std::endl;
    
    // Calculate average score
    int totalScore = 0;
    for (size_t i = 0; i < data["users"].size(); ++i) {
        totalScore += data["users"][i]["score"].asInt();
    }
    double averageScore = static_cast<double>(totalScore) / data["users"].size();
    std::cout << "  Average score: " << averageScore << std::endl;
    std::cout << std::endl;
}

void demonstrateJsonValidation() {
    std::cout << "3. JSON Validation:" << std::endl;
    
    // Define a schema for validation
    auto validateUser = [](const json::Value& user) -> bool {
        if (!user.isObject()) return false;
        if (!user.has("id") || !user["id"].isInt()) return false;
        if (!user.has("name") || !user["name"].isString()) return false;
        if (!user.has("email") || !user["email"].isString()) return false;
        if (user.has("age") && !user["age"].isInt()) return false;
        return true;
    };
    
    // Test valid user
    json::Value validUser;
    validUser["id"] = 1;
    validUser["name"] = "John Doe";
    validUser["email"] = "john@example.com";
    validUser["age"] = 30;
    
    std::cout << "  Valid user: " << (validateUser(validUser) ? "PASS" : "FAIL") << std::endl;
    
    // Test invalid users
    json::Value invalidUser1;
    invalidUser1["id"] = "not a number";  // Wrong type
    invalidUser1["name"] = "Jane Doe";
    
    json::Value invalidUser2;
    invalidUser2["id"] = 2;
    invalidUser2["name"] = "Bob Smith";
    invalidUser2["age"] = "thirty";  // Wrong type for age
    
    std::cout << "  Invalid user 1: " << (validateUser(invalidUser1) ? "PASS" : "FAIL") << std::endl;
    std::cout << "  Invalid user 2: " << (validateUser(invalidUser2) ? "PASS" : "FAIL") << std::endl;
    std::cout << std::endl;
}

void demonstrateFileOperations() {
    std::cout << "4. File Operations:" << std::endl;
    
    // Create sample data
    json::Value config;
    config["application"] = "MyApp";
    config["version"] = "1.0.0";
    config["settings"] = json::Object();
    config["settings"]["debug"] = true;
    config["settings"]["port"] = 8080;
    config["settings"]["allowed_hosts"] = json::Array();
    config["settings"]["allowed_hosts"].push_back("localhost");
    config["settings"]["allowed_hosts"].push_back("127.0.0.1");
    
    // Write to file
    std::string filename = "example_config.json";
    try {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << json::dump(config, json::Format::Pretty);
            file.close();
            std::cout << "  Configuration written to " << filename << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  Error writing file: " << e.what() << std::endl;
    }
    
    // Read from file
    try {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string jsonStr((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            file.close();
            
            json::Value loadedConfig = json::parse(jsonStr);
            std::cout << "  Configuration loaded from " << filename << std::endl;
            std::cout << "  Application: " << loadedConfig["application"].asString() << std::endl;
            std::cout << "  Debug mode: " << (loadedConfig["settings"]["debug"].asBool() ? "ON" : "OFF") << std::endl;
            std::cout << "  Port: " << loadedConfig["settings"]["port"].asInt() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  Error reading file: " << e.what() << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateAdvancedQueries() {
    std::cout << "5. Advanced JSON Queries:" << std::endl;
    
    // Create a complex data structure
    json::Value company;
    company["name"] = "Tech Corp";
    company["departments"] = json::Array();
    
    std::vector<std::string> deptNames = {"Engineering", "Sales", "Marketing", "HR"};
    for (size_t d = 0; d < deptNames.size(); ++d) {
        json::Value dept;
        dept["name"] = deptNames[d];
        dept["employees"] = json::Array();
        
        for (int e = 0; e < 5; ++e) {
            json::Value emp;
            emp["id"] = d * 100 + e;
            emp["name"] = "Emp" + std::to_string(d * 100 + e);
            emp["salary"] = 50000 + (rand() % 50000);
            emp["skills"] = json::Array();
            
            std::vector<std::string> skills = {"C++", "Python", "Java", "JavaScript", "SQL"};
            int numSkills = rand() % 3 + 1;
            for (int s = 0; s < numSkills; ++s) {
                emp["skills"].push_back(skills[rand() % skills.size()]);
            }
            
            dept["employees"].push_back(emp);
        }
        
        company["departments"].push_back(dept);
    }
    
    // Find employees with specific skills
    std::cout << "  Employees with C++ skills:" << std::endl;
    for (size_t d = 0; d < company["departments"].size(); ++d) {
        const json::Value& dept = company["departments"][d];
        for (size_t e = 0; e < dept["employees"].size(); ++e) {
            const json::Value& emp = dept["employees"][e];
            
            bool hasCpp = false;
            for (size_t s = 0; s < emp["skills"].size(); ++s) {
                if (emp["skills"][s].asString() == "C++") {
                    hasCpp = true;
                    break;
                }
            }
            
            if (hasCpp) {
                std::cout << "    " << emp["name"].asString() 
                          << " (" << dept["name"].asString() << ")" << std::endl;
            }
        }
    }
    
    // Calculate department statistics
    std::cout << "  Department salary statistics:" << std::endl;
    for (size_t d = 0; d < company["departments"].size(); ++d) {
        const json::Value& dept = company["departments"][d];
        int totalSalary = 0;
        int maxSalary = 0;
        int minSalary = INT_MAX;
        
        for (size_t e = 0; e < dept["employees"].size(); ++e) {
            int salary = dept["employees"][e]["salary"].asInt();
            totalSalary += salary;
            maxSalary = std::max(maxSalary, salary);
            minSalary = std::min(minSalary, salary);
        }
        
        double avgSalary = static_cast<double>(totalSalary) / dept["employees"].size();
        std::cout << "    " << dept["name"].asString() << ": "
                  << "Avg: $" << static_cast<int>(avgSalary) << ", "
                  << "Min: $" << minSalary << ", "
                  << "Max: $" << maxSalary << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "JSON Library Advanced Features Example" << std::endl;
    std::cout << "=======================================" << std::endl << std::endl;
    
    try {
        demonstrateCustomSerialization();
        demonstrateJsonExtensions();
        demonstrateJsonValidation();
        demonstrateFileOperations();
        demonstrateAdvancedQueries();
        
        std::cout << "Advanced features example completed successfully!" << std::endl;
        std::cout << std::endl;
        std::cout << "Advanced Features Summary:" << std::endl;
        std::cout << "- Custom serialization for C++ classes" << std::endl;
        std::cout << "- JSON validation with custom schemas" << std::endl;
        std::cout << "- File I/O operations" << std::endl;
        std::cout << "- Complex data queries and analysis" << std::endl;
        std::cout << "- Extension functions for common operations" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
