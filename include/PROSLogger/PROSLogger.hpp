#pragma once

#include <string>
#include <utility>

#include "LogLevel.hpp"

namespace PROSLogger {
    class Logger {
    private:
        const std::string id;
    public:
        explicit Logger(std::string loggerId): id(std::move(loggerId)) {}

        void log(const std::string& message, LogLevel level) const;

        void debug(const std::string &message) const;
        void info(const std::string &message) const;
        void warn(const std::string &message) const;
        void error(const std::string &message) const;
    };

}
