// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/status-command.hpp>

// C++ STL
#include <cassert>

namespace rpi_gc::commands {

    StatusCommand::StatusCommand(
        option_parser_ptr optionParser, std::vector<diagnostic_probeable_ref> objs, std::ostream& outputStream) noexcept :
        m_optionParser{std::move(optionParser)},
        m_diagnosticsObjects{std::move(objs)},
        m_outputStream{std::ref(outputStream)} {
        assert(m_optionParser != nullptr);
    }

    bool StatusCommand::execute() noexcept {
        const auto options{m_optionParser->getOptions()};

        // If the --help option is selected we need to print the help.
        auto helpIt = std::find_if(options.begin(), options.end(), [](const option_parser::option_pointer& option) -> bool{
            return option->getLongName() == "help";
        });

        if(helpIt != options.end() && (*helpIt)->isSet()) {
            printHelp(m_outputStream);
            for(auto& option : options)
                option->clear();

            return true;
        }

        m_outputStream.get() << "[SYSTEM STATUS]" << std::endl;
        if(m_diagnosticsObjects.empty()) {
            m_outputStream.get() << "No relevant diagnostic found." << std::endl;
            return true;
        }

        // Otherwise we can print all the diagnostics.
        for(const auto& diagnosticObj : m_diagnosticsObjects) {
            diagnosticObj.get().printDiagnostic(m_outputStream.get());
        }

        return true;
    }

    bool StatusCommand::processInputOptions(const std::vector<string_type>& inputTockens) noexcept {
        try {
            m_optionParser->parse(inputTockens);
        } catch([[maybe_unused]] const std::exception& exc) {
            assert(false);
        }
        return true;
    }

    void StatusCommand::printHelp(help_ostream_type outputStream) const noexcept {
        m_optionParser->printHelp(outputStream);
    }


} // namespace rpi_gc::commands
