// Copyright (C) 2023 Andrea Ballestrazzi
#include <commands/automatic-watering/automatic-watering-command.hpp>

// C++ STL
#include <algorithm>
#include <exception>

namespace rpi_gc {

namespace details {

static void TriggerOptionEventIfBound(auto&& optionMap, auto&& option) noexcept {
    if (!optionMap.contains(option->getLongName()))
        return;

    // We retrieve all the events in the map.
    auto iteratorPair = optionMap.equal_range(option->getLongName());

    for (auto it = std::get<0>(iteratorPair); it != std::get<1>(iteratorPair); ++it) {
        const AutomaticWateringCommand::option_event& event{std::get<1>(*it)};
        assert(static_cast<bool>(event));

        // We trigger the event.
        event(option);
    }
}

} // namespace details

AutomaticWateringCommand::AutomaticWateringCommand(ostream_ref outputStream,
                                                   option_parser_pointer optionParser) noexcept
    : m_outputStream{outputStream},
      m_optionParser{std::move(optionParser)} {}

bool AutomaticWateringCommand::processInputOptions(const std::vector<string_type>& inputTokens) {
    assert(m_optionParser != nullptr);

    try {
        m_optionParser->parse(inputTokens);
    } catch (const std::exception& exc) {
        m_outputStream.get() << "[ERROR] => " << exc.what() << std::endl;
        m_outputStream.get() << "[ERROR] => The command won\'t be executed." << std::endl;
        return false;
    }
    return true;
}

bool AutomaticWateringCommand::execute() noexcept {
    auto options{m_optionParser->getOptions()};

    // First of all we need to check if the help
    // option is set. If this is the case we need to
    // execute it first and return.
    auto optionIt = std::find_if(options.begin(), options.end(),
                                 [](const option_parser::const_option_pointer& option) -> bool {
                                     assert(static_cast<bool>(option));

                                     return option->getLongName() ==
                                            strings::commands::options::AUTOMATIC_WATERING_HELP;
                                 });

    if (optionIt != options.end() && (*optionIt)->isSet()) {
        // We have found the command help and it's set. We can execute it.
        printHelp(m_outputStream.get());

        gh_cmd::utility::ClearAllOptions(options);
        return true;
    }

    // We need to cache the events because we don't want to start the watering system if
    // the user typed commands along with --stop. E.g. If the user typed --start or --stop
    // we don't want to proceed. Stop has the precedence on all.
    auto stopOptionIt = std::find_if(options.begin(), options.end(),
                                     [](const option_parser::const_option_pointer& option) -> bool {
                                         assert(static_cast<bool>(option));

                                         return option->getLongName() ==
                                                strings::commands::options::AUTOMATIC_WATERING_STOP;
                                     });

    if (stopOptionIt != options.end() && (*stopOptionIt)->isSet()) {
        details::TriggerOptionEventIfBound(m_optionsEvents, *stopOptionIt);
        gh_cmd::utility::ClearAllOptions(options);
        return true;
    }

    for (auto& option : options) {
        assert(option != nullptr);

        if (option->isSet())
            details::TriggerOptionEventIfBound(m_optionsEvents, option);
    }

    // We need to reset the state of the option otherwise the next time the
    // user types it, it will execute all of the previous options.
    gh_cmd::utility::ClearAllOptions(options);

    return true;
}

void AutomaticWateringCommand::printHelp(help_ostream_type outputStream) const noexcept {
    assert(m_optionParser != nullptr);

    outputStream.get() << "[NAME]" << std::endl;
    outputStream.get() << "\t" << strings::commands::AUTOMATIC_WATERING
                       << " - manages the automatic irrigation of the greenhouse" << std::endl;
    outputStream.get() << "[SYNOPSIS]" << std::endl;
    outputStream.get() << "\t" << strings::commands::AUTOMATIC_WATERING << " [OPTION]..."
                       << std::endl;
    outputStream.get() << "[DESCRIPTION]" << std::endl;
    outputStream.get() << "\t"
                       << "Manages the automatic irrigation of the greenhouse activating or"
                       << std::endl;
    outputStream.get() << "\tdisactivating the pump and the valve in order to make the water flow."
                       << std::endl;
    outputStream.get() << std::endl;
    outputStream.get() << "\tThe hardware configuration is active-LOW." << std::endl;
    outputStream.get() << std::endl;

    m_optionParser->printHelp(outputStream.get());
}

void AutomaticWateringCommand::registerOptionEvent(option_type::long_name_type optionName,
                                                   option_event event) noexcept {
    assert(!optionName.empty());

    if (optionName == strings::commands::options::AUTOMATIC_WATERING_HELP) {
        // The user can't register an event for the "--help" command.
        assert(false);

        return;
    }

    m_optionsEvents.emplace(std::move(optionName), std::move(event));
}

} // namespace rpi_gc
