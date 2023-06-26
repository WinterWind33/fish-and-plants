// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

namespace gc::project_management {

    class Project final {
    public:
        constexpr Project() noexcept = default;

    private:
        semver::version m_projectVersion{};
    };

} // namespace gc::project_management
