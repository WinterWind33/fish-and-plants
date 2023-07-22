// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <cstdint>
#include <type_traits>
#include <concepts>
#include <chrono>
#include <string>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>

namespace gc::project_management {

    template<typename T>
    concept ProjectFieldValue = std::integral<std::decay_t<T>> || std::floating_point<std::decay_t<T>> || std::same_as<std::decay_t<T>, std::string>;

    template<typename T>
    concept ProjectFieldKey = std::convertible_to<T, std::string>;

    class ProjectNode {
    public:
        using value_impl_type = std::variant<bool, std::int64_t, std::uint64_t, double, std::string>;

        auto& addValue(ProjectFieldKey auto&& key, ProjectFieldValue auto&& value) {
            m_values[std::forward<decltype(key)>(key)] = std::forward<decltype(value)>(value);
            return *this;
        }

        auto& addValue(ProjectFieldKey auto&& key, value_impl_type&& value) {
            m_values[std::forward<decltype(key)>(key)] = std::move(value);
            return *this;
        }

        auto& addValueArray(ProjectFieldKey auto&& key, std::ranges::range auto&& arr) {
            std::vector<value_impl_type> finalArr{};

            // We need to construct the final array starting from the array one.
            // To do this, we transform the first array and we move the value to the new
            // one.
            std::transform(std::begin(arr), std::end(arr), std::back_inserter(finalArr), [](auto&& val) -> value_impl_type {
                return value_impl_type{std::forward<decltype(val)>(val)};
            });

            m_valuesArrays.emplace(std::forward<decltype(key)>(key), std::move(finalArr));
            return *this;
        }

        template<ProjectFieldValue ValueType>
        auto& addValueArray(ProjectFieldKey auto&& key, std::initializer_list<ValueType> items) {
            return addValueArray<decltype(key), decltype(items)>(std::forward<decltype(key)>(key), std::forward<decltype(items)>(items));
        }

        auto& addObject(ProjectFieldKey auto&& key, ProjectNode&& node) {
            m_objects[std::forward<decltype(key)>(key)] = std::move(node);
            return *this;
        }

        bool contains(ProjectFieldKey auto&& key) const noexcept {
            return m_values.contains(std::forward<decltype(key)>(key)) ||
                m_valuesArrays.contains(std::forward<decltype(key)>(key)) ||
                m_objects.contains(std::forward<decltype(key)>(key));
        }

        bool containsValue(ProjectFieldKey auto&& key) const noexcept {
            return m_values.contains(std::forward<decltype(key)>(key));
        }

        bool containsValueArray(ProjectFieldKey auto&& key) const noexcept {
            return m_valuesArrays.contains(std::forward<decltype(key)>(key));
        }

        bool containsObject(ProjectFieldKey auto&& key) const noexcept {
            return m_objects.contains(std::forward<decltype(key)>(key));
        }

        template<ProjectFieldValue ValueType>
        ValueType getValue(ProjectFieldKey auto&& key) const {
            return std::get<ValueType>(m_values.at(std::forward<decltype(key)>(key)));
        }

        const auto& getValueArray(ProjectFieldKey auto&& key) const {
            return m_valuesArrays.at(std::forward<decltype(key)>(key));
        }

        const auto& getObject(ProjectFieldKey auto&& key) const {
            return m_objects.at(std::forward<decltype(key)>(key));
        }

        [[nodiscard]]
        const auto& getValues() const noexcept {
            return m_values;
        }

        [[nodiscard]]
        const auto& getValuesArrays() const noexcept {
            return m_valuesArrays;
        }

        [[nodiscard]] const auto& getObjects() const noexcept {
            return m_objects;
        }

    protected:
        std::map<std::string, value_impl_type> m_values{};
        std::map<std::string, std::vector<value_impl_type>> m_valuesArrays{};
        std::map<std::string, ProjectNode> m_objects{};
    };

    using ProjectFieldObject = ProjectNode;

    //!!
    //! \brief Represent the class of a project of the greenhouse CAD. This class happens to be
    //!  versioned and can be extended through a component architecture.
    //!
    class Project final : public ProjectNode {
    public:
        using time_point_type = std::chrono::time_point<std::chrono::system_clock>;
        using project_version = semver::version;
        using project_title = std::string;
        using structure = ProjectNode;

        Project() noexcept = default;

        explicit Project(
            time_point_type creationTime, project_title projectTitle, project_version version) noexcept :
            m_creationTimePoint{creationTime},
            m_projectTitle{std::move(projectTitle)},
            m_projectVersion{version} {}

        [[nodiscard]]
        constexpr time_point_type getCreationTime() const noexcept {
            return m_creationTimePoint;
        }

        [[nodiscard]]
        project_title getTitle() const noexcept {
            return m_projectTitle;
        }

        [[nodiscard]]
        constexpr project_version getVersion() const noexcept {
            return m_projectVersion;
        }

        [[nodiscard]]
        auto& getStructure() noexcept {
            return m_projectStructure;
        }

        [[nodiscard]]
        const auto& getStructure() const noexcept {
            return m_projectStructure;
        }


    private:
        time_point_type m_creationTimePoint;
        project_title m_projectTitle{};
        project_version m_projectVersion{};

        structure m_projectStructure{};
    };

    [[nodiscard]]
    inline bool SoftCompareProjects(const Project& lhs, const Project& rhs) noexcept {
        return lhs.getCreationTime() == rhs.getCreationTime() &&
            lhs.getTitle() == rhs.getTitle() && lhs.getVersion() == rhs.getVersion();
    }

} // namespace gc::project_management
