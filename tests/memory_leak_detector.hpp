#ifndef JANSSON_MEMORY_LEAK_DETECTOR_HPP
#define JANSSON_MEMORY_LEAK_DETECTOR_HPP

#include <cstddef>
#include <cstdlib>
#include <unordered_map>
#include <mutex>
#include <string>

namespace jasson {
namespace test {

class MemoryLeakDetector {
public:
    static MemoryLeakDetector& getInstance() {
        static MemoryLeakDetector instance;
        return instance;
    }

    void startMonitoring() {
        std::lock_guard<std::mutex> lock(mutex_);
        allocations_.clear();
        monitoring_ = true;
    }

    void stopMonitoring() {
        std::lock_guard<std::mutex> lock(mutex_);
        monitoring_ = false;
    }

    void recordAllocation(void* ptr, size_t size, const std::string& file, int line) {
        if (!monitoring_) return;
        std::lock_guard<std::mutex> lock(mutex_);
        allocations_[ptr] = AllocationInfo{size, file, line};
    }

    void recordDeallocation(void* ptr) {
        if (!monitoring_) return;
        std::lock_guard<std::mutex> lock(mutex_);
        allocations_.erase(ptr);
    }

    bool hasLeaks() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return !allocations_.empty();
    }

    size_t getLeakCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return allocations_.size();
    }

    std::string getLeakReport() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string report = "Memory leak report:\n";
        for (const auto& [ptr, info] : allocations_) {
            report += "  Leak at " + std::to_string(reinterpret_cast<uintptr_t>(ptr)) + 
                     " (size: " + std::to_string(info.size) + ") in " + 
                     info.file + ":" + std::to_string(info.line) + "\n";
        }
        return report;
    }

private:
    struct AllocationInfo {
        size_t size;
        std::string file;
        int line;
    };

    MemoryLeakDetector() : monitoring_(false) {}
    ~MemoryLeakDetector() {
        if (hasLeaks()) {
            // In a real implementation, you might want to log this
            // For now, we'll just clear the allocations
            allocations_.clear();
        }
    }

    mutable std::mutex mutex_;
    std::unordered_map<void*, AllocationInfo> allocations_;
    bool monitoring_;
};

// Simple memory leak detection functions
inline bool hasLeaks() {
    return MemoryLeakDetector::getInstance().hasLeaks();
}

inline std::string getLeakReport() {
    return MemoryLeakDetector::getInstance().getLeakReport();
}

} // namespace test
} // namespace jasson

#endif // JANSSON_MEMORY_LEAK_DETECTOR_HPP
