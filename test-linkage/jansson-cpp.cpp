#include "jansson-cpp.hpp"
#include <sstream>
#include <iomanip>

namespace jasson {

// Helper function to escape JSON strings
std::string escape_json_string(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (c >= 0x20 && c <= 0x7E) {
                    result += c;
                } else {
                    std::stringstream ss;
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (unsigned char)c;
                    result += ss.str();
                }
        }
    }
    return result;
}

std::string JsonString::dump(int indent) const {
    return "\"" + escape_json_string(value_) + "\"";
}

json_t* JsonString::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonInteger::dump(int indent) const {
    return std::to_string(value_);
}

json_t* JsonInteger::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonReal::dump(int indent) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value_;
    std::string result = oss.str();
    // Remove trailing zeros
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    result.erase(result.find_last_not_of('.') + 1, std::string::npos);
    return result;
}

json_t* JsonReal::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonBoolean::dump(int indent) const {
    return value_ ? "true" : "false";
}

json_t* JsonBoolean::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonNull::dump(int indent) const {
    return "null";
}

json_t* JsonNull::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonArray::dump(int indent) const {
    if (indent < 0) {
        // Compact format
        std::string result = "[";
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (i > 0) result += ",";
            result += elements_[i]->dump(indent);
        }
        result += "]";
        return result;
    } else {
        // Pretty format
        std::string result = "[\n";
        std::string indent_str(indent, ' ');
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (i > 0) result += ",\n";
            result += indent_str + "  " + elements_[i]->dump(indent);
        }
        result += "\n" + indent_str + "]";
        return result;
    }
}

json_t* JsonArray::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

std::string JsonObject::dump(int indent) const {
    if (indent < 0) {
        // Compact format
        std::string result = "{";
        bool first = true;
        for (const auto& member : members_) {
            if (!first) result += ",";
            first = false;
            result += "\"" + escape_json_string(member.first) + "\":";
            result += member.second->dump(indent);
        }
        result += "}";
        return result;
    } else {
        // Pretty format
        std::string result = "{\n";
        std::string indent_str(indent, ' ');
        bool first = true;
        for (const auto& member : members_) {
            if (!first) result += ",\n";
            first = false;
            result += indent_str + "  \"" + escape_json_string(member.first) + "\": ";
            result += member.second->dump(indent);
        }
        result += "\n" + indent_str + "}";
        return result;
    }
}

json_t* JsonObject::to_c_json() const {
    // Return nullptr since we can't create C JSON objects
    return nullptr;
}

} // namespace jasson
