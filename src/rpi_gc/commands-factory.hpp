// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/project-command.hpp>
#include <gc-project/project-controller.hpp>

#include <commands/terminal-command.hpp>

#include <gh_log/logger.hpp>

// C++ STL
#include <memory>
#include <type_traits>
#include <ostream>
#include <istream>
#include <functional>

namespace rpi_gc::commands_factory {

    template<typename CommandType>
    struct CommandFactory {
        using command_type = std::decay_t<CommandType>;

        virtual ~CommandFactory() noexcept = default;

        virtual std::unique_ptr<command_type> create() = 0;
    };

    class ProjectCommandFactory final : public CommandFactory<commands::ProjectCommand> {
    public:
        explicit ProjectCommandFactory(std::ostream& ost, std::istream& ist, gc_project::ProjectController& projectController) noexcept;

        inline ProjectCommandFactory& setUserLogger(std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
            m_userLogger = std::move(loggerPtr);
            return *this;
        }

        inline ProjectCommandFactory& setMainLogger(std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
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

        command_type::option_parser_pointer
            create_option_parser() const;

        command_type::event_handler_map create_event_handler_map();
    };

} // namespace rpi_gc::commands_factory
