// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-writer.hpp>

// Third-party
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>
#include <type_traits>

namespace gc::project_management::project_io {

    JsonProjectWriter::JsonProjectWriter(std::unique_ptr<std::ostream> outputStream) noexcept :
        m_outputStream{std::move(outputStream)} {}

    void JsonProjectWriter::serializeProject(const Project& project) {
        nlohmann::json projectJson{};

        projectJson["version"] = project.getVersion().to_string();
        projectJson["title"] = project.getTitle();
        projectJson["creation_timedate"] = std::chrono::system_clock::to_time_t(project.getCreationTime());

        // Now we need to traverse the project nodes and write all the sub-nodes.
        serializeProjectNode(project, projectJson);

        // here we use .dump() because it prints newlines. If we don't use it
        // newlines won't be printed and the file is formatted on one line.
        *m_outputStream << projectJson.dump(4);
    }

    void JsonProjectWriter::serializeProjectNode(const ProjectNode& node, nlohmann::json& parentJson) {
        // Single values
        const auto& values{node.getValues()};
        for(const auto& value : values) {
            const auto& key{std::get<0>(value)};

            std::visit([&key, &parentJson](auto&& arg){
                parentJson[key] = std::forward<decltype(arg)>(arg);
            }, std::get<1>(value));
        }

        // Values arrays
        const auto& valuesArrays{node.getValuesArrays()};
        for(const auto& arr : valuesArrays) {
            const auto& key{std::get<0>(arr)};

            nlohmann::json arrJson = nlohmann::json::array();
            for(const auto& val : std::get<1>(arr)) {
                std::visit([&arrJson](auto&& arg){
                    arrJson.push_back(std::forward<decltype(arg)>(arg));
                }, val);
            }

            parentJson[key] = std::move(arrJson);
        }

        // Time to serialize all the objects.
        const auto& objects{node.getObjects()};
        for(const auto& object : objects) {
            // We need to create a new JSON node for every object.
            nlohmann::json objectJsonNode{};
            serializeProjectNode(std::get<1>(object), objectJsonNode);

            parentJson[std::get<0>(object)] = std::move(objectJsonNode);
        }
    }

} // namespace gc::project_management::project_io
