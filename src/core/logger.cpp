#include "vigil/core/logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace vigil::core {

Logger& Logger::instance() {
    static Logger logger_instance;
    return logger_instance;
}

Logger::~Logger() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::init(LogLevel level, const std::string& logFilePath) {
    std::lock_guard<std::mutex> lock(mutex_);
    current_level_ = level;
    if (!logFilePath.empty()) {
        log_file_.open(logFilePath, std::ios::out | std::ios::app);
    }
}

void Logger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    current_level_ = level;
}

LogLevel Logger::get_level() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return current_level_;
}

std::string Logger::level_to_string(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO ";
        case LogLevel::Warning: return "WARN ";
        case LogLevel::Error:   return "ERROR";
    }
    return "UNKNOWN";
}

std::string Logger::get_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    std::tm bt{};
#if defined(_MSC_VER)
    localtime_s(&bt, &in_time_t);
#else
    localtime_r(&in_time_t, &bt);
#endif
    ss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void Logger::log(LogLevel level, std::string_view message) {
    if (level < current_level_) {
        return;
    }

    std::string timestamp = get_timestamp();
    std::string lvl_str = level_to_string(level);

    std::lock_guard<std::mutex> lock(mutex_);
    
    // Console output
    std::ostream& out = (level >= LogLevel::Warning) ? std::cerr : std::cout;
    out << "[" << timestamp << "] [" << lvl_str << "] " << message << "\n";

    // File output if open
    if (log_file_.is_open()) {
        log_file_ << "[" << timestamp << "] [" << lvl_str << "] " << message << "\n";
        log_file_.flush();
    }
}

} // namespace vigil::core
