// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <chrono>
#include <string_view>

namespace gc::project_management {

    class Project final {
    public:
        using time_point_type = std::chrono::time_point<std::chrono::system_clock>;
        using project_version = semver::version;
        using project_title = std::string_view;

        constexpr explicit Project(
            time_point_type creationTime, project_title projectTitle, project_version version) noexcept :
            m_creationTimePoint{creationTime},
            m_projectTitle{projectTitle},
            m_projectVersion{version} {}

        [[nodiscard]]
        constexpr time_point_type getCreationTime() const noexcept {
            return m_creationTimePoint;
        }

        [[nodiscard]]
        constexpr project_title getTitle() const noexcept {
            return m_projectTitle;
        }

        [[nodiscard]]
        constexpr project_version getVersion() const noexcept {
            return m_projectVersion;
        }

    private:
        const time_point_type m_creationTimePoint;
        project_title m_projectTitle{};
        project_version m_projectVersion{};
    };

} // namespace gc::project_management
