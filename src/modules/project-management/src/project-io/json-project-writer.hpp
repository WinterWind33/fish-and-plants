// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project-io/project-writer.hpp>

#include <nlohmann/json.hpp>

// C++ STL
#include <ostream>
#include <functional> // for std::reference_wrapper

namespace gc::project_management::project_io {

    //!!
    //! \brief Serialize the given project to an output stream using
    //!  a JSON format.
    class JsonProjectWriter final : public ProjectWriter {
    public:
        explicit JsonProjectWriter(std::unique_ptr<std::ostream> outputStream) noexcept;

        void serializeProject(const Project& project) override;
    private:
        std::unique_ptr<std::ostream> m_outputStream;

        void serializeProjectNode(const ProjectNode& node, nlohmann::json& parentJson);
    };

} // namespace gc::project_management::project_io
