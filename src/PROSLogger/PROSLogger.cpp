#include "PROSLogger/PROSLogger.hpp"
#include "PROSLogger/LogManager.hpp"

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
