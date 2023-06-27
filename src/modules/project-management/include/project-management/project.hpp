// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <chrono>
#include <string>

namespace gc::project_management {

    class Project final {
    public:
        using time_point_type = std::chrono::time_point<std::chrono::system_clock>
        using project_version = semver::version;
        using project_title = std::string;

        constexpr explicit Project(
            time_point_type creationTime, project_title projectTitle, project_version version) noexcept :
            m_creationTimePoint{std::move(creationTime)},
            m_projectTitle{std::move(projectTitle)},
            m_projectVersion{std::move(version)} {}

        constexpr time_point_type getCreationTime() const noexcept {
            return m_creationTimePoint;
        }

        constexpr project_title getTitle() const noexcept {
            return m_projectTitle;
        }

        constexpr project_version getVersion() const noexcept {
            return m_projectVersion;
        }

    private:
        const time_point_type m_creationTimePoint;
        project_title m_projectTitle{};
        project_version m_projectVersion{};
    };

} // namespace gc::project_management
