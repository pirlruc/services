#include <gtest/gtest.h>

#include <improc/infrastructure/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

class IncrementTestSO : public improc::StringKeyBaseService
{
    public:
        IncrementTestSO() : improc::StringKeyBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyBaseService::Load(service_json);
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
        }

        void    Run    (improc::StringKeyContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori-1 = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestSO : public improc::StringKeyBaseService
{
    public:
        SubtractTestSO() : improc::StringKeyBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyBaseService::Load(service_json);
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {};{}",this->inputs_[0],this->inputs_[1]);
            spdlog::info("Output: {}",this->outputs_[0]);
        }

        void    Run    (improc::StringKeyContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) 
                                       - std::any_cast<int>(context.Get(this->inputs_[1]));
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class MultiplyTestSO : public improc::StringKeyBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTestSO() : improc::StringKeyBaseService() {}
        void    Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyBaseService::Load(service_json);

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

        void    Run    (improc::StringKeyContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            spdlog::info("Multiply Service: ori-2 = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

TEST(SequenceService,TestSequenceServiceRunWithSeveralOutputs) {
    improc::JsonFile json_file {"../../test/test_ex2.json"};
    Json::Value json_content = json_file.Read();

    improc::StringKeyServicesFactory factory {};
    factory.Add("increment",std::function<std::shared_ptr<improc::StringKeyBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestSO>});
    factory.Add("subtract" ,std::function<std::shared_ptr<improc::StringKeyBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestSO>} );
    factory.Add("multiply" ,std::function<std::shared_ptr<improc::StringKeyBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestSO>} );

    improc::StringKeySequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),3);

    improc::StringKeyContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-1")),3);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-2")),6);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori"  )),3);
}
