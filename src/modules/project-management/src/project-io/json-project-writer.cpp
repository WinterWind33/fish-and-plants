// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-writer.hpp>

// Third-party
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>
#include <type_traits>

namespace gc::project_management::project_io {

    namespace details {
        template<class>
        inline constexpr bool InvalidVariantV = false;
    }

    JsonProjectWriter::JsonProjectWriter(std::unique_ptr<std::ostream> outputStream) noexcept :
        m_outputStream{std::move(outputStream)} {}

    void JsonProjectWriter::serializeProject(const Project& project) {
        nlohmann::json projectJson{};

        projectJson["version"] = project.getVersion().to_string();
        projectJson["title"] = project.getTitle();
        projectJson["creation_timedate"] = std::chrono::system_clock::to_time_t(project.getCreationTime());

        // Now we need to traverse the project nodes and write all the sub-nodes.

        // Single values
        const auto& values{project.getValues()};
        for(const auto& value : values) {
            const auto& key{std::get<0>(value)};

            std::visit([&key, &projectJson](auto&& arg){
                projectJson[key] = std::forward<decltype(arg)>(arg);
            }, std::get<1>(value));
        }

        // Values arrays
        const auto& valuesArrays{project.getValuesArrays()};
        for(const auto& arr : valuesArrays) {
            const auto& key{std::get<0>(arr)};

            nlohmann::json arrJson = nlohmann::json::array();
            for(const auto& val : std::get<1>(arr)) {
                std::visit([&arrJson](auto&& arg){
                    arrJson.push_back(std::forward<decltype(arg)>(arg));
                }, val);
            }

            projectJson[key] = std::move(arrJson);
        }

        // here we use .dump() because it prints newlines. If we don't use it
        // newlines won't be printed and the file is formatted on one line.
        *m_outputStream << projectJson.dump(4);
    }

} // namespace gc::project_management::project_io
