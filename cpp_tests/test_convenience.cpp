#include "../jasson_cpp/json_convenience.hpp"
#include "../jasson_cpp/json_value.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <optional>
#include <variant>

using namespace jasson;

// Custom test type
struct Person {
    std::string name;
    int age;
    
    JsonValuePtr to_json() const {
        auto obj = json_object();
        obj->set("name", json_string(name));
        obj->set("age", json_integer(age));
        return obj;
    }
    
    static Person from_json(const JsonValue& j) {
        if (!j.is_object()) {
            throw std::runtime_error("Expected JSON object");
        }
        const auto& obj = static_cast<const JsonObject&>(j);
        Person p;
        p.name = static_cast<const JsonString&>(*obj.get("name")).value();
        p.age = static_cast<const JsonInteger&>(*obj.get("age")).value();
        return p;
    }
};

// Specialize type traits for Person
namespace jasson {
template<>
struct is_json_serializable<Person> : std::true_type {};

template<>
struct is_json_deserializable<Person> : std::true_type {};
} // namespace jasson

int main() {
    try {
        // Test parse and stringify
        std::cout << "Testing parse and stringify..." << std::endl;
        auto json_str = R"({"name": "John", "age": 30, "active": true})";
        auto value = parse(json_str);
        assert(value->is_object());
        std::string str = stringify(*value);
        std::cout << "Parsed and stringified: " << str << std::endl;

        // Test std::to_json and std::from_json for built-in types
        std::cout << "\nTesting built-in type conversions..." << std::endl;
        auto bool_val = to_json(true);
        assert(bool_val->is_boolean());
        bool b = from_json_value<bool>(*bool_val);
        assert(b == true);

        auto int_val = to_json(42);
        assert(int_val->is_integer());
        int i = from_json_value<int>(*int_val);
        assert(i == 42);

        auto str_val = to_json(std::string("hello"));
        assert(str_val->is_string());
        std::string s = from_json_value<std::string>(*str_val);
        assert(s == "hello");

        // Test container support
        std::cout << "\nTesting container support..." << std::endl;
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto vec_json = to_json(vec);
        assert(vec_json->is_array());
        auto vec2 = from_json_vector<int>(*vec_json);
        assert(vec == vec2);

        std::unordered_map<std::string, int> map{{"a", 1}, {"b", 2}};
        auto map_json = to_json(map);
        assert(map_json->is_object());
        auto map2 = from_json_map<int>(*map_json);
        assert(map == map2);

        // Test optional support
        std::cout << "\nTesting optional support..." << std::endl;
        std::optional<int> opt1 = 42;
        auto opt1_json = to_json(opt1);
        assert(opt1_json->is_integer());
        auto opt2 = from_json_optional<int>(*opt1_json);
        assert(opt2.has_value() && opt2.value() == 42);

        std::optional<int> opt3;
        auto opt3_json = to_json(opt3);
        assert(opt3_json->is_null());
        auto opt4 = from_json_optional<int>(*opt3_json);
        assert(!opt4.has_value());

        // Test variant support
        std::cout << "\nTesting variant support..." << std::endl;
        std::variant<int, std::string> var1 = 42;
        auto var1_json = to_json(var1);
        assert(var1_json->is_integer());

        std::variant<int, std::string> var2 = std::string("hello");
        auto var2_json = to_json(var2);
        assert(var2_json->is_string());

        // Test custom type
        std::cout << "\nTesting custom type..." << std::endl;
        Person p1{"Alice", 25};
        auto p1_json = p1.to_json();
        auto p2 = Person::from_json(*p1_json);
        assert(p1.name == p2.name && p1.age == p2.age);

        // Test type traits
        std::cout << "\nTesting type traits..." << std::endl;
        static_assert(is_json_serializable_v<int>);
        static_assert(is_json_deserializable_v<int>);
        static_assert(is_json_compatible_v<int>);
        static_assert(is_json_serializable_v<Person>);
        static_assert(is_json_deserializable_v<Person>);
        static_assert(is_json_compatible_v<Person>);

        // Test concepts
        std::cout << "\nTesting concepts..." << std::endl;
        static_assert(JsonSerializable<int>);
        static_assert(JsonDeserializable<int>);
        static_assert(JsonCompatible<int>);
        static_assert(JsonSerializable<Person>);
        static_assert(JsonDeserializable<Person>);
        static_assert(JsonCompatible<Person>);

        std::cout << "\nAll tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
