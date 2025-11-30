/**
 * JSON Array Example
 * 
 * This example demonstrates how to work with JSON arrays,
 * including creation, manipulation, and iteration.
 */

#include <iostream>
#include <string>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"

int main() {
    std::cout << "JSON Array Example" << std::endl;
    std::cout << "==================" << std::endl << std::endl;
    
    // Create an empty array
    json::Value fruits = json::Array();
    
    std::cout << "1. Creating and populating an array:" << std::endl;
    fruits.push_back("apple");
    fruits.push_back("banana");
    fruits.push_back("orange");
    fruits.push_back("grape");
    
    std::cout << "Fruits array: " << json::dump(fruits) << std::endl;
    std::cout << "Array size: " << fruits.size() << std::endl << std::endl;
    
    std::cout << "2. Accessing array elements:" << std::endl;
    std::cout << "First fruit: " << fruits[0].asString() << std::endl;
    std::cout << "Second fruit: " << fruits[1].asString() << std::endl;
    std::cout << "Last fruit: " << fruits[fruits.size() - 1].asString() << std::endl << std::endl;
    
    std::cout << "3. Modifying array elements:" << std::endl;
    fruits[1] = "strawberry";  // Replace banana
    std::cout << "After replacing banana: " << json::dump(fruits) << std::endl;
    
    std::cout << "4. Iterating through array:" << std::endl;
    std::cout << "All fruits: ";
    for (size_t i = 0; i < fruits.size(); ++i) {
        std::cout << fruits[i].asString();
        if (i < fruits.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "5. Array of numbers:" << std::endl;
    json::Value numbers = json::Array();
    for (int i = 1; i <= 5; ++i) {
        numbers.push_back(i * 10);
    }
    std::cout << "Numbers: " << json::dump(numbers) << std::endl;
    std::cout << "Sum of numbers: ";
    
    int sum = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        sum += numbers[i].asInt();
    }
    std::cout << sum << std::endl << std::endl;
    
    std::cout << "6. Array of objects:" << std::endl;
    json::Value employees = json::Array();
    
    json::Value emp1;
    emp1["name"] = "Alice";
    emp1["department"] = "Engineering";
    emp1["salary"] = 75000;
    
    json::Value emp2;
    emp2["name"] = "Bob";
    emp2["department"] = "Marketing";
    emp2["salary"] = 65000;
    
    employees.push_back(emp1);
    employees.push_back(emp2);
    
    std::cout << "Employees: " << json::dump(employees, json::Format::Pretty) << std::endl;
    
    std::cout << std::endl << "Array example completed successfully!" << std::endl;
    return 0;
}
