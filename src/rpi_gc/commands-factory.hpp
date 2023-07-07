// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/project-command.hpp>

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
        ProjectCommandFactory& setOutputStream(std::ostream& ost) noexcept {
            m_outputStream = std::ref(ost);
        }

        ProjectCommandFactory& setInputStream(std::istream& ist) noexcept {
            m_inputStream = std::ref(ist);
        }

        ProjectCommandFactory& setUserLogger(std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
            m_userLogger = std::move(loggerPtr);
        }

        ProjectCommandFactory& setMainLogger(std::shared_ptr<gh_log::Logger> loggerPtr) noexcept {
            m_mainLogger = std::move(loggerPtr);
        }

        std::unique_ptr<command_type> create() override;

    private:
        std::reference_wrapper<std::ostream> m_outputStream;
        std::reference_wrapper<std::istream> m_inputStream;
        std::shared_ptr<gh_log::Logger> m_userLogger;
        std::shared_ptr<gh_log::Logger> m_mainLogger;
    };

} // namespace rpi_gc::commands_factory
