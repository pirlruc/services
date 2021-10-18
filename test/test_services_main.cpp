#include <gtest/gtest.h>

#include <improc/infrastructure/benchmark_singleton.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/infrastructure/logger_infrastructure.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>

class BenchmarkDetector : public improc::BenchmarkSingleton<BenchmarkDetector>
{
    friend std::shared_ptr<BenchmarkDetector> LoggerSingleton::get(const std::string& logger_name);
    private:
        BenchmarkDetector(std::shared_ptr<spdlog::logger>&& logger) : BenchmarkSingleton(logger) {}
};

class IncrementTestDS : public improc::StringKeyHeterogeneousBaseService
{
    public:
        IncrementTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteFields("IncrementTestDS",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_subtract_ = 0;

    public:
        SubtractTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for subtract service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_subtract_ = service_field_iter->asInt();
                    break;
                }
            }
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number: {}",this->number_to_subtract_);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) - this->number_to_subtract_;
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteFields("SubtractTestDS",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class MultiplyTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for multiply service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_multiply_ = service_field_iter->asInt();
                    break;
                }
            }
            spdlog::info("--- MULTIPLY SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number: {}",this->number_to_multiply_);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            spdlog::info("Multiply Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteFields("MultiplyTestDS",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

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

    auto benchmark_logger = spdlog::basic_logger_mt("benchmark", "logs/benchmark.txt");

    improc::InfrastructureLogger::get("infrastructure");
    improc::ServicesLogger::get("services");
    BenchmarkDetector::get("benchmark");

    improc::JsonFile json_file {"../../test/data/test_ex1.json"};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Add("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Add("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Add("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    std::cout << "Computed: " << std::any_cast<int>(cntxt.Get("ori")) << std::endl;
    std::cout << "Expected: " << 4 << std::endl;

    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),4);
    improc::File log_infrastructure_file  {"logs/infrastructure.txt"};
    improc::File log_services_file        {"logs/services.txt"};
    improc::File benchmark_file           {"logs/benchmark.txt"};
    EXPECT_TRUE(log_infrastructure_file.Exists());
    EXPECT_TRUE(log_services_file.Exists());
    EXPECT_TRUE(benchmark_file.Exists());
}