#include "PROSLogger/PROSLogger.hpp"

#include <format>
#include <iostream>


namespace PROSLogger {
    void Logger::log(const std::string& message, const LogLevel level) const {
        if (level < Manager::getLevel()) return;

        const std::string formatted_message = std::format("[00:00:00] [{}] [{}] {}", levelToString(level), id, message);

        if (Manager::isConsoleEnabled()) {
            std::cout << levelToColor(level) << formatted_message << std::endl;
        }

        Manager::notify({
            level,
            id,
            message,
            formatted_message
        });
    }

    void Logger::debug(const std::string &message) const { log(message, DEBUG); }
    void Logger::info(const std::string &message) const { log(message, INFO); }
    void Logger::warn(const std::string &message) const { log(message, WARN); }
    void Logger::error(const std::string &message) const { log(message, ERROR); }
}

namespace PROSLogger::Manager {
    LogLevel level = DEBUG;
    std::vector<Subscriber> subscribers;
    bool consoleEnabled = true;

    void setLevel(const LogLevel level) {
        Manager::level = level;
    }

    LogLevel getLevel() {
        return level;
    }

    void setConsoleEnabled(const bool enabled) {
        consoleEnabled = enabled;
    }

    bool isConsoleEnabled() {
        return consoleEnabled;
    }

    void subscribe(const Subscriber &callback) {
        subscribers.push_back(callback);
    }

    void notify(const LoggerEvent &event) {
        for (const auto& sub : subscribers) {
            sub(event);
        }
    }
}
