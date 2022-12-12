#include <gtest/gtest.h>

#include <base_service_tests_def.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/infrastructure/logging/logger_infrastructure.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>

#include <improc_services_test_config.hpp>

TEST(ServicesMain,TestMain)
{
    auto console_infrastructure_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_infrastructure_sink->set_level(spdlog::level::info);
    auto file_infrastructure_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/infrastructure.txt", true);
    file_infrastructure_sink->set_level(spdlog::level::trace);
    spdlog::logger logger_infrastructure("infrastructure", {console_infrastructure_sink,file_infrastructure_sink});
    logger_infrastructure.set_level(spdlog::level::trace);
    spdlog::register_logger(std::make_shared<spdlog::logger>(logger_infrastructure));

    auto console_services_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_services_sink->set_level(spdlog::level::info);
    auto file_services_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/services.txt", true);
    file_services_sink->set_level(spdlog::level::trace);
    spdlog::logger logger_services("services", {console_services_sink, file_services_sink});
    logger_services.set_level(spdlog::level::trace);
    spdlog::register_logger(std::make_shared<spdlog::logger>(logger_services));

    spdlog::basic_logger_mt("benchmark", "logs/benchmark.txt");

    improc::InfrastructureLogger::get("infrastructure");
    improc::ServicesLogger::get("services_logger");
    BenchmarkDetector::get("benchmark");
    std::unordered_set<std::string> keys {"service","output"};
    BenchmarkDetector::get()->AddKeys(keys);

    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    std::cout << "Computed: " << std::any_cast<int>(cntxt.Get("ori")) << std::endl;
    std::cout << "Expected: " << 4 << std::endl;

    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),4);
    improc::File log_infrastructure_file  {std::string("logs/infrastructure.txt")};
    improc::File log_services_file        {std::string("logs/services.txt")};
    improc::File benchmark_file           {std::string("logs/benchmark.txt")};
    EXPECT_TRUE(log_infrastructure_file.Exists());
    EXPECT_TRUE(log_services_file.Exists());
    EXPECT_TRUE(benchmark_file.Exists());
}