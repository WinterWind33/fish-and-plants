// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#ifdef USE_POPL20
#include <popl20/popl.hpp>
#else
#error "There is no alternative to POPL20 right now, you must choose it."
#endif // USE_POPL20

// C++ STL
#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

namespace gh_cmd {

//! \brief Represents a visitor used to access the internal implementation of command
//!  options as they are wrappers for third-party definitions. This visitor can modify the
//!  state of the implementation.
//! \tparam OptionType The type of the option that need to be passed to the visitor.
//!  Can be a smart pointer, a reference etc.
template <typename OptionType>
struct OptionVisitor {
    virtual ~OptionVisitor() noexcept = default;
    virtual void visit(OptionType option) noexcept = 0;
};

//! \brief Represents a read-only visitor used to access the internal implementation of command
//!  options as they are wrappers for third-party definitions.
//! \tparam OptionType The type of the option that need to be passed to the visitor.
//!  Can be a smart pointer, a reference etc.
template <typename OptionType>
struct ConstOptionVisitor {
    virtual void visit(const OptionType option) const noexcept = 0;
};

//! \brief Represents the basic interface of a command's option.
//!
//! \tparam CharType The type of the characters to be used.
template <typename CharType>
class CommandOption {
public:
    using base_impl_type = popl::Option;
    using char_type = std::decay_t<CharType>;
    using string_type = std::basic_string<char_type>;
    using short_name_type = char_type;
    using long_name_type = string_type;

    virtual ~CommandOption() noexcept = default;

    //! \brief Retrieves the short name of this option.
    [[nodiscard]] virtual short_name_type getShortName() const noexcept = 0;

    //! \brief Retrieves the long name of this option.
    [[nodiscard]] virtual long_name_type getLongName() const noexcept = 0;

    //! \brief Retrieves the description of this option.
    [[nodiscard]] virtual string_type getDescription() const noexcept = 0;

    //! \brief Accepts an external read-only visitor.
    virtual void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>&
                                   visitor) const noexcept = 0;

    //! \brief Accepts an external visitor that can modify the internal implementation's state.
    virtual void acceptVisitor(
        OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept = 0;

    //! \brief Checks whether or not this option was set at least once in the given input lines.
    [[nodiscard]] virtual bool isSet() const noexcept = 0;

    //! \brief Clears the internal option state, resetting it.
    //!  After this call, isSet() == false.
    virtual void clear() noexcept = 0;
};

template <typename CharType, typename ValueType>
class ValuedOption : public CommandOption<CharType> {
public:
    using value_type = std::decay_t<ValueType>;

    virtual value_type value() const noexcept = 0;
};

template <typename CharType, typename ValueType>
class Value : public ValuedOption<CharType, ValueType> {
public:
    using typename CommandOption<CharType>::char_type;
    using typename CommandOption<CharType>::string_type;
    using typename CommandOption<CharType>::short_name_type;
    using typename CommandOption<CharType>::long_name_type;
    using typename CommandOption<CharType>::base_impl_type;
    using typename ValuedOption<CharType, ValueType>::value_type;

    using impl_type = popl::Value<ValueType>;
    static_assert(std::is_same_v<char_type, char>, "Only char is accepted as a valid char type.");

    Value(short_name_type shortName, long_name_type longName, string_type description,
          value_type defaultValue = value_type{}) noexcept;

    short_name_type getShortName() const noexcept override;
    long_name_type getLongName() const noexcept override;
    string_type getDescription() const noexcept override;

    void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor)
        const noexcept override;
    void acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept override;

    [[nodiscard]] bool isSet() const noexcept override;
    value_type value() const noexcept override;
    void clear() noexcept override;

private:
    std::shared_ptr<impl_type> m_valueImpl{};
};

//! \brief Represents an option with boolean value.
//!
//! \tparam CharType The char type to be used.
template <typename CharType>
class Switch : public ValuedOption<CharType, bool> {
public:
    using typename ValuedOption<CharType, bool>::value_type;
    using typename ValuedOption<CharType, bool>::char_type;
    using typename ValuedOption<CharType, bool>::string_type;
    using typename ValuedOption<CharType, bool>::short_name_type;
    using typename ValuedOption<CharType, bool>::long_name_type;
    using typename ValuedOption<CharType, bool>::base_impl_type;

    static_assert(std::is_same_v<char_type, char>, "Only char is accepted as a valid char type.");

    using impl_type = popl::Switch;

    Switch(short_name_type shortName, long_name_type longName, string_type description) noexcept;

    short_name_type getShortName() const noexcept override;
    long_name_type getLongName() const noexcept override;
    string_type getDescription() const noexcept override;

