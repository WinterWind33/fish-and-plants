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

namespace gc::project_management {

    namespace project_fields {
        using Key = std::string;
        using Value = std::variant<bool, std::int32_t, std::string>;

        template<typename V>
        concept ProjectValueType = std::is_same_v<V, bool> || std::is_same_v<V, std::int32_t> || std::is_same_v<V, std::string>;

        using Object = std::map<Key, Value>;
        using ValueArray = std::vector<Value>;
        using ObjectArray = std::vector<Object>;

        using Field = std::map<Key, Value>;

        template<typename FieldType>
        concept ProjectFieldType = std::is_same_v<FieldType, Object> ||  std::is_same_v<FieldType, ValueArray> || std::is_same_v<FieldType, ObjectArray>;

    } // namespace project_fields

    template<typename T>
    concept ProjectFieldValue = std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> || std::same_as<T, bool>;

    template<typename T>
    concept ProjectFieldKey = std::convertible_to<T, std::string>;

    class ProjectNode {
    public:
        using value_impl_type = std::variant<bool, std::int32_t, double, std::string>;

        auto& addValue(ProjectFieldKey auto&& key, ProjectFieldValue auto&& value) {
            m_values[std::forward<decltype(key)>(key)] = std::forward<decltype(value)>(value);
            return *this;
        }

        template<ProjectFieldValue ValueType>
        auto& addValueArray(ProjectFieldKey auto&& key, std::vector<ValueType> arr) {
            std::vector<value_impl_type> finalArr{};

            // We need to construct the final array starting from the array one.
            // To do this, we transform the first array and we move the value to the new
            // one.
            std::transform(std::begin(arr), std::end(arr), std::back_inserter(finalArr), [](ValueType& val) -> value_impl_type {
                return value_impl_type{std::move(val)};
            });

            m_valuesArrays.emplace(std::forward<decltype(key)>(key), std::move(finalArr));
            return *this;
        }

        [[nodiscard]]
        const auto& getValues() const noexcept {
            return m_values;
        }

        [[nodiscard]]
        const auto& getValuesArrays() const noexcept {
            return m_valuesArrays;
        }

    protected:
        std::map<std::string, value_impl_type> m_values{};
        std::map<std::string, std::vector<value_impl_type>> m_valuesArrays{};
        std::map<std::string, std::vector<ProjectNode>> m_objects{};
    };

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
