// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-writer.hpp>

// Third-party
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>

namespace gc::project_management::project_io {

    JsonProjectWriter::JsonProjectWriter(std::ostream& outputStream) noexcept :
        m_outputStream{outputStream} {}

    void JsonProjectWriter::serializeProject(const Project& project) {        ;
        nlohmann::json projectJson{};

        std::ostringstream creationTimeStream{};
        creationTimeStream << project.getCreationTime();

        projectJson =
        {
            {"version", project.getVersion().to_string()},
            {"title", project.getTitle()},
            {"creation_timedate", creationTimeStream.str()}
        };

        m_outputStream.get() << projectJson;
    }

} // namespace gc::project_management::project_io
