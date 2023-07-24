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

        if(m_diagnosticsObjects.empty()) {
            m_outputStream.get() << "No relevant diagnostic found." << std::endl;
            return true;
        }

        // Otherwise we can print all the diagnostics.
        for(const auto& diagnosticObj : m_diagnosticsObjects) {
            m_outputStream.get() << "<Diagnostic section start>" << std::endl;
            diagnosticObj.get().printDiagnostic(m_outputStream.get());
            m_outputStream.get() << std::endl;
            m_outputStream.get() << "<section end>" << std::endl;
        }

        return true;
    }

    bool StatusCommand::processInputOptions(const std::vector<string_type>& inputTockens) {
        try {
            m_optionParser->parse(inputTockens);
        } catch([[maybe_unused]] const std::exception& exc) {
            assert(false);
            return false;
        }
        return true;
    }

    void StatusCommand::printHelp(help_ostream_type outputStream) const noexcept {
        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::STATUS << " - display the status of all the active systems" << std::endl;
        outputStream.get() << "[SYNOPSIS]" << std::endl;
        outputStream.get() << "\t" << strings::commands::STATUS << " [OPTION]..." << std::endl;
        outputStream.get() << "[DESCRIPTION]" << std::endl;
        outputStream.get() << "\t" << "Retrieves status from all the active systems in the applications (AWS flows etc)" << std::endl;
        outputStream.get() << "\tand it displays them in a structured way, every system separated by a section." << std::endl;
        outputStream.get() << std::endl;

        m_optionParser->printHelp(outputStream);
    }


} // namespace rpi_gc::commands
