#include <iostream>
#include <cassert>
#include "json_value.hpp"
#include "json_factory.hpp"
#include "json_packer.hpp"
#include "json_unpacker.hpp"

using namespace jasson;

void test_basic_pack() {
    // Test packing basic types
    auto json1 = JsonPacker::pack("i", 42);
    assert(json1 && json1->is_integer());

    auto json2 = JsonPacker::pack("f", 3.14);
    assert(json2 && json2->is_real());

    auto json3 = JsonPacker::pack("s", "hello");
    assert(json3 && json3->is_string());

    auto json4 = JsonPacker::pack("b", true);
    assert(json4 && json4->is_boolean());

    auto json5 = JsonPacker::pack("n");
    assert(json5 && json5->is_null());
}

void test_array_pack() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto json = JsonPacker::pack("[i]", numbers);
    assert(json && json->is_array());
    auto array = static_cast<JsonArray*>(json.get());
    assert(array->size() == 5);
    for (size_t i = 0; i < 5; ++i) {
        assert(array->at(i)->is_integer());
    }
}

void test_object_pack() {
    auto json = JsonPacker::pack("{s:i,s:f,s:b}", 
        "age", 25,
        "height", 5.9,
        "active", true
    );
    assert(json && json->is_object());
    auto object = static_cast<JsonObject*>(json.get());
    assert(object->get("age")->is_integer());
    assert(object->get("height")->is_real());
    assert(object->get("active")->is_boolean());
}

void test_basic_unpack() {
    // Test unpacking basic types
    auto json1 = JsonFactory::create_integer(42);
    int value1;
    assert(JsonUnpacker::unpack(*json1, "i", value1) && value1 == 42);

    auto json2 = JsonFactory::create_real(3.14);
    double value2;
    assert(JsonUnpacker::unpack(*json2, "f", value2) && value2 == 3.14);

    auto json3 = JsonFactory::create_string("hello");
    std::string value3;
    assert(JsonUnpacker::unpack(*json3, "s", value3) && value3 == "hello");

    auto json4 = JsonFactory::create_true();
    bool value4;
    assert(JsonUnpacker::unpack(*json4, "b", value4) && value4 == true);

    auto json5 = JsonFactory::create_null();
    assert(JsonUnpacker::unpack(*json5, "n"));
}

void test_array_unpack() {
    auto json = JsonFactory::create_array();
    json->append(JsonFactory::create_integer(1));
    json->append(JsonFactory::create_integer(2));
    json->append(JsonFactory::create_integer(3));

    std::vector<int> numbers;
    assert(JsonUnpacker::unpack(*json, "[i]", numbers));
    assert(numbers.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
        assert(numbers[i] == i + 1);
    }
}

void test_object_unpack() {
    auto json = JsonFactory::create_object();
    json->set("age", JsonFactory::create_integer(25));
    json->set("height", JsonFactory::create_real(5.9));
    json->set("active", JsonFactory::create_true());

    int age;
    double height;
    bool active;
    assert(JsonUnpacker::unpack(*json, "{s:i,s:f,s:b}", 
        "age", age,
        "height", height,
        "active", active
    ));
    assert(age == 25);
    assert(height == 5.9);
    assert(active == true);
}

void test_optional_pack_unpack() {
    // Test optional packing
    std::optional<int> opt1 = 42;
    auto json1 = JsonPacker::pack("i", *opt1);
    assert(json1 && json1->is_integer());

    std::optional<int> opt2;
    auto json2 = JsonPacker::pack("n");
    assert(json2 && json2->is_null());

    // Test optional unpacking
    auto json3 = JsonFactory::create_integer(42);
    std::optional<int> opt3;
    assert(JsonUnpacker::unpack(*json3, "i", *opt3) && opt3.has_value() && *opt3 == 42);

    auto json4 = JsonFactory::create_null();
    std::optional<int> opt4 = 42; // Start with a value
    assert(JsonUnpacker::unpack(*json4, "n") && !opt4.has_value());
}

void test_variant_pack_unpack() {
    // Test variant packing
    std::variant<int, double, std::string> var1 = 42;
    auto json1 = JsonPacker::pack("i", std::get<int>(var1));
    assert(json1 && json1->is_integer());

    var1 = 3.14;
    auto json2 = JsonPacker::pack("f", std::get<double>(var1));
    assert(json2 && json2->is_real());

    var1 = std::string("hello");
    auto json3 = JsonPacker::pack("s", std::get<std::string>(var1));
    assert(json3 && json3->is_string());

    // Test variant unpacking
    auto json4 = JsonFactory::create_integer(42);
    int var2;
    assert(JsonUnpacker::unpack(*json4, "i", var2) && var2 == 42);
}

int main() {
    std::cout << "Running pack/unpack tests..." << std::endl;

    test_basic_pack();
    std::cout << "✓ Basic pack tests passed" << std::endl;

    test_array_pack();
    std::cout << "✓ Array pack tests passed" << std::endl;

    test_object_pack();
    std::cout << "✓ Object pack tests passed" << std::endl;

    test_basic_unpack();
    std::cout << "✓ Basic unpack tests passed" << std::endl;

    test_array_unpack();
    std::cout << "✓ Array unpack tests passed" << std::endl;

    test_object_unpack();
    std::cout << "✓ Object unpack tests passed" << std::endl;

    test_optional_pack_unpack();
    std::cout << "✓ Optional pack/unpack tests passed" << std::endl;

    test_variant_pack_unpack();
    std::cout << "✓ Variant pack/unpack tests passed" << std::endl;

    std::cout << "\nAll tests passed! ✨" << std::endl;
    return 0;
}
