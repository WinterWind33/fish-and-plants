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

        return Project{creationTimeDate, std::move(projectTitle), projectVersion};
    }

} // namespace gc::project_management::project_io
