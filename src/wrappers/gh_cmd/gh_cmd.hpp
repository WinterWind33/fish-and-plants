// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef GH_CMD_HPP
#define GH_CMD_HPP

#ifdef USE_POPL20
#include <popl20/popl.hpp>
#else
#error "There is no alternative to POPL20 right now, you must choose it."
#endif // USE_POPL20

// C++ STL
#include <string>
#include <type_traits>
#include <memory>
#include <ostream>

namespace gh_cmd {

    struct OptionVisitor {
        using option_type = popl::Option;

        virtual void visit(const option_type& option) const noexcept = 0;
    };

    template<typename CharType>
    class CommandOption {
    public:
        using char_type = std::decay_t<CharType>;
        using string_type = std::basic_string<char_type>;
        using short_name_type = char_type;
        using long_name_type = string_type;

        virtual ~CommandOption() noexcept = default;

        virtual short_name_type getShortName() const noexcept = 0;
        virtual long_name_type getLongName() const noexcept = 0;
        virtual string_type getDescription() const noexcept = 0;
        virtual void acceptVisitor(const OptionVisitor& visitor) const noexcept = 0;
    };

    template<typename CharType>
    class Switch : public CommandOption<CharType> {
    public:
        using typename CommandOption<CharType>::char_type;
        using typename CommandOption<CharType>::string_type;
        using typename CommandOption<CharType>::short_name_type;
        using typename CommandOption<CharType>::long_name_type;
        static_assert(std::is_same_v<char_type, char>, "Only char is accepted as a valid char type.");

        using impl_type = popl::Switch;

        Switch(short_name_type shortName, long_name_type longName, string_type description) noexcept;
        ~Switch() noexcept override = default;

        short_name_type getShortName() const noexcept override;
        long_name_type getLongName() const noexcept override;
        string_type getDescription() const noexcept override;

        void acceptVisitor(const OptionVisitor& visitor) const noexcept override;

    private:
        std::unique_ptr<impl_type> m_switchImpl{};
    };

    template<typename CharType>
    struct OptionParser {
        using char_type = std::decay_t<CharType>;

        virtual void parseCommandLine(const std::int32_t argc, const char_type* const argv[]) noexcept = 0;
        virtual void reset() noexcept = 0;
        virtual void printHelp(std::basic_ostream<char_type>& outputStream) const noexcept;
    };

    // Switch implementation
    template<typename C>
    inline Switch<C>::Switch(short_name_type shortName, long_name_type longName, string_type description) noexcept :
        m_switchImpl{std::make_unique<impl_type>(std::string{shortName}, std::move(longName), std::move(description))} {}

    template<typename C>
    inline auto Switch<C>::getShortName() const noexcept -> short_name_type {
        return m_switchImpl->short_name();
    }

    template<typename C>
    inline auto Switch<C>::getLongName() const noexcept -> long_name_type {
        return m_switchImpl->long_name();
    }

    template<typename C>
    inline auto Switch<C>::getDescription() const noexcept -> string_type {
        return m_switchImpl->description();
    }

    template<typename C>
    inline void Switch<C>::acceptVisitor(const OptionVisitor& visitor) const noexcept {
        visitor->visit(*m_switchImpl);
    }

} // namespace gh_cmd

#endif // !GH_CMD_HPP
