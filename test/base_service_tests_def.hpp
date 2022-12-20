#ifndef IMPROC_TEST_BASE_SERVICE_DEFINITIONS_HPP
#define IMPROC_TEST_BASE_SERVICE_DEFINITIONS_HPP

#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/infrastructure/benchmark/benchmark_singleton.hpp>

class BenchmarkDetector : public improc::BenchmarkSingleton<BenchmarkDetector>
{
    friend std::shared_ptr<BenchmarkDetector> LoggerSingleton::get(const std::string& logger_name);
    private:
        explicit BenchmarkDetector(std::shared_ptr<spdlog::logger>&& logger) : BenchmarkSingleton(std::move(logger)) {}
};

class IncrementTest : public improc::StringKeyHeterogeneousBaseService
{
    public:
        IncrementTest() : improc::StringKeyHeterogeneousBaseService() {}
        explicit IncrementTest(const Json::Value& service_json) : IncrementTest() 
        {
            this->Load(service_json);
        }
        IncrementTest& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            BenchmarkDetector::get()->SetKeyContent("service","IncrementTest");
            BenchmarkDetector::get()->SetKeyContent("output" ,std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteLine();
        }
};

class SubtractTestTwoInputs : public improc::StringKeyHeterogeneousBaseService
{
    public:
        SubtractTestTwoInputs() : improc::StringKeyHeterogeneousBaseService() {}
        explicit SubtractTestTwoInputs(const Json::Value& service_json) : SubtractTestTwoInputs() 
        {
            this->Load(service_json);
        }
        SubtractTestTwoInputs& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) 
                                       - std::any_cast<int>(context.Get(this->inputs_[1]));
            BenchmarkDetector::get()->SetKeyContent("service","SubtractTestTwoInputs");
            BenchmarkDetector::get()->SetKeyContent("output" ,std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteLine();
        }
};

class MultiplyTest : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTest() : improc::StringKeyHeterogeneousBaseService() {}
        explicit MultiplyTest(const Json::Value& service_json) : MultiplyTest() 
        {
            this->Load(service_json);
        }
        MultiplyTest& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            for (Json::Value::const_iterator multiply_field_iter = service_json.begin(); multiply_field_iter != service_json.end(); ++multiply_field_iter)
            {
                if (multiply_field_iter.name() == "number")
                {
                    this->number_to_multiply_ = multiply_field_iter->asInt();
                    break;
                }
            }
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            BenchmarkDetector::get()->SetKeyContent("service","MultiplyTest");
            BenchmarkDetector::get()->SetKeyContent("output" ,std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteLine();
        }
};

class SubtractTestOneInput : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_subtract_ = 0;

    public:
        SubtractTestOneInput() : improc::StringKeyHeterogeneousBaseService() {}
        explicit SubtractTestOneInput(const Json::Value& service_json) : SubtractTestOneInput() 
        {
            this->Load(service_json);
        }
        SubtractTestOneInput& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            for (Json::Value::const_iterator subtract_field_iter = service_json.begin(); subtract_field_iter != service_json.end(); ++subtract_field_iter)
            {
                if (subtract_field_iter.name() == "number")
                {
                    this->number_to_subtract_ = subtract_field_iter->asInt();
                    break;
                }
            }
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) - this->number_to_subtract_;
            BenchmarkDetector::get()->SetKeyContent("service","SubtractTestOneInput");
            BenchmarkDetector::get()->SetKeyContent("output" ,std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteLine();
        }
};

class LoadFactory
{
    public:
        improc::StringKeyHeterogeneousServicesFactory factory;

    public:
        LoadFactory()
        {
            this->factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
            this->factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
            this->factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );
        }
};

#endif