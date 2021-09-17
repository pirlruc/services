#define WITH_DEBUG

#include <improc/infrastructure/file.hpp>

struct DataElem
{
    std::string name;
    std::string type;
};

int main()
{
    const std::string kDataKey      = "data";

    improc::JsonFile json_file {"../../test/test_ex1.json"};
    Json::Value json_content = json_file.Read();

    if (json_content.isMember(kDataKey) == false) 
    {
        #ifdef WITH_DEBUG
        SPDLOG_ERROR("");
        spdlog::error("ERROR_01: Member {} missing on json file.",kDataKey);
        #endif

        throw improc::file_processing_error();
    }


    // Obtain list of variables not coming from services
    Json::Value             data_elements = json_content[kDataKey];
    std::vector<DataElem>   input_list {};
    for (Json::Value::const_iterator data_elem_iter = data_elements.begin(); data_elem_iter != data_elements.end(); ++data_elem_iter)
    {
        const std::string kDataName = "name";
        const std::string kDataType = "type";

        DataElem data {};
        for (Json::Value::const_iterator data_elem_field_iter = data_elem_iter->begin(); data_elem_field_iter != data_elem_iter->end(); ++data_elem_field_iter)
        {
            #ifdef WITH_DEBUG
            SPDLOG_DEBUG("");
            spdlog::debug("Analyzing field {} for data element...",data_elem_field_iter.name());
            #endif

            if      (data_elem_field_iter.name() == kDataName)   data.name = data_elem_field_iter->asString();
            else if (data_elem_field_iter.name() == kDataType)   data.type = data_elem_field_iter->asString();
            else 
            {
                #ifdef WITH_DEBUG
                SPDLOG_WARN("");
                spdlog::warn("WARN_03: Member {} not recognized for data element.",data_elem_field_iter.name());
                #endif
            }
        }
        
        if (data.name.empty() == true || data.type.empty() == true)
        {
            #ifdef WITH_DEBUG
            SPDLOG_ERROR("");
            spdlog::error("ERROR_04: Member missing for data element.");
            #endif

            throw improc::file_processing_error();
        }
        input_list.push_back(data);
    }
    return 0;
}