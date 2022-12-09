// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef COMMANDS_STRINGS_HPP
#define COMMANDS_STRINGS_HPP

#include <common/types.hpp>
#include <user-interface/application-strings.hpp>

namespace rpi_gc::strings::commands {

    constexpr StringViewType HELP{"help"};
    constexpr StringViewType VERSION{"version"};
    constexpr StringViewType APPLICATION{strings::application::EXECUTABLE_NAME};
    constexpr StringViewType ABORT{"abort"};
    constexpr StringViewType EXIT{"exit"};
    constexpr StringViewType AUTOMATIC_WATERING{"auto-watering"};

    namespace feedbacks {
        constexpr StringViewType TYPE_HELP{"Type \'help\' for a list of the available commands."};
        constexpr StringViewType UNRECOGNIZED_COMMAND{"Command not recognized."};
        constexpr StringViewType TEARING_DOWN{"Tearing down..."};
        constexpr StringViewType GOODBYE{"Goodbye."};

        namespace abort {
            constexpr StringViewType EMERGENCY_ABORT_SYSTEM_LOGGER_NAME{"Emergency Abort"};
            constexpr StringViewType ABORT_ISSUED{"ABORT COMMAND ISSUED."};
            constexpr StringViewType STARTING_EMERGENCY_ABORT{"Starting emergency abort."};
            constexpr StringViewType EMERGENCY_ABORT_FINISHED{"Emergency abort finished."};
        } // namespace abort
    } // namespace feedbacks

} // namespace rpi_gc

#endif // !COMMANDS_STRINGS_HPP
