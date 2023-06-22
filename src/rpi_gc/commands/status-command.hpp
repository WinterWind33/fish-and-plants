// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <user-interface/commands-strings.hpp>
#include <commands/terminal-command.hpp>

#include <diagnostics/diagnostic-status-probeable.hpp>
#include <common/types.hpp> // For CharType
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <vector>
#include <functional>

namespace rpi_gc::commands {

    class StatusCommand final : public TerminalCommandType {
    public:
        using diagnostic_probeable_ref = std::reference_wrapper<diagnostics::DiagnosticStatusProbeable>;
        using option_parser = gh_cmd::OptionParser<CharType>;
        using option_parser_ptr = std::unique_ptr<option_parser>;

        explicit StatusCommand(
            option_parser_ptr optionParser, std::vector<diagnostic_probeable_ref> objs, std::ostream& outputStream) noexcept;

        //!!
        //! \brief Gets the name of this command. Resolves to "status".
        [[nodiscard]] constexpr name_type getName() const noexcept override {
            return strings::commands::STATUS;
        }

        bool execute() noexcept override;

        [[nodiscard]]
        bool processInputOptions(const std::vector<string_type>& inputTockens) noexcept override;

        void printHelp(help_ostream_type outputStream) const noexcept override;
    private:
        option_parser_ptr m_optionParser{};
        std::vector<diagnostic_probeable_ref> m_diagnosticsObjects{};
        std::reference_wrapper<std::ostream> m_outputStream;
    };

} // namespace rpi_gc::commands
