#include "json_unpacker.hpp"
#include "json_factory.hpp"
#include "type_traits.hpp"
#include <sstream>
#include <regex>
#include <type_traits>
#include <stdexcept>

namespace jasson {

namespace detail {
    class UnpackParser {
    public:
        struct Token {
            char type = '\0';
            bool optional = false;
            bool strict = true;
            size_t index = 0;
        };

        explicit UnpackParser(std::string_view format) : format_(format), pos_(0) {}

        Token next_token() {
            skip_whitespace();
            if (pos_ >= format_.size()) return Token{};

            Token token;
            token.type = format_[pos_++];
            token.index = pos_ - 1;

            // Check for modifiers
            if (pos_ < format_.size()) {
                if (format_[pos_] == '?') {
                    token.optional = true;
                    pos_++;
                } else if (format_[pos_] == '*') {
                    token.optional = true;
                    token.strict = false;
                    pos_++;
                } else if (format_[pos_] == '!') {
                    token.strict = true;
                    pos_++;
                }
            }

            return token;
        }

        bool has_error() const { return has_error_; }
        std::string error_message() const { return error_message_; }

    private:
        void skip_whitespace() {
            while (pos_ < format_.size() && 
                   (format_[pos_] == ' ' || format_[pos_] == '\t' || 
                    format_[pos_] == '\n' || format_[pos_] == ',')) {
                pos_++;
            }
        }

        std::string_view format_;
        size_t pos_;
        bool has_error_ = false;
        std::string error_message_;
    };
}

template<typename T>
bool JsonUnpacker::unpack_value(const Value& json, T& target) {
    using DecayedT = std::decay_t<T>;

    if constexpr (std::is_same_v<DecayedT, bool>) {
        if (!json.is_boolean()) return false;
        target = json.as_boolean();
        return true;
    } else if constexpr (std::is_integral_v<DecayedT>) {
        if (!json.is_integer()) return false;
        target = static_cast<DecayedT>(json.as_integer());
        return true;
    } else if constexpr (std::is_floating_point_v<DecayedT>) {
        if (!json.is_number()) return false;
        target = static_cast<DecayedT>(json.as_number());
        return true;
    } else if constexpr (std::is_same_v<DecayedT, std::string>) {
        if (!json.is_string()) return false;
        target = json.as_string();
        return true;
    } else if constexpr (std::is_same_v<DecayedT, const char*>) {
        if (!json.is_string()) return false;
        target = json.as_cstring();
        return true;
    } else if constexpr (is_specialization_v<DecayedT, std::optional>) {
        return unpack_optional(json, target);
    } else if constexpr (is_specialization_v<DecayedT, std::variant>) {
        return unpack_variant(json, target);
    } else {
        static_assert(always_false_v<DecayedT>, "Unsupported type for unpacking");
    }
}

template<typename T>
bool JsonUnpacker::unpack_array(const Value& json, std::vector<T>& target) {
    if (!json.is_array()) return false;
    
    const auto& array = json.as_array();
    target.reserve(array.size());
    
    for (const auto& item : array) {
        T value;
        if (!unpack_value(item, value)) return false;
        target.push_back(std::move(value));
    }
    return true;
}

template<typename... Args>
bool JsonUnpacker::unpack_object(const Value& json, Args&&... args) {
    static_assert(sizeof...(Args) % 2 == 0, "Object must have key-value pairs");
    if (!json.is_object()) return false;
    
    const auto& object = json.as_object();
    bool success = true;
    
    auto process_pair = [&](auto&& key, auto&& value) {
        std::string key_str(key);
        auto it = object.find(key_str);
        if (it == object.end()) return false;
        return unpack_value(*it->second, value);
    };

    size_t index = 0;
    auto args_tuple = std::forward_as_tuple(args...);
    
    auto process_args = [&](auto&&... tuple_args) {
        for (; index < sizeof...(Args) && success; index += 2) {
            success = process_pair(std::get<index>(tuple_args), 
                                 std::get<index + 1>(tuple_args));
        }
    };
    
    std::apply(process_args, args_tuple);
    return success;
}

template<typename T>
bool JsonUnpacker::unpack_optional(const Value& json, std::optional<T>& target) {
    if (json.is_null()) {
        target.reset();
        return true;
    }
    
    T value;
    if (!unpack_value(json, value)) return false;
    target = std::move(value);
    return true;
}

template<typename... Args>
bool JsonUnpacker::unpack_variant(const Value& json, std::variant<Args...>& target) {
    return std::visit([&](auto&& alternative) {
        using T = std::decay_t<decltype(alternative)>;
        T value;
        if (!unpack_value(json, value)) return false;
        target = std::move(value);
        return true;
    }, target);
}

template<typename... Args>
bool JsonUnpacker::unpack(const Value& root, std::string_view format, Args&&... args) {
    Error error;
    return unpack_ex(root, error, 0, format, std::forward<Args>(args)...);
}

template<typename... Args>
bool JsonUnpacker::unpack_ex(const Value& root, Error& error, size_t flags,
                            std::string_view format, Args&&... args) {
    detail::UnpackParser parser(format);
    
    try {
        auto tokens = std::vector<detail::UnpackParser::Token>();
        while (true) {
            auto token = parser.next_token();
            if (token.type == '\0') break;
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            error.set("<format>", JSON_ERROR_INVALID_FORMAT, "Empty format string");
            return false;
        }

        // Process arguments based on format tokens
        size_t arg_index = 0;
        bool success = true;

        auto process_token = [&](auto&& token, auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            
            switch (token.type) {
                case '{':
                    return unpack_object(root, std::forward<Args>(args)...);
                case '[':
                    return unpack_array(root, arg);
                case 's':
                    return unpack_value(root, arg);
                case 'n':
                    return root.is_null();
                case 'b':
                    return unpack_value(root, arg);
                case 'i':
                case 'I':
                    return unpack_value(root, arg);
                case 'f':
                case 'F':
                    return unpack_value(root, arg);
                case 'O':
                case 'o':
                    return unpack_value(root, arg);
                default:
                    error.set("<format>", JSON_ERROR_INVALID_FORMAT,
                             "Unexpected format character '%c'", token.type);
                    return false;
            }
        };

        auto args_tuple = std::forward_as_tuple(args...);
        
        auto process_args = [&](auto&&... tuple_args) {
            for (size_t i = 0; i < tokens.size() && success; ++i) {
                success = std::apply([&](auto&&... tuple_args) {
                    return process_token(tokens[i], std::get<arg_index>(tuple_args));
                }, args_tuple);
                arg_index++;
            }
        };
        
        std::apply(process_args, args_tuple);
        return success;

    } catch (const std::exception& e) {
        error.set("<internal>", JSON_ERROR_OUT_OF_MEMORY, "Exception: %s", e.what());
        return false;
    }
}

// Explicit template instantiations for common types
template bool JsonUnpacker::unpack<>(const JsonValue&, std::string_view);
template bool JsonUnpacker::unpack<int>(const JsonValue&, std::string_view, int&);
template bool JsonUnpacker::unpack<double>(const JsonValue&, std::string_view, double&);
template bool JsonUnpacker::unpack<std::string>(const JsonValue&, std::string_view, std::string&);
template bool JsonUnpacker::unpack<bool>(const JsonValue&, std::string_view, bool&);

} // namespace jasson
