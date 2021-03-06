#include <gtest/gtest.h>

#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <improc_services_test_config.hpp>

class IncrementTestDS : public improc::StringKeyHeterogeneousBaseService
{
    public:
        IncrementTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        IncrementTestDS(const Json::Value& service_json) : IncrementTestDS() 
        {
            this->Load(service_json);
        }
        IncrementTestDS& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_subtract_ = 0;

    public:
        SubtractTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        SubtractTestDS(const Json::Value& service_json) : SubtractTestDS() 
        {
            this->Load(service_json);
        }
        SubtractTestDS& Load   (const Json::Value& service_json) override
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
        }
};

class MultiplyTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        MultiplyTestDS(const Json::Value& service_json) : MultiplyTestDS() 
        {
            this->Load(service_json);
        }
        MultiplyTestDS& Load   (const Json::Value& service_json) override
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

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            spdlog::info("Multiply Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

TEST(Factory,TestFactoryEmptyConstructor) {
    improc::StringKeyHeterogeneousServicesFactory factory {};
    EXPECT_EQ(factory.Size(),0);    
}

TEST(Factory,TestAddItemsToFactory) {
    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );
    EXPECT_EQ(factory.Size(),3);    
}

TEST(Context,TestContextEmptyConstructor) {
    improc::StringKeyHeterogeneousContext cntxt {};
    EXPECT_EQ(cntxt.Size(),0);    
}

TEST(Context,TestAddItemToContext) {
    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    EXPECT_EQ(cntxt.Size(),1);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),2);
}

TEST(SequenceService,TestSequenceServiceEmptyConstructor) {
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_EQ(sequence.Size(),0);    
}

TEST(SequenceService,TestSequenceServiceLoadWithInputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_in.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoadWithOutputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_out.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoadWithInputAndOutputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_out.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoad) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),4);
}

TEST(SequenceService,TestSequenceServiceRunWithDuplicateService) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),4);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),4);
}
