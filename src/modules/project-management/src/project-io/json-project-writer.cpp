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

        projectJson =
        {
            {"version", project.getVersion().to_string()},
            {"title", project.getTitle()},
            {"creation_timedate", std::chrono::system_clock::to_time_t(project.getCreationTime())}
        };

        // Now we need to traverse the project nodes and write all the sub-nodes.
        const auto& values{project.getValues()};
        for(const auto& value : values)
            std::visit([&value, &projectJson](auto&& arg){
                using T = std::decay_t<decltype(arg)>;

                const auto& key = std::get<0>(value);
                const auto& val = std::get<1>(value);

                if constexpr (std::is_integral_v<T>)
                    projectJson[key] = std::get<std::int32_t>(val);
                else if constexpr (std::is_floating_point_v<T>)
                    projectJson[key] = std::get<double>(val);
                else if constexpr (std::is_same_v<T, std::string>)
                    projectJson[key] = std::get<std::string>(val);
                else if constexpr (std::is_same_v<T, bool>)
                    projectJson[key] = std::get<bool>(val);
                else
                    static_assert(details::InvalidVariantV<T>, "non-exhaustive visitor!");

            }, std::get<1>(value));

        // here we use .dump() because it prints newlines. If we don't use it
        // newlines won't be printed and the file is formatted on one line.
        *m_outputStream << projectJson.dump(4);
    }

} // namespace gc::project_management::project_io
