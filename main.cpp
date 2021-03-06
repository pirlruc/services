#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
// #define IMPROC_BENCHMARK_DISABLED

#include <improc/services/logger_services.hpp>
#include <improc/infrastructure/logging/logger_infrastructure.hpp>
#include <improc/infrastructure/benchmark/benchmark_singleton.hpp>

#include <improc/services/factory/variant_factory_pattern.hpp>
#include <improc/services/base_service.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>
#include <variant>
#include <functional>
#include <iostream>

template<class ... DataType> 
struct VariantOverload : DataType ... 
{ 
    using DataType::operator() ...; 
};
template<class ... DataType> VariantOverload(DataType ...) -> VariantOverload<DataType ...>;

class BenchmarkTest : public improc::BenchmarkSingleton<BenchmarkTest>
{
    friend std::shared_ptr<BenchmarkTest> LoggerSingleton::get(const std::string& logger_name);
    private:
        BenchmarkTest(std::shared_ptr<spdlog::logger>&& logger) : BenchmarkSingleton(logger) {}
};

int add_int(int a) {return a + 1;}
int add2_int(int a, int b) {return a + b;}
float multiply(float a) {return a * 2;}
float multiply2(float a, int b) {return a * b;}

std::shared_ptr<int> add_int_ptr(int a) {return std::make_shared<int>(a + 1);}
std::shared_ptr<int> add2_int_ptr(int a, int b) {return std::make_shared<int>(a + b);}

int main()
{
    auto console_infrastructure_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_infrastructure_sink->set_level(spdlog::level::info);
    auto file_infrastructure_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/infrastructure.txt", true);
    file_infrastructure_sink->set_level(spdlog::level::trace);
    spdlog::logger logger_infrastructure("infrastructure", {console_infrastructure_sink,file_infrastructure_sink});
    logger_infrastructure.set_level(spdlog::level::trace);
    spdlog::register_logger(std::make_shared<spdlog::logger>(logger_infrastructure));
    improc::InfrastructureLogger::get("infrastructure");

    IMPROC_INFRASTRUCTURE_LOGGER_TRACE      ("Trace test");
    IMPROC_INFRASTRUCTURE_LOGGER_DEBUG      ("Debug test");
    IMPROC_INFRASTRUCTURE_LOGGER_INFO       ("Info test");
    IMPROC_INFRASTRUCTURE_LOGGER_WARN       ("Warn test");
    IMPROC_INFRASTRUCTURE_LOGGER_ERROR      ("Error test");
    IMPROC_INFRASTRUCTURE_LOGGER_CRITICAL   ("Critical test");

    auto console_services_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_services_sink->set_level(spdlog::level::info);
    auto file_services_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/services.txt", true);
    file_services_sink->set_level(spdlog::level::trace);
    spdlog::logger logger_services("services", {console_services_sink,file_services_sink});
    logger_services.set_level(spdlog::level::trace);
    spdlog::register_logger(std::make_shared<spdlog::logger>(logger_services));
    improc::ServicesLogger::get("services");

    IMPROC_SERVICES_LOGGER_TRACE      ("Trace test");
    IMPROC_SERVICES_LOGGER_DEBUG      ("Debug test");
    IMPROC_SERVICES_LOGGER_INFO       ("Info test");
    IMPROC_SERVICES_LOGGER_WARN       ("Warn test");
    IMPROC_SERVICES_LOGGER_ERROR      ("Error test");
    IMPROC_SERVICES_LOGGER_CRITICAL   ("Critical test");

    auto benchmark_logger = spdlog::basic_logger_mt("benchmark", "logs/benchmark.txt");
    BenchmarkTest::get("benchmark");
    std::unordered_set<std::string> keys {"test","out"};
    BenchmarkTest::get()->AddKeys(keys);
    IMPROC_BENCHMARK_SET_CONTENT(BenchmarkTest::get(),"test","test_bench");
    IMPROC_BENCHMARK_SET_CONTENT(BenchmarkTest::get(),"out" ,true);
    IMPROC_BENCHMARK_WRITE_LINE(BenchmarkTest::get());
    return 0;
}