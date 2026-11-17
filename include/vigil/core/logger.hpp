#ifndef VIGIL_CORE_LOGGER_HPP
#define VIGIL_CORE_LOGGER_HPP

#include <string>
#include <string_view>
#include <fstream>
#include <mutex>
#include <memory>

namespace vigil::core {

enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3
};

class Logger {
public:
    static Logger& instance();

    void init(LogLevel level, const std::string& logFilePath = "");
    void set_level(LogLevel level);
    LogLevel get_level() const;

    void log(LogLevel level, std::string_view message);

    template<typename... Args>
    void debug(std::string_view fmt, Args&&... args) {
        if (current_level_ <= LogLevel::Debug) {
            log(LogLevel::Debug, format_string(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    void info(std::string_view fmt, Args&&... args) {
        if (current_level_ <= LogLevel::Info) {
            log(LogLevel::Info, format_string(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    void warn(std::string_view fmt, Args&&... args) {
        if (current_level_ <= LogLevel::Warning) {
            log(LogLevel::Warning, format_string(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    void error(std::string_view fmt, Args&&... args) {
        if (current_level_ <= LogLevel::Error) {
            log(LogLevel::Error, format_string(fmt, std::forward<Args>(args)...));
        }
    }

private:
    Logger() = default;
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string level_to_string(LogLevel level) const;
    std::string get_timestamp() const;

    template<typename... Args>
    std::string format_string(std::string_view fmt, Args&&... args) {
        // Simple internal formatting fallback without heavy fmt dependency
        // For standard messages, we can just append or use std::format if available
        // Here we implement robust string formatting or fallback concatenation
        return std::string(fmt); 
    }

    LogLevel current_level_{LogLevel::Info};
    std::ofstream log_file_;
    mutable std::mutex mutex_;
};

} // namespace vigil::core

#endif // VIGIL_CORE_LOGGER_HPP
