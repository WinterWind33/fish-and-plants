// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project-io/project-reader.hpp>
#include <project-management/project.hpp>

#include <nlohmann/json.hpp>

// C++ STL
#include <istream>
#include <memory>

namespace gc::project_management::project_io {

    //!!
    //! \brief A project reader that reads a project from a JSON file.
    //!
    class JsonProjectReader final : public ProjectReader {
    public:
        explicit JsonProjectReader(std::unique_ptr<std::istream> inputStream) noexcept;

        //!!
        //! \brief Read a project from the input stream.
        //!
        //! \return Project The project read from the input stream.
        [[nodiscard]] Project readProject() override;

    private:
        std::unique_ptr<std::istream> m_inputStream;

        [[nodiscard]] gc::project_management::ProjectNode read_project_node(const nlohmann::json& jsonNode);
    };

} // namespace gc::project_management::project_io
