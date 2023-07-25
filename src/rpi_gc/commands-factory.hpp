// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/project-command.hpp>
#include <gc-project/project-controller.hpp>

#include <commands/terminal-command.hpp>

#include <gh_log/logger.hpp>

// C++ STL
#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <type_traits>

namespace rpi_gc::commands_factory {

//!!
//! \brief Interface of a command factory that creates a unique pointer to a command .
//!  of the specified type.
//!
//! \tparam CommandType The type of the command to create.
template <typename CommandType>
struct CommandFactory {
    using command_type = std::decay_t<CommandType>;

    virtual ~CommandFactory() noexcept = default;

    //!!
    //! \brief Creates a new command.
    //!
    //! \return The pointer to the command.
    virtual std::unique_ptr<command_type> create() = 0;
};

//!!
//! \brief Factory of the ProjectCommand. Configures the options and the event handlers
//!  for this command.
class ProjectCommandFactory final : public CommandFactory<commands::ProjectCommand> {
public:
    explicit ProjectCommandFactory(std::ostream& ost, std::istream& ist,
                                   gc_project::ProjectController& projectController) noexcept;

    inline ProjectCommandFactory& setUserLogger(
        std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
        m_userLogger = std::move(loggerPtr);
        return *this;
    }

    inline ProjectCommandFactory& setMainLogger(
        std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
        m_mainLogger = std::move(loggerPtr);
        return *this;
    }

    std::unique_ptr<command_type> create() override;

private:
    std::reference_wrapper<std::ostream> m_outputStream;
    std::reference_wrapper<std::istream> m_inputStream;
    std::reference_wrapper<gc_project::ProjectController> m_projectController;
    std::shared_ptr<gh_log::Logger> m_userLogger;
    std::shared_ptr<gh_log::Logger> m_mainLogger;

    [[nodiscard]] command_type::option_parser_pointer create_option_parser() const;

    void save_current_project(const std::string& customMessage = {});

    [[nodiscard]] command_type::event_handler_map create_event_handler_map();
};

}  // namespace rpi_gc::commands_factory
