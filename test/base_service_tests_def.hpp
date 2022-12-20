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
            std::cout << "IncrementTest: I = " << this->inputs_[0] << " -> O = " << this->outputs_[0] << std::endl; 
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
            std::cout << "SubtractTestTwoInputs: I1 = " << this->inputs_[0] 
                                                        << this->inputs_[1]
                                                        << " -> O = " << this->outputs_[0] << std::endl; 
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
            for (Json::Value::const_iterator multiply_field_iter = service_json.begin(); multiply_field_iter != service_json.end(); ++multiply_field_iter)
            {
                if (multiply_field_iter.name() == "number")
                {
                    this->number_to_multiply_ = multiply_field_iter->asInt();
                    break;
                }
            }
            std::cout << "MultiplyTest: I = "   << this->inputs_[0]
                                                << " , N = " << this->number_to_multiply_ 
                                                << " -> O = " << this->outputs_[0] << std::endl; 
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
            for (Json::Value::const_iterator subtract_field_iter = service_json.begin(); subtract_field_iter != service_json.end(); ++subtract_field_iter)
            {
                if (subtract_field_iter.name() == "number")
                {
                    this->number_to_subtract_ = subtract_field_iter->asInt();
                    break;
                }
            }
            std::cout << "SubtractTestOneInput: I = "   << this->inputs_[0]
                                                        << " , N = " << this->number_to_subtract_ 
                                                        << " -> O = " << this->outputs_[0] << std::endl; 
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
