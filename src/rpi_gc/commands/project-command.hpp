// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/terminal-command.hpp>
#include <user-interface/commands-strings.hpp>
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <vector>
#include <memory>

namespace rpi_gc::commands {

    class ProjectCommand final : public TerminalCommandType {
    public:
        using option_parser = gh_cmd::OptionParser<CharType>;
        using option_parser_pointer = std::unique_ptr<option_parser>;

        explicit ProjectCommand(option_parser_pointer optionParser) noexcept;

        //!!
        //! \brief Retrieves the name of this command: project.
        //!
        [[nodiscard]]
        constexpr name_type getName() const noexcept override {
            return strings::commands::PROJECT;
        }

        [[nodiscard]]
        bool execute() noexcept override;

        [[nodiscard]]
        bool processInputOptions(const std::vector<string_type>& inputTokes) noexcept override;

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        option_parser_pointer m_optionParser{};
    };

} // namespace rpi_gc::commands
