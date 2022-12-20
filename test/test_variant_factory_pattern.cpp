#include <gtest/gtest.h>

#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory/variant_factory_pattern.hpp>
#include <improc/services/factory.hpp>

#include <improc_services_test_config.hpp>

class TestBaseService
{
    protected:
        std::vector<std::string>    inputs_;
        std::vector<std::string>    outputs_;

    public:
        TestBaseService() {};
        explicit TestBaseService(const Json::Value& service_json) {};

        virtual void            Run             (improc::Context<std::string,std::any>& context) const = 0;
};

class IncrementTestDS : public TestBaseService
{
    public:
        IncrementTestDS() : TestBaseService() {}
        explicit IncrementTestDS(const Json::Value& service_json) : IncrementTestDS() 
        {
            this->Load(service_json);
        }
        IncrementTestDS& Load   (const Json::Value& service_json)
        {
            static const std::string kInputKey  = "inputs";
            static const std::string kOutputKey = "outputs";
            if (service_json.isMember(kInputKey) == true)
            {
                this->inputs_ = improc::json::ReadVector<std::string>(service_json[kInputKey]);
            }
            else
            {
                IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Input member missing.");
                throw improc::file_processing_error();
            }

            if (service_json.isMember(kOutputKey) == true)
            {
                this->outputs_ = improc::json::ReadVector<std::string>(service_json[kOutputKey]);
            }
            else
            {
                throw improc::file_processing_error();
            }
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestDS : public TestBaseService
{
    private: 
        int number_to_subtract_1_ = 0;
        int number_to_subtract_2_ = 0;

    public:
        SubtractTestDS() : TestBaseService() {}
        explicit SubtractTestDS(const Json::Value& service_json) : SubtractTestDS() 
        {
            this->Load(service_json,0);
        }
        SubtractTestDS& Load   (const Json::Value& service_json, int number_to_subtract)
        {
            static const std::string kInputKey  = "inputs";
            static const std::string kOutputKey = "outputs";
            if (service_json.isMember(kInputKey) == true)
            {
                this->inputs_ = improc::json::ReadVector<std::string>(service_json[kInputKey]);
            }
            else
            {
                IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Input member missing.");
                throw improc::file_processing_error();
            }

            if (service_json.isMember(kOutputKey) == true)
            {
                this->outputs_ = improc::json::ReadVector<std::string>(service_json[kOutputKey]);
            }
            else
            {
                throw improc::file_processing_error();
            }

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for subtract service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_subtract_1_ = service_field_iter->asInt();
                    break;
                }
            }
            this->number_to_subtract_2_ = number_to_subtract;
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number 1: {}",this->number_to_subtract_1_);
            spdlog::info("Number 2: {}",this->number_to_subtract_2_);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) - this->number_to_subtract_1_ - this->number_to_subtract_2_;
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class MultiplyTestDS : public TestBaseService
{
    private: 
        int number_to_multiply_1_ = 1;
        int number_to_multiply_2_ = 1;

    public:
        MultiplyTestDS() : TestBaseService() {}
        explicit MultiplyTestDS(const Json::Value& service_json) : MultiplyTestDS() 
        {
            this->Load(service_json,1);
        }
        MultiplyTestDS& Load   (const Json::Value& service_json, int number_to_multiply)
        {
            static const std::string kInputKey  = "inputs";
            static const std::string kOutputKey = "outputs";
            if (service_json.isMember(kInputKey) == true)
            {
                this->inputs_ = improc::json::ReadVector<std::string>(service_json[kInputKey]);
            }
            else
            {
                IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Input member missing.");
                throw improc::file_processing_error();
            }

            if (service_json.isMember(kOutputKey) == true)
            {
                this->outputs_ = improc::json::ReadVector<std::string>(service_json[kOutputKey]);
            }
            else
            {
                throw improc::file_processing_error();
            }

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for multiply service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_multiply_1_ = service_field_iter->asInt();
                    break;
                }
            }
            this->number_to_multiply_2_ = number_to_multiply;
            spdlog::info("--- MULTIPLY SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number 1: {}",this->number_to_multiply_1_);
            spdlog::info("Number 1: {}",this->number_to_multiply_2_);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_1_ * this->number_to_multiply_2_;
            spdlog::info("Multiply Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

template <class ServiceType>
std::shared_ptr<TestBaseService> TestLoadServiceFromJson(const Json::Value& service_json)
{
    ServiceType service {};
    std::shared_ptr<TestBaseService> base_service {std::make_shared<ServiceType>(service.Load(service_json))};
    return base_service;
}

template <class ServiceType>
std::shared_ptr<TestBaseService> TestLoadServiceFromJsonInt(const Json::Value& service_json, int number)
{
    ServiceType service {};
    std::shared_ptr<TestBaseService> base_service {std::make_shared<ServiceType>(service.Load(service_json,number))};
    return base_service;
}

typedef improc::VariantFactoryPattern   < TestBaseService,std::string
                                        , std::variant  < std::function<std::shared_ptr<TestBaseService>(const Json::Value&)>
                                                        , std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)>
                                                        >
                                        > VariantFactoryPattern;

TEST(VariantFactoryPattern,TestFactoryEmptyConstructor) {
    VariantFactoryPattern factory {};
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Unregister("test"),false);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(VariantFactoryPattern,TestAddItemsToFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<TestBaseService>(const Json::Value&)>     {&TestLoadServiceFromJson<IncrementTestDS>}));
    EXPECT_NO_THROW(factory.Register("subtract" ,std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestDS>} ));
    EXPECT_NO_THROW(factory.Register("multiply" ,std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<MultiplyTestDS>} ));
    EXPECT_EQ(factory.GetRegisteredIds().size(),3);    
    EXPECT_EQ(factory.Size(),3);    
}

TEST(VariantFactoryPattern,TestDuplicateItemsToFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<TestBaseService>(const Json::Value&)>   {&TestLoadServiceFromJson<IncrementTestDS>}));
    EXPECT_THROW(factory.Register("increment" ,std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestDS>}),improc::duplicated_key);
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.GetRegisteredIds()[0],"increment");    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(VariantFactoryPattern,TestRemoveExistingItemFromFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<TestBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTestDS>}));
    EXPECT_TRUE(factory.Unregister("increment"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(VariantFactoryPattern,TestRemoveNonExistingItemFromFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<TestBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTestDS>}));
    EXPECT_FALSE(factory.Unregister("decrement"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(VariantFactoryPattern,TestCreationExceptionFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    VariantFactoryPattern factory {};
    EXPECT_THROW(factory.Create<std::function<std::shared_ptr<TestBaseService>(const Json::Value&)>>("increment",json_content),improc::not_found_in_factory);
}

TEST(VariantFactoryPattern,TestCreationFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    VariantFactoryPattern factory {};
    factory.Register("increment",std::function<std::shared_ptr<TestBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<MultiplyTestDS>} );

    EXPECT_NO_THROW(std::shared_ptr<TestBaseService> increment = factory.Create<std::function<std::shared_ptr<TestBaseService>(const Json::Value&)>>("increment",json_content));
    EXPECT_NO_THROW(std::shared_ptr<TestBaseService> subtract  = factory.Create<std::function<std::shared_ptr<TestBaseService>(const Json::Value&,int)>>("subtract",json_content,3));
}
