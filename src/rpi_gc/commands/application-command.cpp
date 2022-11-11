// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/application-command.hpp>

#include <rpi_gc-config-file.hpp> // For printing the version.

// C++ STL
#include <cassert>
#include <string_view>
#include <algorithm>

namespace rpi_gc {

    ApplicationCommand::ApplicationCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParser{std::move(optionParser)} {

#ifndef NDEBUG
        // Note: this is a temporary implementation in order to reach the end of the MVP UX.0. In the future
        // this behaviour needs to be injectable.
        const auto supportedOptions{m_optionParser.get().getOptions()};

        auto checkSupportedOption = [&supportedOptions](const option_type::short_name_type shortName) -> bool {
            const auto resIt = std::find_if(supportedOptions.cbegin(), supportedOptions.cend(),
            [shortName](const std::shared_ptr<const option_type>& option) -> bool {
                return option->getShortName() == shortName;
            });

            return resIt != supportedOptions.cend();
        };
#endif // !NDEBUG
        assert(checkSupportedOption('h')); // Supports help option
        assert(checkSupportedOption('v')); // Supports version option

        // We initialize the supported options callbacks for this application here.
        // NOTE: possible DIP breaking?
        m_optionsCallbacks['h'] = [this](){ print_help(); };
        m_optionsCallbacks['v'] = [this](){ print_version(); };
    }

    bool ApplicationCommand::processOptions(const options_vector& options, const non_options_vector& nonOptions,
            const unknown_options_vector& unknowns) noexcept {
        // Here we don't need to do anything because we already have the reference to the option parser that
        // contains all the supported options.
        // Here we simply check that the given parameters are the same as the ones inside the option
        // parser.
        assert(m_optionParser.get().getOptions().size() == options.size());
        assert(m_optionParser.get().getNonOptionArguments().size() == nonOptions.size());
        assert(m_optionParser.get().getUnknownOptions().size() == unknowns.size());

        return true;
    }

    bool ApplicationCommand::execute() noexcept {
        bool bCanContinueExecution{true};
        std::vector<std::function<void()>> callbacksToExecute{};

        const auto supportedOptions{m_optionParser.get().getOptions()};

        auto isOptionSet = [&supportedOptions](const option_type::short_name_type shortName) -> bool {
            const auto resIt = std::find_if(supportedOptions.cbegin(), supportedOptions.cend(),
            [shortName](const std::shared_ptr<const option_type>& option) -> bool {
                return option->getShortName() == shortName && option->isSet();
            });

            return resIt != supportedOptions.cend();
        };

        if(isOptionSet('h')) {
            assert(m_optionsCallbacks.contains('h'));
            m_optionsCallbacks['h']();

            // Tipically we don't want to execute the application if the
            // user typed --help
            return false;
        }

        for(const auto& option : supportedOptions) {
            assert(option != nullptr);

            if(option->isSet()) {
                assert(m_optionsCallbacks[option->getShortName()]);

                callbacksToExecute.push_back(m_optionsCallbacks[option->getShortName()]);

                // Tipically we don't want to execute the application if the
                // user typed --version
                if(option->getShortName() == 'v')
                    bCanContinueExecution = false;
            }
        }

        for(std::function<void()> callback : callbacksToExecute)
            callback();

        return bCanContinueExecution;
    }

    void ApplicationCommand::print_help() noexcept {
        constexpr std::string_view COPYRIGHT_DISCLAIMER{"Copyright (c) 2022 Andrea Ballestrazzi"};
        constexpr std::string_view TEAM_NAME{"Fish&Plants Team"};

        // We initially print the help page header.
        print_version();
        m_outputStream.get() << COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << "Developed by " << TEAM_NAME << std::endl;
        m_outputStream.get() << std::endl;

        m_optionParser.get().printHelp(m_outputStream.get());
        m_outputStream.get() << std::endl;
    }

    void ApplicationCommand::print_version() noexcept {
        constexpr std::string_view APPLICATION_NAME{"Greenhouse Controller"};
        m_outputStream.get() << APPLICATION_NAME << " - Version " <<
            rpi_gc_VERSION_MAJOR << "." <<
            rpi_gc_VERSION_MINOR << "." <<
            rpi_gc_VERSION_PATCH << std::endl;
    }

} // namespace rpi_gc
