/**
 * JSON Object Example
 * 
 * This example demonstrates how to work with JSON objects,
 * including creation, manipulation, and property access.
 */

#include <iostream>
#include <string>
#include "src/jasson.hpp"
#include "src/json_value.hpp"
#include "src/json_factory.hpp"

int main() {
    std::cout << "JSON Object Example" << std::endl;
    std::cout << "===================" << std::endl << std::endl;
    
    // Create an empty object
    json::Value person;
    
    std::cout << "1. Creating and populating an object:" << std::endl;
    person["name"] = "John Doe";
    person["age"] = 30;
    person["email"] = "john@example.com";
    person["active"] = true;
    
    std::cout << "Person object: " << json::dump(person, json::Format::Pretty) << std::endl;
    std::cout << "Number of properties: " << person.size() << std::endl << std::endl;
    
    std::cout << "2. Accessing object properties:" << std::endl;
    std::cout << "Name: " << person["name"].asString() << std::endl;
    std::cout << "Age: " << person["age"].asInt() << std::endl;
    std::cout << "Email: " << person["email"].asString() << std::endl;
    std::cout << "Active: " << (person["active"].asBool() ? "yes" : "no") << std::endl << std::endl;
    
    std::cout << "3. Checking property existence:" << std::endl;
    std::cout << "Has 'name' property: " << (person.has("name") ? "yes" : "no") << std::endl;
    std::cout << "Has 'phone' property: " << (person.has("phone") ? "yes" : "no") << std::endl << std::endl;
    
    std::cout << "4. Modifying object properties:" << std::endl;
    person["age"] = 31;  // Update age
    person["phone"] = "+1-555-1234";  // Add new property
    
    std::cout << "After modifications: " << json::dump(person, json::Format::Pretty) << std::endl << std::endl;
    
    std::cout << "5. Nested objects:" << std::endl;
    json::Value address;
    address["street"] = "123 Main St";
    address["city"] = "New York";
    address["zip"] = "10001";
    
    person["address"] = address;
    
    std::cout << "Person with address: " << json::dump(person, json::Format::Pretty) << std::endl;
    std::cout << "City: " << person["address"]["city"].asString() << std::endl << std::endl;
    
    std::cout << "6. Object iteration:" << std::endl;
    std::cout << "All properties:" << std::endl;
    for (auto it = person.begin(); it != person.end(); ++it) {
        std::cout << "  " << it.key() << ": " << json::dump(it.value()) << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "7. Removing properties:" << std::endl;
    person.remove("phone");
    std::cout << "After removing phone: " << json::dump(person, json::Format::Pretty) << std::endl;
    
    std::cout << std::endl << "Object example completed successfully!" << std::endl;
    return 0;
}
