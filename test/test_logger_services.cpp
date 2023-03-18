#include <gtest/gtest.h>

#include <improc/services/logger_services.hpp>

TEST(LoggerServices,TestLoggerLogging) {
    EXPECT_NE(improc::ServicesLogger::get()->data(),nullptr);
    IMPROC_SERVICES_LOGGER_TRACE("Test {} {}",1,2);
    IMPROC_SERVICES_LOGGER_DEBUG("Test {} {}",2,3);
    IMPROC_SERVICES_LOGGER_INFO ("Test {} {}",3,4);
    IMPROC_SERVICES_LOGGER_ERROR("Test {} {}",4,5);
    IMPROC_SERVICES_LOGGER_WARN ("Test {} {}",5,6);
    IMPROC_SERVICES_LOGGER_CRITICAL("Test {} {}",6,7);
}
