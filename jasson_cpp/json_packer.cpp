#include "json_packer.hpp"
#include "json_factory.hpp"
#include "type_traits.hpp"
#include <sstream>
#include <regex>
#include <stdexcept>

namespace jasson {

namespace detail {
    class FormatParser {
    public:
        struct Token {
            char type = '\0';
            bool optional = false;
            bool strict = true;
            size_t index = 0;
        };

        explicit FormatParser(std::string_view format) : format_(format), pos_(0) {}

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
std::unique_ptr<JsonValue> JsonPacker::pack_value(T&& value) {
    using DecayedT = std::decay_t<T>;

    if constexpr (std::is_same_v<DecayedT, bool>) {
        return value ? JsonFactory::create_true() : JsonFactory::create_false();
    } else if constexpr (std::is_integral_v<DecayedT>) {
        return JsonFactory::create_integer(static_cast<json_int_t>(value));
    } else if constexpr (std::is_floating_point_v<DecayedT>) {
        return JsonFactory::create_real(value);
    } else if constexpr (std::is_same_v<DecayedT, std::string> || 
                         std::is_same_v<DecayedT, const char*> ||
                         std::is_same_v<DecayedT, std::string_view>) {
        return JsonFactory::create_string(std::forward<T>(value));
    } else if constexpr (std::is_same_v<DecayedT, std::nullptr_t>) {
        return JsonFactory::create_null();
    } else if constexpr (is_specialization_v<DecayedT, std::optional>) {
        return pack_optional(std::forward<T>(value));
    } else if constexpr (is_specialization_v<DecayedT, std::variant>) {
        return std::visit([](auto&& arg) {
            return pack_value(std::forward<decltype(arg)>(arg));
        }, std::forward<T>(value));
    } else {
        static_assert(always_false_v<DecayedT>, "Unsupported type for packing");
    }
}

template<typename T>
std::unique_ptr<JsonValue> JsonPacker::pack_array(T&& container) {
    auto array = JsonFactory::create_array();
    for (auto&& item : container) {
        auto value = pack_value(std::forward<decltype(item)>(item));
        if (!value) return nullptr;
        array->append(std::move(value));
    }
    return array;
}

template<typename... Args>
std::unique_ptr<JsonValue> JsonPacker::pack_object(Args&&... args) {
    static_assert(sizeof...(Args) % 2 == 0, "Object must have key-value pairs");
    auto object = JsonFactory::create_object();
    
    auto process_pair = [&](auto&& key, auto&& value) {
        auto key_str = std::string(key);
        auto value_json = pack_value(std::forward<decltype(value)>(value));
        if (!value_json) return false;
        object->set(key_str, std::move(value_json));
        return true;
    };

    size_t index = 0;
    bool success = true;
    auto args_tuple = std::forward_as_tuple(args...);
    
    auto process_args = [&](auto&&... tuple_args) {
        for (; index < sizeof...(Args) && success; index += 2) {
            success = process_pair(std::get<index>(tuple_args), 
                                 std::get<index + 1>(tuple_args));
        }
    };
    
    std::apply(process_args, args_tuple);
    return success ? std::move(object) : nullptr;
}

template<typename T>
std::unique_ptr<JsonValue> JsonPacker::pack_optional(std::optional<T>&& opt) {
    if (opt.has_value()) {
        return pack_value(std::move(*opt));
    }
    return JsonFactory::create_null();
}

template<typename... Args>
std::unique_ptr<JsonValue> JsonPacker::pack(std::string_view format, Args&&... args) {
    Error error;
    return pack_ex(error, 0, format, std::forward<Args>(args)...);
}

template<typename... Args>
std::unique_ptr<JsonValue> JsonPacker::pack_ex(Error& error, size_t flags, 
                                               std::string_view format, Args&&... args) {
    detail::FormatParser parser(format);
    
    try {
        auto tokens = std::vector<detail::FormatParser::Token>();
        while (true) {
            auto token = parser.next_token();
            if (token.type == '\0') break;
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            error.set("<format>", JSON_ERROR_INVALID_FORMAT, "Empty format string");
            return nullptr;
        }

        // Process arguments based on format tokens
        return std::apply([&](auto&&... args) {
            size_t arg_index = 0;
            std::unique_ptr<JsonValue> result;

            auto process_token = [&](auto&& token) {
                using T = std::decay_t<decltype(args)>;
                if (arg_index >= sizeof...(Args)) {
                    error.set("<args>", JSON_ERROR_INVALID_FORMAT, "Too few arguments");
                    return false;
                }

                switch (token.type) {
                    case '{':
                        result = pack_object(std::forward<Args>(args)...);
                        break;
                    case '[':
                        result = pack_array(std::forward<Args>(args)...);
                        break;
                    case 's':
                        result = pack_value(std::forward<Args>(args)...);
                        break;
                    case 'n':
                        result = JsonFactory::create_null();
                        break;
                    case 'b':
                        result = pack_value(bool(std::forward<Args>(args)...));
                        break;
                    case 'i':
                    case 'I':
                        result = pack_value(std::forward<Args>(args)...);
                        break;
                    case 'f':
                    case 'F':
                        result = pack_value(std::forward<Args>(args)...);
                        break;
                    case 'O':
                    case 'o':
                        result = pack_value(std::forward<Args>(args)...);
                        break;
                    default:
                        error.set("<format>", JSON_ERROR_INVALID_FORMAT,
                                 "Unexpected format character '%c'", token.type);
                        return false;
                }
                arg_index++;
                return true;
            };

            for (auto&& token : tokens) {
                if (!process_token(token)) return nullptr;
            }

            return result;
        }, std::forward_as_tuple(args...));

    } catch (const std::exception& e) {
        error.set("<internal>", JSON_ERROR_OUT_OF_MEMORY, "Exception: %s", e.what());
        return nullptr;
    }
}

// Explicit template instantiations for common types
template std::unique_ptr<JsonValue> JsonPacker::pack<>(std::string_view);
template std::unique_ptr<JsonValue> JsonPacker::pack<int>(std::string_view, int);
template std::unique_ptr<JsonValue> JsonPacker::pack<double>(std::string_view, double);
template std::unique_ptr<JsonValue> JsonPacker::pack<std::string>(std::string_view, std::string);
template std::unique_ptr<JsonValue> JsonPacker::pack<bool>(std::string_view, bool);

} // namespace jasson