    void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor)
        const noexcept override;
    void acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept override;

    [[nodiscard]] bool isSet() const noexcept override;
    value_type value() const noexcept override;
    void clear() noexcept override;

private:
    // We use shared_ptr for now as the popl implementation
    // provides APIs with a shared ptr.
    std::shared_ptr<impl_type> m_switchImpl{};
};

//!!
//! \brief Represents an option with implicit value. This means that the
//!  user does not need to type a value for this option, but it is implicitly
//!  set to a default value if the user does not type anything.
//!
template <typename CharType, typename ValueType>
class ImplicitValue : public ValuedOption<CharType, ValueType> {
public:
    using typename CommandOption<CharType>::char_type;
    using typename CommandOption<CharType>::string_type;
    using typename CommandOption<CharType>::short_name_type;
    using typename CommandOption<CharType>::long_name_type;
    using typename CommandOption<CharType>::base_impl_type;
    using typename ValuedOption<CharType, ValueType>::value_type;

    using impl_type = popl::Implicit<ValueType>;
    static_assert(std::is_same_v<char_type, char>, "Only char is accepted as a valid char type.");

    explicit ImplicitValue(short_name_type shorName, long_name_type longName,
                           string_type description, value_type defaultValue) noexcept;

    [[nodiscard]] short_name_type getShortName() const noexcept override;
    [[nodiscard]] long_name_type getLongName() const noexcept override;
    [[nodiscard]] string_type getDescription() const noexcept override;

    void acceptVisitor(const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor)
        const noexcept override;
    void acceptVisitor(OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept override;

    [[nodiscard]] bool isSet() const noexcept override;
    [[nodiscard]] value_type value() const noexcept override;
    void clear() noexcept override;

private:
    std::shared_ptr<impl_type> m_implicitValueImpl{};
};

//! \brief Represents the basic interface of an option parser, i.e. an object that,
//!  given an input line, it extracts all of the command's options.
//!
//! \tparam CharT The char type to be used.
template <typename CharT>
struct OptionParser {
    using char_type = std::decay_t<CharT>;
    using string_type = std::basic_string<char_type>;
    using option_pointer = std::shared_ptr<CommandOption<char_type>>;
    using const_option_pointer = std::shared_ptr<const CommandOption<char_type>>;

    virtual ~OptionParser() noexcept = default;

    //! \brief Adds an option to the option parser.
    virtual void addOption(option_pointer option) noexcept = 0;

    //! \brief Adds a switch to the command representation.
    virtual void addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept = 0;

    //! \brief Parses the given input tokens searching for options, unknown options and
    //!  non-options tokens. These tokens can be retrieve by the getters below.
    virtual void parse(const std::vector<string_type>& args) = 0;

    //! \brief Resets the state of this parser, erasing all of the previous saved
    //!  options, unknowns etc.
    virtual void reset() noexcept = 0;

    //! \brief Prints to the given output stream the help page extracted from
    //!  the configuration of this parser.
    virtual void printHelp(std::basic_ostream<char_type>& outputStream) const noexcept = 0;

    //! \brief Retrieves all of the options saved inside this parser configuration.
    virtual std::vector<const_option_pointer> getOptions() const noexcept = 0;

    //! \brief Retrieves all of the options saved inside this parser configuration.
    virtual std::vector<option_pointer> getOptions() noexcept = 0;

    //! \brief Retrieves all of the non-option arguments parsed by this object.
    virtual std::vector<string_type> getNonOptionArguments() const noexcept = 0;

    //! \brief Retrieves all of the unknown options parsed by this object.
    virtual std::vector<string_type> getUnknownOptions() const noexcept = 0;
};

template <typename CharType>
class DefaultOptionParser : public OptionParser<CharType> {
public:
    using typename OptionParser<CharType>::char_type;
    using typename OptionParser<CharType>::string_type;
    using typename OptionParser<CharType>::const_option_pointer;
    using typename OptionParser<CharType>::option_pointer;

    using impl_type = popl::OptionParser;
    static_assert(std::is_same_v<CharType, char>, "Only char is accepted as a valid char type");

    DefaultOptionParser() noexcept;
    DefaultOptionParser(string_type description) noexcept;

    void addOption(std::shared_ptr<CommandOption<char_type>> option) noexcept override;
    void addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept override;
    void parse(const std::vector<string_type>& args) override;
    void reset() noexcept override;
    void printHelp(std::basic_ostream<char_type>& outputStream) const noexcept override;

    std::vector<const_option_pointer> getOptions() const noexcept override;
    std::vector<option_pointer> getOptions() noexcept override;
    std::vector<string_type> getUnknownOptions() const noexcept override;
    std::vector<string_type> getNonOptionArguments() const noexcept override;

private:
    using option_ptr = std::shared_ptr<CommandOption<char_type>>;

