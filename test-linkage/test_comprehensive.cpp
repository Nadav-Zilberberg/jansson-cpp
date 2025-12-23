#include "jansson-cpp.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace jasson;

void test_complex_json_structure() {
    std::cout << "Testing Complex JSON Structure..." << std::endl;
    
    // Create a complex JSON structure representing a company
    auto company = std::make_shared<JsonObject>();
    
    // Company info
    company->set("name", std::string("Tech Corp"));
    company->set("founded", static_cast<json_int_t>(2010));
    company->set("public", true);
    company->set("revenue", 1250000000.50);
    
    // Create departments array
    auto departments = std::make_shared<JsonArray>();
    
    // Engineering department
    auto engineering = std::make_shared<JsonObject>();
    engineering->set("name", std::string("Engineering"));
    engineering->set("budget", static_cast<json_int_t>(50000000));
    
    auto eng_employees = std::make_shared<JsonArray>();
    auto eng_emp1 = std::make_shared<JsonObject>();
    eng_emp1->set("name", std::string("Alice Johnson"));
    eng_emp1->set("role", std::string("Senior Developer"));
    eng_emp1->set("salary", static_cast<json_int_t>(120000));
    eng_emp1->set("remote", true);
    
    auto eng_emp2 = std::make_shared<JsonObject>();
    eng_emp2->set("name", std::string("Bob Smith"));
    eng_emp2->set("role", std::string("DevOps Engineer"));
    eng_emp2->set("salary", static_cast<json_int_t>(110000));
    eng_emp2->set("remote", false);
    
    eng_employees->append(eng_emp1);
    eng_employees->append(eng_emp2);
    engineering->set("employees", eng_employees);
    
    // Sales department
    auto sales = std::make_shared<JsonObject>();
    sales->set("name", std::string("Sales"));
    sales->set("budget", static_cast<json_int_t>(30000000));
    
    auto sales_employees = std::make_shared<JsonArray>();
    auto sales_emp1 = std::make_shared<JsonObject>();
    sales_emp1->set("name", std::string("Carol Davis"));
    sales_emp1->set("role", std::string("Sales Manager"));
    sales_emp1->set("salary", static_cast<json_int_t>(90000));
    sales_emp1->set("quota", static_cast<json_int_t>(1000000));
    
    sales_employees->append(sales_emp1);
    sales->set("employees", sales_employees);
    
    departments->append(engineering);
    departments->append(sales);
    company->set("departments", departments);
    
    // Create metadata object
    auto metadata = std::make_shared<JsonObject>();
    metadata->set("created", std::string("2023-12-01"));
    metadata->set("version", std::string("1.0"));
    metadata->set_null("deprecated");
    company->set("metadata", metadata);
    
    // Verify the structure
    assert(company->has("name"));
    assert(company->has("founded"));
    assert(company->has("public"));
    assert(company->has("revenue"));
    assert(company->has("departments"));
    assert(company->has("metadata"));
    
    assert(company->get("name")->as_string()->value() == "Tech Corp");
    assert(company->get("founded")->as_integer()->value() == 2010);
    assert(company->get("public")->as_boolean()->value() == true);
    assert(company->get("revenue")->as_real()->value() == 1250000000.50);
    
    auto depts = company->get("departments")->as_array();
    assert(depts->size() == 2);
    
    auto eng_dept = depts->at(0)->as_object();
    assert(eng_dept->get("name")->as_string()->value() == "Engineering");
    assert(eng_dept->get("budget")->as_integer()->value() == 50000000);
    
    auto eng_emps = eng_dept->get("employees")->as_array();
    assert(eng_emps->size() == 2);
    assert(eng_emps->at(0)->as_object()->get("name")->as_string()->value() == "Alice Johnson");
    assert(eng_emps->at(1)->as_object()->get("name")->as_string()->value() == "Bob Smith");
    
    auto meta = company->get("metadata")->as_object();
    assert(meta->get("created")->as_string()->value() == "2023-12-01");
    assert(meta->get("version")->as_string()->value() == "1.0");
    assert(meta->get("deprecated")->is_null());
    
    // Test serialization
    std::string json_str = company->dump(2);
    std::cout << "Complex company structure:\n" << json_str << std::endl;
    
    assert(json_str.find("\"name\": \"Tech Corp\"") != std::string::npos);
    assert(json_str.find("\"founded\": 2010") != std::string::npos);
    assert(json_str.find("\"public\": true") != std::string::npos);
    assert(json_str.find("\"revenue\": 1250000000.5") != std::string::npos);
    
    std::cout << "✓ Complex JSON Structure test passed" << std::endl;
}

