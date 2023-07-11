// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-writer.hpp>

// Third-party
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>

namespace gc::project_management::project_io {

    JsonProjectWriter::JsonProjectWriter(std::unique_ptr<std::ostream> outputStream) noexcept :
        m_outputStream{std::move(outputStream)} {}

    void JsonProjectWriter::serializeProject(const Project& project) {        ;
        nlohmann::json projectJson{};

        std::ostringstream creationTimeStream{};
        creationTimeStream << std::chrono::system_clock::to_time_t(project.getCreationTime());

        projectJson =
        {
            {"version", project.getVersion().to_string()},
            {"title", project.getTitle()},
            {"creation_timedate", creationTimeStream.str()}
        };

        *m_outputStream << projectJson;
    }

} // namespace gc::project_management::project_io