    std::unique_ptr<impl_type> m_implParser{};
    std::vector<option_ptr> m_options{};
};

// Switch implementation
template <typename C>
inline Switch<C>::Switch(short_name_type shortName, long_name_type longName,
                         string_type description) noexcept {
    const std::array<short_name_type, 2> shortNameRawString{shortName, 0};

    m_switchImpl = std::make_shared<impl_type>(shortNameRawString.data(), std::move(longName),
                                               std::move(description));
}

template <typename C>
inline auto Switch<C>::getShortName() const noexcept -> short_name_type {
    return m_switchImpl->short_name();
}

template <typename C>
inline auto Switch<C>::getLongName() const noexcept -> long_name_type {
    return m_switchImpl->long_name();
}

template <typename C>
inline auto Switch<C>::getDescription() const noexcept -> string_type {
    return m_switchImpl->description();
}

template <typename C>
inline void Switch<C>::acceptVisitor(
    const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept {
    visitor.visit(m_switchImpl);
}

template <typename C>
inline void Switch<C>::acceptVisitor(
    OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept {
    visitor.visit(m_switchImpl);
}

template <typename C>
inline bool Switch<C>::isSet() const noexcept {
    return m_switchImpl->count() > 0 && value();
}

template <typename C>
inline auto Switch<C>::value() const noexcept -> value_type {
    // The index of the value to check for inside the popl internal state.
    const std::size_t historyEntryIndex{m_switchImpl->count() - 1};
    return m_switchImpl->value_or(false, historyEntryIndex);
}

template <typename C>
inline void Switch<C>::clear() noexcept {
    m_switchImpl->set_value(false);
}

// Value implementation
template <typename C, typename V>
inline Value<C, V>::Value(short_name_type shortName, long_name_type longName,
                          string_type description, value_type defaultValue) noexcept {
    const std::array<short_name_type, 2> shortNameRawString{shortName, 0};

    m_valueImpl = std::make_shared<impl_type>(shortNameRawString.data(), std::move(longName),
                                              std::move(description), std::move(defaultValue));
    // We add the default value so that isSet() can properly return a valid value.
    Value::clear();
}

template <typename C, typename V>
inline auto Value<C, V>::getShortName() const noexcept -> short_name_type {
    return m_valueImpl->short_name();
}

template <typename C, typename V>
inline auto Value<C, V>::getLongName() const noexcept -> long_name_type {
    return m_valueImpl->long_name();
}

template <typename C, typename V>
inline auto Value<C, V>::getDescription() const noexcept -> string_type {
    return m_valueImpl->description();
}

template <typename C, typename V>
inline void Value<C, V>::acceptVisitor(
    const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept {
    visitor.visit(m_valueImpl);
}

template <typename C, typename V>
inline void Value<C, V>::acceptVisitor(
    OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept {
    visitor.visit(m_valueImpl);
}

template <typename C, typename V>
inline bool Value<C, V>::isSet() const noexcept {
    // This is just a temporary check. The value has always a value, i.e. the default one
    // in the internal state. So when the user types a value he adds another value to this
    // internal array.
    return m_valueImpl->count() > 1;
}

template <typename C, typename V>
inline auto Value<C, V>::value() const noexcept -> value_type {
    // The index of the value to check for inside the popl internal state.
    const std::size_t historyEntryIndex{m_valueImpl->count() - 1};
    return m_valueImpl->value_or(value_type{}, historyEntryIndex);
}

template <typename C, typename V>
inline void Value<C, V>::clear() noexcept {
    m_valueImpl->set_value(value_type{});
}

// ImplicitValue implementation
template <typename C, typename V>
inline ImplicitValue<C, V>::ImplicitValue(short_name_type shortName, long_name_type longName,
                                          string_type description, value_type defaultValue) noexcept
    : m_implicitValueImpl{std::make_shared<impl_type>(std::string{shortName}, longName, description,
                                                      defaultValue)} {}

template <typename C, typename V>
inline auto ImplicitValue<C, V>::getShortName() const noexcept -> short_name_type {
    return m_implicitValueImpl->short_name();
}

template <typename C, typename V>
inline auto ImplicitValue<C, V>::getLongName() const noexcept -> long_name_type {
    return m_implicitValueImpl->long_name();
}

template <typename C, typename V>
inline auto ImplicitValue<C, V>::getDescription() const noexcept -> string_type {
    return m_implicitValueImpl->description();
}

template <typename C, typename V>
inline void ImplicitValue<C, V>::acceptVisitor(
    const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>& visitor) const noexcept {
    visitor.visit(m_implicitValueImpl);
}

template <typename C, typename V>
inline void ImplicitValue<C, V>::acceptVisitor(
    OptionVisitor<std::shared_ptr<base_impl_type>>& visitor) noexcept {
    visitor.visit(m_implicitValueImpl);
}

template <typename C, typename V>
inline bool ImplicitValue<C, V>::isSet() const noexcept {
    // In this case, the value is set only if the user types a value.
    // The default value is not considered.
    return m_implicitValueImpl->count() > 0;
}

template <typename C, typename V>
inline auto ImplicitValue<C, V>::value() const noexcept -> value_type {
    // The index of the value to check for inside the popl internal state.
    const std::size_t historyEntryIndex{m_implicitValueImpl->count() - 1};
    const auto val = m_implicitValueImpl->value_or(value_type{}, historyEntryIndex);
    if (val == m_implicitValueImpl->get_default())
        return value_type{};

    return val;
}

template <typename C, typename V>
inline void ImplicitValue<C, V>::clear() noexcept {
    m_implicitValueImpl->set_value(value_type{});
}

// DefaultOptionParser implementation
template <typename C>
inline DefaultOptionParser<C>::DefaultOptionParser() noexcept
    : m_implParser{std::make_unique<impl_type>()} {}

template <typename C>
inline DefaultOptionParser<C>::DefaultOptionParser(string_type description) noexcept
    : m_implParser{std::make_unique<impl_type>(std::move(description))} {}

template <typename C>
inline auto DefaultOptionParser<C>::getNonOptionArguments() const noexcept
    -> std::vector<string_type> {
    return m_implParser->non_option_args();
}

template <typename C>
inline auto DefaultOptionParser<C>::getUnknownOptions() const noexcept -> std::vector<string_type> {
    return m_implParser->unknown_options();
}

template <typename C>
inline auto DefaultOptionParser<C>::getOptions() const noexcept
    -> std::vector<const_option_pointer> {
    std::vector<const_option_pointer> result{};

    for (const auto& option : m_options)
        result.emplace_back(option);

    return result;
}

template <typename C>
inline auto DefaultOptionParser<C>::getOptions() noexcept -> std::vector<option_pointer> {
    std::vector<option_pointer> result{};

    for (auto& option : m_options)
        result.emplace_back(option);

    return result;
}

template <typename C>
inline void DefaultOptionParser<C>::reset() noexcept {
    m_implParser->reset();
}

template <typename C>
inline void DefaultOptionParser<C>::printHelp(
    std::basic_ostream<char_type>& outputStream) const noexcept {
    outputStream << m_implParser->help();
}

namespace details {
// Internal use only, should not be used outside the wrapper.
template <typename OptionType>
class CommandOptionInserterVisitor : public OptionVisitor<std::shared_ptr<popl::Option>> {
public:
    using option_parser_ref = std::reference_wrapper<popl::OptionParser>;

    CommandOptionInserterVisitor(option_parser_ref optionParser) noexcept
        : m_optionParser{optionParser} {}

    void visit(std::shared_ptr<popl::Option> option) noexcept override {
        std::shared_ptr<OptionType> optionPtr{
            std::static_pointer_cast<OptionType>(std::move(option))};
        m_optionParser.get().template add_shared<OptionType>(std::move(optionPtr));
    }

private:
    option_parser_ref m_optionParser;
};

} // namespace details

template <typename C>
inline void DefaultOptionParser<C>::addOption(
    std::shared_ptr<CommandOption<char_type>> option) noexcept {
    assert(option != nullptr);

    details::CommandOptionInserterVisitor<typename CommandOption<char_type>::base_impl_type>
        inserter{*m_implParser};
    option->acceptVisitor(inserter);

    m_options.push_back(std::move(option));
}

template <typename C>
inline void DefaultOptionParser<C>::addSwitch(std::shared_ptr<Switch<char_type>> option) noexcept {
    assert(option != nullptr);

    details::CommandOptionInserterVisitor<typename Switch<char_type>::impl_type> inserter{
        *m_implParser};
    option->acceptVisitor(inserter);

    m_options.push_back(std::move(option));
}

template <typename C>
inline void DefaultOptionParser<C>::parse(const std::vector<string_type>& args) {
    // Before proceeding we need to create a vector of const char* because the
    // popl parser uses primitive types and not string types.
    std::vector<const char_type*> rawStrings{};
    for (const string_type& str : args)
        rawStrings.push_back(str.c_str());

    m_implParser->parse(static_cast<std::int32_t>(rawStrings.size()), rawStrings.data());
}

namespace utility {

//!!
//! \brief Clears all the given options.
//!
//! \param options The options to clear.
inline void ClearAllOptions(auto&& options) noexcept {
    for (auto& option : options)
        option->clear();
}

} // namespace utility

} // namespace gh_cmd
