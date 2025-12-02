#ifndef JSON_LOADER_HPP
#define JSON_LOADER_HPP

#include "json_value.hpp"
#include "json_factory.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

namespace jasson {

class JsonLoader {
public:
    static JsonValuePtr load(const std::string& json_str) {
        JsonLoader loader;
        loader.input_ = json_str;
        loader.pos_ = 0;
        return loader.parse_value();
    }
    
private:
    std::string input_;
    size_t pos_;
    
    void skip_whitespace() {
        while (pos_ < input_.size() && std::isspace(input_[pos_])) {
            ++pos_;
        }
    }
    
    char peek() const {
        if (pos_ >= input_.size()) {
            throw std::runtime_error("Unexpected end of input");
        }
        return input_[pos_];
    }
    
    char consume() {
        char c = peek();
        ++pos_;
        return c;
    }
    
    bool match(char expected) {
        skip_whitespace();
        if (peek() == expected) {
            consume();
            return true;
        }
        return false;
    }
    
    JsonValuePtr parse_value() {
        skip_whitespace();
        char c = peek();
        
        switch (c) {
            case 'n':
                return parse_null();
            case 't':
            case 'f':
                return parse_boolean();
            case '"':
                return parse_string();
            case '[':
                return parse_array();
            case '{':
                return parse_object();
            case '-':
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return parse_number();
            default:
                throw std::runtime_error("Unexpected character: " + std::string(1, c));
        }
    }
    
    JsonValuePtr parse_null() {
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return jasson_null();
        }
        throw std::runtime_error("Invalid null value");
    }
    
    JsonValuePtr parse_boolean() {
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return jasson_true();
        } else if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return jasson_false();
        }
        throw std::runtime_error("Invalid boolean value");
    }
    
    JsonValuePtr parse_string() {
        if (consume() != '"') {
            throw std::runtime_error("Expected string");
        }
        
        std::string result;
        while (peek() != '"') {
            char c = consume();
            if (c == '\\') {
                c = consume();
                switch (c) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default:
                        throw std::runtime_error("Invalid escape sequence");
                }
            } else {
                result += c;
            }
        }
        
        consume(); // Closing quote
        return jasson_string(std::move(result));
    }
    
    JsonValuePtr parse_number() {
        size_t start = pos_;
        
        if (peek() == '-') {
            consume();
        }
        
        if (peek() == '0') {
            consume();
        } else {
            while (std::isdigit(peek())) {
                consume();
            }
        }
        
        bool is_real = false;
        if (peek() == '.') {
            is_real = true;
            consume();
            while (std::isdigit(peek())) {
                consume();
            }
        }
        
        if (peek() == 'e' || peek() == 'E') {
            is_real = true;
            consume();
            if (peek() == '+' || peek() == '-') {
                consume();
            }
            while (std::isdigit(peek())) {
                consume();
            }
        }
        
        std::string num_str = input_.substr(start, pos_ - start);
        
        if (is_real) {
            return jasson_real(std::stod(num_str));
        } else {
            return jasson_integer(std::stoll(num_str));
        }
    }
    
    JsonValuePtr parse_array() {
        if (consume() != '[') {
            throw std::runtime_error("Expected array");
        }
        
        auto array = jasson_array();
        
        skip_whitespace();
        if (peek() == ']') {
            consume();
            return array;
        }
        
        while (true) {
            array->as_array()->append(parse_value());
            
            skip_whitespace();
            if (peek() == ']') {
                consume();
                break;
            } else if (peek() == ',') {
                consume();
            } else {
                throw std::runtime_error("Expected ',' or ']' in array");
            }
        }
        
        return array;
    }
    
    JsonValuePtr parse_object() {
        if (consume() != '{') {
            throw std::runtime_error("Expected object");
        }
        
        auto object = jasson_object();
        
        skip_whitespace();
        if (peek() == '}') {
            consume();
            return object;
        }
        
        while (true) {
            skip_whitespace();
            
            // Parse key
            if (peek() != '"') {
                throw std::runtime_error("Expected string key");
            }
            auto key_value = parse_string();
            std::string key = key_value->as_string()->value();
            
            skip_whitespace();
            if (consume() != ':') {
                throw std::runtime_error("Expected ':' after object key");
            }
            
            // Parse value
            auto value = parse_value();
            object->as_object()->add(key, value);
            
            skip_whitespace();
            if (peek() == '}') {
                consume();
                break;
            } else if (peek() == ',') {
                consume();
            } else {
                throw std::runtime_error("Expected ',' or '}' in object");
            }
        }
        
        return object;
    }
};

// Convenience function
inline JsonValuePtr json_load(const std::string& json_str) {
    return JsonLoader::load(json_str);
}

} // namespace jasson

#endif // JSON_LOADER_HPP
