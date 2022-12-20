#include <gtest/gtest.h>

#include <base_service_tests_def.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <improc_services_test_config.hpp>

TEST(SequenceService,TestSequenceServiceEmptyConstructor) {
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_EQ(sequence.Size(),0);    
}

TEST(SequenceService,TestServicesNotInJson) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_services.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestServiceTypeNotInJson) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_service_type.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoadWithInputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_in.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoadWithOutputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_out.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoadWithInputAndOutputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1_no_out.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceLoad) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),4);
}

TEST(SequenceService,TestSequenceServiceRunWithDuplicateService) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex1.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),4);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    spdlog::info("Start, ori = {}",std::any_cast<int>(cntxt.Get("ori")));
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),4);
}

TEST(SequenceService,TestSequenceServiceRunWithSeveralOutputs) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_ex2.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestTwoInputs>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

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
