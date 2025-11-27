#include "jasson_cpp/jasson.hpp"
#include <iostream>
#include <cassert>

using namespace jasson;

void test_object() {
    auto obj = jasson_object();
    assert(obj->is_object());
    assert(obj->size() == 0);

    obj->set("name", jasson_string("John"));
    obj->set("age", jasson_integer(30));
    obj->set("active", jasson_true());

    assert(obj->size() == 3);
    assert(obj->get("name")->is_string());
    assert(obj->get("age")->is_integer());
    assert(json_is_true(*obj->get("active")));

    std::cout << "Object test passed!" << std::endl;
}

void test_array() {
    auto arr = jasson_array();
    assert(arr->is_array());
    assert(arr->size() == 0);

    arr->append(jasson_string("hello"));
    arr->append(jasson_integer(42));
    arr->append(jasson_false());

    assert(arr->size() == 3);
    assert(arr->at(0)->is_string());
    assert(arr->at(1)->is_integer());
    assert(json_is_false(*arr->at(2)));

    std::cout << "Array test passed!" << std::endl;
}

void test_string() {
    auto str = jasson_string("test");
    assert(str->is_string());
    assert(str->value() == "test");

    std::cout << "String test passed!" << std::endl;
}

void test_number() {
    auto int_num = jasson_integer(42);
    assert(int_num->is_integer());
    assert(int_num->value() == 42);

    auto real_num = jasson_real(3.14);
    assert(real_num->is_real());
    assert(real_num->value() == 3.14);

    assert(json_equals(*int_num, *jasson_integer(42)));
    assert(!json_equals(*int_num, *real_num));

    std::cout << "Number test passed!" << std::endl;
}

void test_boolean() {
    auto bool_true = jasson_true();
    assert(bool_true->is_boolean());
    assert(json_is_true(*bool_true));

    auto bool_false = jasson_false();
    assert(bool_false->is_boolean());
    assert(json_is_false(*bool_false));

    assert(json_equals(*bool_true, *jasson_true()));
    assert(!json_equals(*bool_true, *bool_false));

    std::cout << "Boolean test passed!" << std::endl;
}

void test_null() {
    auto null_val = jasson_null();
    assert(null_val->is_null());
    assert(json_equals(*null_val, *jasson_null()));

    std::cout << "Null test passed!" << std::endl;
}

void test_builder() {
    auto obj = json_builder()
        .object()
        .add("name", "John")
        .add("age", static_cast<int64_t>(30))
        .add("active", true)
        .build();

    assert(obj->is_object());
    auto obj_cast = std::static_pointer_cast<JsonObject>(obj);
    assert(obj_cast->get("name")->is_string());
    assert(obj_cast->get("age")->is_integer());
    assert(json_is_true(*obj_cast->get("active")));

    std::cout << "Builder test passed!" << std::endl;
}

void test_array_builder() {
    auto arr = json_builder()
        .array()
        .append("hello")
        .append(static_cast<int64_t>(42))
        .append(false)
        .build();

    assert(arr->is_array());
    auto arr_cast = std::static_pointer_cast<JsonArray>(arr);
    assert(arr_cast->at(0)->is_string());
    assert(arr_cast->at(1)->is_integer());
    assert(json_is_false(*arr_cast->at(2)));

    std::cout << "Array builder test passed!" << std::endl;
}

void test_literal_operators() {
    auto str = "hello"_json;
    assert(str->is_string());
    assert(str->value() == "hello");

    auto num = 42_json;
    assert(num->is_integer());
    assert(num->value() == 42);

    auto real = 3.14_json;
    assert(real->is_real());
    assert(real->value() == 3.14);

    std::cout << "Literal operators test passed!" << std::endl;
}

int main() {
    test_object();
    test_array();
    test_string();
    test_number();
    test_boolean();
    test_null();
    test_builder();
    test_array_builder();
    test_literal_operators();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
