// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-io/json-project-reader.hpp>

namespace gc::project_management::project_io {

    JsonProjectReader::JsonProjectReader(std::istream& ist) noexcept :
        m_inputStream{ist} {}

} // namespace gc::project_management::project_io