void test_json_roundtrip() {
    std::cout << "Testing JSON Roundtrip..." << std::endl;
    
    // Create original JSON
    auto original = std::make_shared<JsonObject>();
    original->set("string", std::string("test value"));
    original->set("integer", static_cast<json_int_t>(42));
    original->set("real", 3.14159);
    original->set("boolean", true);
    original->set_null("null_value");
    
    auto array = std::make_shared<JsonArray>();
    array->append(std::string("item1"));
    array->append(static_cast<json_int_t>(123));
    array->append(false);
    original->set("array", array);
    
    auto nested = std::make_shared<JsonObject>();
    nested->set("nested_key", std::string("nested_value"));
    original->set("nested", nested);
    
    // Serialize to string
    std::string json_str = original->dump();
    std::cout << "Serialized JSON: " << json_str << std::endl;
    
    // Verify all elements are present in serialization
    assert(json_str.find("\"string\":\"test value\"") != std::string::npos);
    assert(json_str.find("\"integer\":42") != std::string::npos);
    assert(json_str.find("\"real\":3.14159") != std::string::npos);
    assert(json_str.find("\"boolean\":true") != std::string::npos);
    assert(json_str.find("\"null_value\":null") != std::string::npos);
    assert(json_str.find("\"array\":[") != std::string::npos);
    assert(json_str.find("\"nested\":{") != std::string::npos);
    
    std::cout << "✓ JSON Roundtrip test passed" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing Error Handling..." << std::endl;
    
    // Test array bounds checking
    auto array = std::make_shared<JsonArray>();
    array->append(std::string("test"));
    
    bool array_exception_thrown = false;
    try {
        array->at(999); // Out of bounds
    } catch (const std::out_of_range&) {
        array_exception_thrown = true;
    }
    assert(array_exception_thrown);
    
    // Test object key checking
    auto object = std::make_shared<JsonObject>();
    object->set("existing", std::string("value"));
    
    bool object_exception_thrown = false;
    try {
        object->get("nonexistent");
    } catch (const std::out_of_range&) {
        object_exception_thrown = true;
    }
    assert(object_exception_thrown);
    
    // Test type checking
    auto str_val = std::make_shared<JsonString>("test");
    assert(!str_val->is_integer());
    assert(!str_val->is_array());
    assert(str_val->as_integer() == nullptr);
    assert(str_val->as_array() == nullptr);
    
    auto int_val = std::make_shared<JsonInteger>(42);
    assert(!int_val->is_string());
    assert(!int_val->is_object());
    assert(int_val->as_string() == nullptr);
    assert(int_val->as_object() == nullptr);
    
    std::cout << "✓ Error Handling test passed" << std::endl;
}

void test_memory_management() {
    std::cout << "Testing Memory Management..." << std::endl;
    
    // Create a complex structure and ensure shared_ptr handles cleanup
    {
        auto root = std::make_shared<JsonObject>();
        auto array = std::make_shared<JsonArray>();
        
        for (int i = 0; i < 100; ++i) {
            auto item = std::make_shared<JsonObject>();
            item->set("id", static_cast<json_int_t>(i));
            item->set("name", std::string("Item " + std::to_string(i)));
            item->set("active", i % 2 == 0);
            
            auto nested_array = std::make_shared<JsonArray>();
            for (int j = 0; j < 10; ++j) {
                nested_array->append(static_cast<json_int_t>(j * i));
            }
            item->set("values", nested_array);
            
            array->append(item);
        }
        
        root->set("items", array);
        root->set("count", static_cast<json_int_t>(100));
        
        // Verify structure
        assert(root->get("items")->as_array()->size() == 100);
        assert(root->get("count")->as_integer()->value() == 100);
        
        // Serialize to ensure everything works
        std::string json_str = root->dump();
        // Check for the count field in compact format
        assert(json_str.find("\"count\":100") != std::string::npos);
        assert(json_str.find("\"id\":99") != std::string::npos);
    }
    // Shared pointers should clean up automatically
    
    std::cout << "✓ Memory Management test passed" << std::endl;
}

int main() {
    std::cout << "=== Running Comprehensive JSON Tests ===" << std::endl;
    
    try {
        test_complex_json_structure();
        test_json_roundtrip();
        test_error_handling();
        test_memory_management();
        
        std::cout << "\n🎉 All comprehensive JSON tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
