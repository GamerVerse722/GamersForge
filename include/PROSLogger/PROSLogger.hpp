#pragma once

#include <functional>
#include <string>

namespace PROSLogger {
    enum LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    inline std::string levelToString(LogLevel level) {
        switch (level) {
            case DEBUG: return "Debug";
            case INFO: return "Info";
            case WARN: return "Warn";
            case ERROR: return "Error";
            default: return "Unknown";
        }
    }

    inline std::string levelToColor(const LogLevel level) {
        switch (level) {
            case DEBUG: return "\033[0;34m";
            case INFO: return "\033[0;32m";
            case WARN: return "\033[0;33m";
            case ERROR: return "\033[0;31m";
            default: return "";
        }
    }

    struct LoggerEvent {
        LogLevel level;
        std::string groupId;
        std::string message;
        std::string formattedMessage;
    };

    typedef std::function<void(const LoggerEvent)> Subscriber;

    class Logger {
        private:
            const std::string id;
        public:
            explicit Logger(std::string loggerId): id(loggerId) {}

            void log(const std::string& message, LogLevel level) const;

            void debug(const std::string &message) const;
            void info(const std::string &message) const;
            void warn(const std::string &message) const;
            void error(const std::string &message) const;
    };
}

namespace PROSLogger::Manager {
    void setLevel(LogLevel level);
    LogLevel getLevel();

    void setConsoleEnabled(bool enabled);
    bool isConsoleEnabled();

    void subscribe(const Subscriber& callback);
    void notify(const LoggerEvent& event);
}