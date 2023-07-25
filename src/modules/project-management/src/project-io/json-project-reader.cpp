// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-reader.hpp>

#include <nlohmann/json.hpp>

// C++ STL
#include <chrono>

namespace gc::project_management::project_io {

namespace details {

auto createVariantFromJsonNode(const nlohmann::json& jsonNode)
    -> gc::project_management::ProjectNode::value_impl_type {
    if (jsonNode.is_boolean())
        return jsonNode.get<bool>();
    else if (jsonNode.is_number_integer()) {
        if (jsonNode.is_number_unsigned())
            return jsonNode.get<std::uint64_t>();
        else
            return jsonNode.get<std::int64_t>();
    } else if (jsonNode.is_number_float())
        return jsonNode.get<double>();
    else if (jsonNode.is_string())
        return jsonNode.get<std::string>();

    throw std::runtime_error{"JSON node type not supported."};
}

} // namespace details

JsonProjectReader::JsonProjectReader(std::unique_ptr<std::istream> ist) noexcept
    : m_inputStream{std::move(ist)} {}

Project JsonProjectReader::readProject() {
    nlohmann::json inputProjectJson{};
    // Read the JSON from the input file.
    *m_inputStream >> inputProjectJson;

    // Begin read the project header:
    // - creation time-date
    // - project title
    // - project version
    const Project::time_point_type creationTimeDate{std::chrono::system_clock::from_time_t(
        inputProjectJson["creation_timedate"].get<std::time_t>())};

    std::string projectTitle{inputProjectJson["title"].get<std::string>()};
    std::string projectVersionStr{inputProjectJson["version"].get<std::string>()};

    // If the version string is empty, we set it to 0.0.0 as default.
    if (projectVersionStr.empty())
        projectVersionStr = "0.0.0";

    semver::version projectVersion{semver::from_string(projectVersionStr)};
    Project finalProject{creationTimeDate, std::move(projectTitle), projectVersion};

    // Now that we have read all the basic project information, we can read the project's values,
    // value arrays and objects.
    for (const auto& [key, value] : inputProjectJson.items()) {
        // Skip the project header.
        if (key == "creation_timedate" || key == "title" || key == "version")
            continue;

        // If the value is an array, we read it as an array of values.
        if (value.is_array()) {
            std::vector<ProjectNode::value_impl_type> valueArray{};
            for (const auto& arrayValue : value) {
                // We need to instance a variant based on the array value type.
                ProjectNode::value_impl_type finalValue{
                    details::createVariantFromJsonNode(arrayValue)};
                valueArray.push_back(std::move(finalValue));
            }
            finalProject.addValueArray(key, std::move(valueArray));
        }

        // If the value is an object, we read it as an object of values.
        else if (value.is_object()) {
            finalProject.addObject(key, read_project_node(value));
        }
        // If the value is a simple value, we read it as a simple value.
        else {
            finalProject.addValue(key, details::createVariantFromJsonNode(value));
        }
    }

    return finalProject;
}

ProjectNode JsonProjectReader::read_project_node(const nlohmann::json& jsonNode) {
    ProjectNode finalNode{};

    for (const auto& [key, value] : jsonNode.items()) {
        // If the value is an array, we read it as an array of values.
        if (value.is_array()) {
            std::vector<ProjectNode::value_impl_type> valueArray{};
            for (const auto& arrayValue : value) {
                // We need to instance a variant based on the array value type.
                ProjectNode::value_impl_type finalValue{
                    details::createVariantFromJsonNode(arrayValue)};
                valueArray.push_back(std::move(finalValue));
            }
            finalNode.addValueArray(key, std::move(valueArray));
        }

        // If the value is an object, we read it as an object of values.
        else if (value.is_object()) {
            finalNode.addObject(key, read_project_node(value));
        }
        // If the value is a simple value, we read it as a simple value.
        else {
            finalNode.addValue(key, details::createVariantFromJsonNode(value));
        }
    }

    return finalNode;
}

} // namespace gc::project_management::project_io
