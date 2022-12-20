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
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_services.json");
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(LoadFactory().factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestServiceTypeNotInJson) {
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_service_type.json");
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(LoadFactory().factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestServiceInputsNotInJson) {
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_service_inputs.json");
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(LoadFactory().factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestServiceOutputsNotInJson) {
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_no_service_outputs.json");
    improc::StringKeyHeterogeneousSequenceService sequence {};
    EXPECT_THROW(sequence.Load(LoadFactory().factory,json_content),improc::file_processing_error);
}

TEST(SequenceService,TestSequenceServiceRunWithDuplicateService) {
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_duplicated_service.json");
    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(LoadFactory().factory,json_content);
    EXPECT_EQ(sequence.Size(),4);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),4);
}

TEST(SequenceService,TestSequenceServiceRunWithSeveralOutputs) {
    Json::Value json_content = improc::JsonFile::Read(std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_several_outputs.json");

    improc::StringKeyHeterogeneousServicesFactory factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestTwoInputs>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    improc::StringKeyHeterogeneousSequenceService sequence {};
    sequence.Load(factory,json_content);
    EXPECT_EQ(sequence.Size(),3);

    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    sequence.Run(cntxt);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-1")),3);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori-2")),6);
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori"  )),3);
}
