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
#include <cstdint>
#include <type_traits>
#include <memory>
#include <ostream>
#include <vector>
#include <functional>
#include <optional>
#include <cassert>

namespace gh_cmd {

    template<typename OptionType>
    struct OptionVisitor {
        virtual void visit(OptionType option) noexcept = 0;
    };

    template<typename OptionType>
    struct ConstOptionVisitor {
        virtual void visit(const OptionType option) const noexcept = 0;
    };

    template<typename CharType>
    class CommandOption {
    public:
        using base_impl_type = popl::Option;
        using char_type = std::decay_t<CharType>;
        using string_type = std::basic_string<char_type>;
        using short_name_type = char_type;
        using long_name_type = string_type;

        virtual ~CommandOption() noexcept = default;

        virtual short_name_type getShortName() const noexcept = 0;
        virtual long_name_type getLongName() const noexcept = 0;
        virtual string_type getDescription() const noexcept = 0;
        virtual void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept = 0;
        virtual void acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept = 0;
    };

    template<typename CharType>
    class Switch : public CommandOption<CharType> {
    public:
        using typename CommandOption<CharType>::char_type;
        using typename CommandOption<CharType>::string_type;
        using typename CommandOption<CharType>::short_name_type;
        using typename CommandOption<CharType>::long_name_type;
        using typename CommandOption<CharType>::base_impl_type;
        static_assert(std::is_same_v<char_type, char>, "Only char is accepted as a valid char type.");

        using impl_type = popl::Switch;

        Switch(short_name_type shortName, long_name_type longName, string_type description) noexcept;
        ~Switch() noexcept override = default;

        short_name_type getShortName() const noexcept override;
        long_name_type getLongName() const noexcept override;
        string_type getDescription() const noexcept override;

        void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept override;
        void acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept override;

    private:
        // We use shared_ptr for now as the popl implementation
        // provides APIs with a shared ptr.
        std::shared_ptr<impl_type> m_switchImpl{};
    };

    template<typename CharType>
    struct OptionParser {
        using char_type = std::decay_t<CharType>;
        using string_type = std::basic_string<char_type>;

        virtual void addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept = 0;
        virtual void parse(const std::vector<string_type>& args) noexcept = 0;
        virtual void reset() noexcept = 0;
        virtual void printHelp(std::basic_ostream<char_type>& outputStream) const noexcept = 0;

        virtual std::vector<std::shared_ptr<const CommandOption<char_type>>> getOptions() const noexcept = 0;
        virtual std::vector<string_type> getNonOptionArguments() const noexcept = 0;
        virtual std::vector<string_type> getUnknownOptions() const noexcept = 0;
    };

    template<typename CharType>
    class DefaultOptionParser : public OptionParser<CharType> {
    public:
        using typename OptionParser<CharType>::char_type;
        using typename OptionParser<CharType>::string_type;

        using impl_type = popl::OptionParser;
        static_assert(std::is_same_v<CharType, char>, "Only char is accepted as a valid char type");

        DefaultOptionParser() noexcept;
        DefaultOptionParser(string_type description) noexcept;

        void addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept override;
        void parse(const std::vector<string_type>& args) noexcept override;
        void reset() noexcept override;
        void printHelp(std::basic_ostream<char_type>& outputStream) const noexcept override;

        std::vector<std::shared_ptr<const CommandOption<char_type>>> getOptions() const noexcept override;
        std::vector<string_type> getUnknownOptions() const noexcept override;
        std::vector<string_type> getNonOptionArguments() const noexcept override;

    private:
        using option_ptr = std::shared_ptr<CommandOption<char_type>>;

        std::unique_ptr<impl_type> m_implParser{};
        std::vector<option_ptr> m_options{};
    };

    // Switch implementation
    template<typename C>
    inline Switch<C>::Switch(short_name_type shortName, long_name_type longName, string_type description) noexcept {
        const short_name_type shortNameRawString[2] = {shortName, 0};

        m_switchImpl = std::make_shared<impl_type>(shortNameRawString, std::move(longName), std::move(description));
    }

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
    inline void Switch<C>::acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept {
        visitor.visit(m_switchImpl);
    }

    template<typename C>
    inline void Switch<C>::acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept {
        visitor.visit(m_switchImpl);
    }

    // DefaultOptionParser implementation
    template<typename C>
    inline DefaultOptionParser<C>::DefaultOptionParser() noexcept :
        m_implParser{std::make_unique<impl_type>()} {}

    template<typename C>
    inline DefaultOptionParser<C>::DefaultOptionParser(string_type description) noexcept :
        m_implParser{std::make_unique<impl_type>(std::move(description))} {}

    template<typename C>
    inline auto DefaultOptionParser<C>::getNonOptionArguments() const noexcept -> std::vector<string_type> {
        return m_implParser->non_option_args();
    }

    template<typename C>
    inline auto DefaultOptionParser<C>::getUnknownOptions() const noexcept -> std::vector<string_type> {
        return m_implParser->unknown_options();
    }

    template<typename C>
    inline auto DefaultOptionParser<C>::getOptions() const noexcept -> std::vector<std::shared_ptr<const CommandOption<char_type>>> {
        std::vector<std::shared_ptr<const CommandOption<char_type>>> result{};

        for(const auto& option : m_options)
            result.emplace_back(option);

        return result;
    }

    template<typename C>
    inline void DefaultOptionParser<C>::reset() noexcept {
        m_implParser->reset();
    }

    template<typename C>
    inline void DefaultOptionParser<C>::printHelp(std::basic_ostream<char_type>& outputStream) const noexcept {
        outputStream << m_implParser->help();
    }

    namespace details {
        // Internal use only, should not be used outside the wrapper.
        class SwitchInserterVisitor : public OptionVisitor<std::shared_ptr<popl::Option>> {
        public:
            using option_parser_ref = std::reference_wrapper<popl::OptionParser>;

            SwitchInserterVisitor(option_parser_ref optionParser) noexcept :
                m_optionParser{std::move(optionParser)} {}

            void visit(std::shared_ptr<popl::Option> option) noexcept override {
                std::shared_ptr<popl::Switch> switchPtr{std::static_pointer_cast<popl::Switch>(std::move(option))};
                m_optionParser.get().template add_shared<popl::Switch>(std::move(switchPtr));
            }

        private:
            option_parser_ref m_optionParser;
        };
    } // namespace details

    template<typename C>
    inline void DefaultOptionParser<C>::addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept {
        assert(option != nullptr);

        details::SwitchInserterVisitor inserter{*m_implParser};
        option->acceptVisitor(inserter);

        m_options.push_back(std::move(option));
    }

    template<typename C>
    inline void DefaultOptionParser<C>::parse(const std::vector<string_type>& args) noexcept {
        // Before proceeding we need to create a vector of const char* because the
        // popl parser uses primitive types and not string types.
        std::vector<const C*> rawStrings{};
        for(const string_type& str : args)
            rawStrings.push_back(str.c_str());

        m_implParser->parse(static_cast<std::int32_t>(rawStrings.size()), rawStrings.data());
    }

} // namespace gh_cmd

#endif // !GH_CMD_HPP
