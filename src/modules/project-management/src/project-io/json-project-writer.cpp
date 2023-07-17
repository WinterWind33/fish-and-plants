// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-writer.hpp>

// Third-party
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>

namespace gc::project_management::project_io {

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

        // here we use .dump() because it prints newlines. If we don't use it
        // newlines won't be printed and the file is formatted on one line.
        *m_outputStream << projectJson.dump(4);
    }

} // namespace gc::project_management::project_io
