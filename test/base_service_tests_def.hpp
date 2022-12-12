#include <improc/services/base_service.hpp>
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
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
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
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {};{}",this->inputs_[0],this->inputs_[1]);
            spdlog::info("Output: {}",this->outputs_[0]);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) 
                                       - std::any_cast<int>(context.Get(this->inputs_[1]));
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
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
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            spdlog::info("Multiply Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
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
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) - this->number_to_subtract_;
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->SetKeyContent("service","SubtractTestOneInput");
            BenchmarkDetector::get()->SetKeyContent("output" ,std::any_cast<int>(context[this->outputs_[0]]));
            BenchmarkDetector::get()->WriteLine();
        }
};
