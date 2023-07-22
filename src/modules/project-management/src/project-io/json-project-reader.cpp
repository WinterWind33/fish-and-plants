// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-reader.hpp>

#include <nlohmann/json.hpp>

// C++ STL
#include <chrono>

namespace gc::project_management::project_io {

    JsonProjectReader::JsonProjectReader(std::unique_ptr<std::istream> ist) noexcept :
        m_inputStream{std::move(ist)} {}

    Project JsonProjectReader::readProject() {
        nlohmann::json inputProjectJson{};
        // Read the JSON from the input file.
        *m_inputStream >> inputProjectJson;

        // Begin read the project header:
        // - creation time-date
        // - project title
        // - project version
        const Project::time_point_type creationTimeDate{
            std::chrono::system_clock::from_time_t(inputProjectJson["creation_timedate"].get<std::time_t>())};

        std::string projectTitle{inputProjectJson["title"].get<std::string>()};
        std::string projectVersionStr{inputProjectJson["version"].get<std::string>()};

        // If the version string is empty, we set it to 0.0.0 as default.
        if(projectVersionStr.empty())
            projectVersionStr = "0.0.0";

        semver::version projectVersion{semver::from_string(projectVersionStr)};
        Project finalProject{creationTimeDate, std::move(projectTitle), projectVersion};

        // Now that we have read all the basic project information, we can read the project's values,
        // value arrays and objects.
        for(const auto& [key, value] : inputProjectJson.items()) {
            // Skip the project header.
            if(key == "creation_timedate" || key == "title" || key == "version")
                continue;

            // If the value is an array, we read it as an array of values.
            if(value.is_array()) {
                std::vector<ProjectNode::value_impl_type> valueArray{};
                for(const auto& arrayValue : value) {
                    // We need to instance a variant based on the array value.
                    ProjectNode::value_impl_type finalValue{};
                    valueArray.push_back(std::move(finalValue));
                }
                finalProject.addValueArray(key, std::move(valueArray));
            }

            // If the value is an object, we read it as an object of values.
            else if(value.is_object()) {
                ProjectNode valueObject{};
                finalProject.addObject(key, std::move(valueObject));
            }
            // If the value is a simple value, we read it as a simple value.
            else {
                if(value.is_boolean()) {
                    finalProject.addValue(key, value.get<bool>());
                }
                else if(value.is_number_integer()) {
                    if(value.is_number_unsigned())
                        finalProject.addValue(key, value.get<std::uint64_t>());
                    else
                        finalProject.addValue(key, value.get<std::int64_t>());
                }
                else if(value.is_number_float()) {
                    finalProject.addValue(key, value.get<double>());
                }
                else if(value.is_string()) {
                    finalProject.addValue(key, value.get<std::string>());
                }
            }
        }

        return finalProject;
    }

} // namespace gc::project_management::project_io
