#include <gtest/gtest.h>

#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <improc_services_test_config.hpp>

class IncrementTestSO : public improc::StringKeyHeterogeneousBaseService
{
    public:
        IncrementTestSO() : improc::StringKeyHeterogeneousBaseService() {}
        IncrementTestSO& Load   (const Json::Value& service_json) override
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
            spdlog::info("Increment Service: ori-1 = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestSO : public improc::StringKeyHeterogeneousBaseService
{
    public:
        SubtractTestSO() : improc::StringKeyHeterogeneousBaseService() {}
        SubtractTestSO& Load   (const Json::Value& service_json) override
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
        }
};

class MultiplyTestSO : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTestSO() : improc::StringKeyHeterogeneousBaseService() {}
        MultiplyTestSO& Load   (const Json::Value& service_json) override
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
            spdlog::info("Multiply Service: ori-2 = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

TEST(SequenceService,TestSequenceServiceRunWithSeveralOutputs) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex2.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestSO>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestSO>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestSO>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),3);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-1")),3);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-2")),6);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori"  )),3);
}
