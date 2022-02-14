#ifndef IMPROC_SERVICES_LOGGER_SERVICES_HPP
#define IMPROC_SERVICES_LOGGER_SERVICES_HPP

#include <improc/improc_defs.hpp>
#include <improc/infrastructure/logging/logger_singleton.hpp>

namespace improc 
{
    /**
     * @brief Logger used for services library
     * 
     */
    class IMPROC_EXPORTS ServicesLogger : public LoggerSingleton<ServicesLogger>
    {
        friend std::shared_ptr<ServicesLogger> LoggerSingleton::get(const std::string& logger_name);
        private:
            ServicesLogger(std::shared_ptr<spdlog::logger>&&      logger) : LoggerSingleton(std::move(logger)) {}
            ServicesLogger(const std::shared_ptr<spdlog::logger>& logger) : LoggerSingleton(std::move(logger)) {}

        public:
            ServicesLogger(ServicesLogger&  that)       = delete;
            ServicesLogger(ServicesLogger&& that)       = delete;
            void operator=(const ServicesLogger&  that) = delete;
            void operator=(const ServicesLogger&& that) = delete;
    };
}

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#    define IMPROC_SERVICES_LOGGER_TRACE(...) IMPROC_LOGGER_TRACE(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_TRACE(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#    define IMPROC_SERVICES_LOGGER_DEBUG(...) IMPROC_LOGGER_DEBUG(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_DEBUG(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#    define IMPROC_SERVICES_LOGGER_INFO(...) IMPROC_LOGGER_INFO(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_INFO(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#    define IMPROC_SERVICES_LOGGER_WARN(...) IMPROC_LOGGER_WARN(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_WARN(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#    define IMPROC_SERVICES_LOGGER_ERROR(...) IMPROC_LOGGER_ERROR(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_ERROR(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#    define IMPROC_SERVICES_LOGGER_CRITICAL(...) IMPROC_LOGGER_CRITICAL(improc::ServicesLogger::get(), __VA_ARGS__)
#else
#    define IMPROC_SERVICES_LOGGER_CRITICAL(...) (void)0
#endif

#endif