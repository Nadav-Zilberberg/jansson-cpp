#include "jasson_cpp/jasson.hpp"
#include <iostream>
#include <cassert>

using namespace jasson;

void test_object() {
    auto obj = json_object();
    assert(obj->is_object());
    assert(obj->size() == 0);
    
    obj->set("name", json_string("John"));
    obj->set("age", json_integer(30));
    obj->set("active", json_true());
    
    assert(obj->size() == 3);
    assert(obj->get("name")->is_string());
    assert(obj->get("age")->is_integer());
    assert(obj->get("active")->is_true());
    
    auto copy = json_copy(obj);
    assert(json_equal(obj, copy));
    
    std::cout << "Object tests passed!\n";
}

void test_array() {
    auto arr = json_array();
    assert(arr->is_array());
    assert(arr->size() == 0);
    
    arr->append(json_string("hello"));
    arr->append(json_integer(42));
    arr->append(json_false());
    
    assert(arr->size() == 3);
    assert(arr->get(0)->is_string());
    assert(arr->get(1)->is_integer());
    assert(arr->get(2)->is_false());
    
    auto copy = json_deep_copy(arr);
    assert(json_equal(arr, copy));
    
    std::cout << "Array tests passed!\n";
}

void test_string() {
    auto str = json_string("test");
    assert(str->is_string());
    assert(str->value() == "test");
    assert(str->length() == 4);
    
    str->set_value("new value");
    assert(str->value() == "new value");
    
    std::cout << "String tests passed!\n";
}

void test_number() {
    auto int_num = json_integer(42);
    assert(int_num->is_integer());
    assert(int_num->int_value() == 42);
    
    auto real_num = json_real(3.14);
    assert(real_num->is_real());
    assert(real_num->real_value() == 3.14);
    
    assert(int_num->equals(*json_integer(42)));
    assert(!int_num->equals(*real_num));
    
    std::cout << "Number tests passed!\n";
}

void test_boolean() {
    auto bool_true = json_true();
    assert(bool_true->is_true());
    assert(bool_true->value() == true);
    
    auto bool_false = json_false();
    assert(bool_false->is_false());
    assert(bool_false->value() == false);
    
    assert(bool_true->equals(*json_true()));
    assert(!bool_true->equals(*bool_false));
    
    std::cout << "Boolean tests passed!\n";
}

void test_null() {
    auto null_val = json_null();
    assert(null_val->is_null());
    assert(null_val->equals(*json_null()));
    
    std::cout << "Null tests passed!\n";
}

int main() {
    test_object();
    test_array();
    test_string();
    test_number();
    test_boolean();
    test_null();
    
    std::cout << "All tests passed!\n";
    return 0;
}
