// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <chrono>
#include <string>
#include <variant>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <type_traits>
#include <concepts>

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

        struct StructureNode {
            std::map<Key, Value> Values{};
            std::map<Key, ValueArray> ValueArrays{};
            std::map<Key, StructureNode> Objects{};
            std::map<Key, std::vector<StructureNode>> ObjectsArray{};
        };

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
            m_values[std::forward<decltype(key)>(key)].emplace(std::forward<decltype(value)>(value));
            return *this;
        }

        const auto& getValues() const noexcept {
            return m_values;
        }

    protected:
        std::map<std::string, value_impl_type> m_values{};
        std::map<std::string, std::vector<value_impl_type>> m_valuesArray{};
        std::map<std::string, std::vector<ProjectNode>> m_objects{};
    };

    //!!
    //! \brief Represent the class of a project of the greenhouse CAD. This class happens to be
    //!  versioned and can be extended through a component architecture.
    //!
    class Project final {
    public:
        using time_point_type = std::chrono::time_point<std::chrono::system_clock>;
        using project_version = semver::version;
        using project_title = std::string;
        using structure = project_fields::StructureNode;

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

        auto& addValueField(project_fields::Key key, project_fields::ProjectValueType auto&& value) noexcept {
            m_projectStructure.Values[key] = std::forward<decltype(value)>(value);

            return m_projectStructure.Values;
        }

        auto& getStructure() noexcept {
            return m_projectStructure;
        }

        const auto& getStructure() const noexcept {
            return m_projectStructure;
        }


    private:
        time_point_type m_creationTimePoint;
        project_title m_projectTitle{};
        project_version m_projectVersion{};

        structure m_projectStructure{};
    };

    inline bool SoftCompareProjects(const Project& lhs, const Project& rhs) noexcept {
        return lhs.getCreationTime() == rhs.getCreationTime() &&
            lhs.getTitle() == rhs.getTitle() && lhs.getVersion() == rhs.getVersion();
    }

} // namespace gc::project_management
