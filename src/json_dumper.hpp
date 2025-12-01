#ifndef JSON_DUMPER_HPP
#define JSON_DUMPER_HPP

#include "json_value.hpp"
#include <string>
#include <fstream>
#include <stdexcept>

namespace jasson {

class JsonDumper {
public:
    static void dump(const JsonValuePtr& value, const std::string& filename, bool pretty = false) {
        if (!value) {
            throw std::runtime_error("Cannot dump null value");
        }
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        
        if (pretty) {
            file << value->to_pretty_string();
        } else {
            file << value->to_string();
        }
        
        file.close();
    }
    
    static std::string dumps(const JsonValuePtr& value, bool pretty = false) {
        if (!value) {
            return "null";
        }
        
        if (pretty) {
            return value->to_pretty_string();
        } else {
            return value->to_string();
        }
    }
};

// Convenience functions
inline void json_dump(const JsonValuePtr& value, const std::string& filename, bool pretty = false) {
    JsonDumper::dump(value, filename, pretty);
}

inline std::string json_dumps(const JsonValuePtr& value, bool pretty = false) {
    return JsonDumper::dumps(value, pretty);
}

} // namespace jasson

#endif // JSON_DUMPER_HPP
