// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef VERSION_COMMAND_HPP
#define VERSION_COMMAND_HPP

#include <common/types.hpp>
#include <commands/terminal-command.hpp>

// C++ STL
#include <functional>
#include <ostream>

namespace rpi_gc {

    class VersionCommand : public TerminalCommand<CharType> {
    public:
        using ostream_ref = std::reference_wrapper<std::ostream>;

        VersionCommand(ostream_ref outputStream) noexcept;
        ~VersionCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override { return name_type{"version"}; }

        bool processOptions(const options_vector& options,
            const non_options_vector& nonOptions, const unknown_options_vector& unknown) noexcept override;

        bool execute() noexcept override;
    };

} // namespace rpi_gc

#endif // !VERSION_COMMAND_HPP
