#include "../jasson_cpp/json_dumper.hpp"
#include "../jasson_cpp/json_factory.hpp"
#include <iostream>
#include <cassert>

using namespace jasson;

void test_basic_dump() {
    auto json = JsonFactory::create_object();
    json->set_field("name", JsonFactory::create_string("test"));
    json->set_field("value", JsonFactory::create_integer(42));
    json->set_field("pi", JsonFactory::create_real(3.14159));
    json->set_field("active", JsonFactory::create_boolean(true));
    json->set_field("data", JsonFactory::create_null());

    JsonDumper dumper;
    std::string result = dumper.dump(*json);
    
    std::cout << "Basic dump result: " << result << std::endl;
    assert(result == R"({"name":"test","value":42,"pi":3.14159,"active":true,"data":null})");
}

void test_formatted_dump() {
    auto json = JsonFactory::create_object();
    json->set_field("name", JsonFactory::create_string("test"));
    json->set_field("items", JsonFactory::create_array());
    json->get_field("items").get_array().push_back(JsonFactory::create_string("item1"));
    json->get_field("items").get_array().push_back(JsonFactory::create_integer(42));

    JsonDumper dumper(JsonDumpFormat::INDENTED, 2);
    std::string result = dumper.dump(*json);
    
    std::cout << "Formatted dump result:\n" << result << std::endl;
    assert(result.find("\n") != std::string::npos);
    assert(result.find("  ") != std::string::npos);
}

void test_array_dump() {
    auto json = JsonFactory::create_array();
    json->get_array().push_back(JsonFactory::create_string("first"));
    json->get_array().push_back(JsonFactory::create_integer(2));
    json->get_array().push_back(JsonFactory::create_real(3.14));
    json->get_array().push_back(JsonFactory::create_boolean(false));
    json->get_array().push_back(JsonFactory::create_null());

    JsonDumper dumper;
    std::string result = dumper.dump(*json);
    
    std::cout << "Array dump result: " << result << std::endl;
    assert(result == R"(["first",2,3.14,false,null])");
}

void test_escape_sequences() {
    auto json = JsonFactory::create_object();
    json->set_field("quote", JsonFactory::create_string("with\"quotes"));
    json->set_field("backslash", JsonFactory::create_string("with\\backslash"));
    json->set_field("newline", JsonFactory::create_string("with\nnewline"));
    json->set_field("tab", JsonFactory::create_string("with\ttab"));

    JsonDumper dumper;
    std::string result = dumper.dump(*json);
    
    std::cout << "Escape sequences result: " << result << std::endl;
    assert(result.find("\\\"") != std::string::npos);
    assert(result.find("\\\\") != std::string::npos);
    assert(result.find("\\n") != std::string::npos);
    assert(result.find("\\t") != std::string::npos);
}

void test_sorted_keys() {
    auto json = JsonFactory::create_object();
    json->set_field("zebra", JsonFactory::create_string("last"));
    json->set_field("alpha", JsonFactory::create_string("first"));
    json->set_field("beta", JsonFactory::create_string("second"));

    JsonDumper dumper(JsonDumpFormat::SORT_KEYS);
    std::string result = dumper.dump(*json);
    
    std::cout << "Sorted keys result: " << result << std::endl;
    assert(result.find("alpha") < result.find("beta"));
    assert(result.find("beta") < result.find("zebra"));
}

void test_file_dump() {
    auto json = JsonFactory::create_object();
    json->set_field("test", JsonFactory::create_string("file dump"));

    JsonDumper dumper(JsonDumpFormat::INDENTED);
    std::filesystem::path test_file("test_output.json");
    
    dumper.dump_to_file(*json, test_file);
    
    // Read back and verify
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    
    std::cout << "File dump result:\n" << content << std::endl;
    assert(content.find("test") != std::string::npos);
    assert(content.find("file dump") != std::string::npos);
    
    // Clean up
    std::filesystem::remove(test_file);
}

int main() {
    try {
        test_basic_dump();
        test_formatted_dump();
        test_array_dump();
        test_escape_sequences();
        test_sorted_keys();
        test_file_dump();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
