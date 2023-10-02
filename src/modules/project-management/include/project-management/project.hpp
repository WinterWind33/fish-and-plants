// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <map>
#include <ranges>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace gc::project_management {

template <typename T>
concept ProjectFieldValue =
    std::integral<std::decay_t<T>> || std::floating_point<std::decay_t<T>> ||
    std::same_as<std::decay_t<T>, std::string>;

template <typename T>
concept ProjectFieldKey = std::convertible_to<T, std::string>;

namespace details {
template <class>
inline constexpr bool InvalidVariantType = false;
} // namespace details

//!!
//! \brief Represent a node of a project of the greenhouse CAD. This class
//!  can be extended through a component architecture.
class ProjectNode {
public:
    using value_impl_type = std::variant<bool, std::int64_t, std::uint64_t, double, std::string>;

    //!!
    //! \brief Add a value to the node. If the value is already present, it will be overwritten.
    //!
    //! \param key The key of the value.
    //! \param value The value to add.
    //! \return A reference to the node.
    auto& addValue(ProjectFieldKey auto&& key, ProjectFieldValue auto&& value) {
        m_values[std::forward<decltype(key)>(key)] = std::forward<decltype(value)>(value);
        return *this;
    }

    //!!
    //! \brief Add a value to the node. If the value is already present, it will be overwritten.
    //!  This overload directly adds a variant to the internal map.
    //!
    //! \param key The key of the value.
    //! \param value The value to add.
    //! \return A reference to the node.
    auto& addValue(ProjectFieldKey auto&& key, value_impl_type&& value) {
        m_values[std::forward<decltype(key)>(key)] = std::move(value);
        return *this;
    }

    //!!
    //! \brief Add a value array to the node. If the value is already present, it will be
    //! overwritten.
    //!
    //! \param key The key of the value.
    //! \param arr The array to add.
    //! \return A reference to the node.
    auto& addValueArray(ProjectFieldKey auto&& key, std::ranges::range auto&& arr) {
        std::vector<value_impl_type> finalArr{};

        // We need to construct the final array starting from the array one.
        // To do this, we transform the first array and we move the value to the new
        // one.
        std::transform(std::begin(arr), std::end(arr), std::back_inserter(finalArr),
                       [](auto&& val) -> value_impl_type {
                           return value_impl_type{std::forward<decltype(val)>(val)};
                       });

        m_valuesArrays[std::forward<decltype(key)>(key)] = std::move(finalArr);
        return *this;
    }

    //!!
    //! \brief Add a value array to the node. If the value is already present, it will be
    //! overwritten.
    //!  This overload adds a value array to the node starting from an initializer list.
    //!
    //! \tparam ValueType The type of the value.
    //! \param key The key of the value.
    //! \param items The items to add.
    //! \return A reference to the node.
    template <ProjectFieldValue ValueType>
    auto& addValueArray(ProjectFieldKey auto&& key, std::initializer_list<ValueType> items) {
        return addValueArray<decltype(key), decltype(items)>(std::forward<decltype(key)>(key),
                                                             std::forward<decltype(items)>(items));
    }

    //!!
    //! \brief Add an object to the node. If the object is already present, it will be overwritten.
    //!
    //! \param key The key of the object.
    //! \param node The object to add.
    //! \return A reference to the node.
    auto& addObject(ProjectFieldKey auto&& key, ProjectNode&& node) {
        m_objects[std::forward<decltype(key)>(key)] = std::move(node);
        return *this;
    }

    //!!
    //! \brief Add an object array to the node. If the object is already present, it will be
    //! overwritten.
    //!
    auto& addObjectArray(ProjectFieldKey auto&& key, std::ranges::range auto&& arr) {
        std::vector<ProjectNode> finalArr{};

        // We need to construct the final array starting from the given one.
        // To do this, we transform the first array and we move the object to the new
        // one.
        std::transform(std::begin(arr), std::end(arr), std::back_inserter(finalArr),
                       [](auto&& val) -> ProjectNode {
                           return ProjectNode{std::forward<decltype(val)>(val)};
                       });

        m_objectsArrays[std::forward<decltype(key)>(key)] = std::move(finalArr);
        return *this;
    }

    [[nodiscard]] bool contains(ProjectFieldKey auto&& key) const noexcept {
        return m_values.contains(std::forward<decltype(key)>(key)) ||
               m_valuesArrays.contains(std::forward<decltype(key)>(key)) ||
               m_objects.contains(std::forward<decltype(key)>(key)) ||
               m_objectsArrays.contains(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] bool containsValue(ProjectFieldKey auto&& key) const noexcept {
        return m_values.contains(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] bool containsValueArray(ProjectFieldKey auto&& key) const noexcept {
        return m_valuesArrays.contains(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] bool containsObject(ProjectFieldKey auto&& key) const noexcept {
        return m_objects.contains(std::forward<decltype(key)>(key));
    }

    template <ProjectFieldValue ValueType>
    [[nodiscard]] ValueType getValue(ProjectFieldKey auto&& key) const {
        if constexpr (std::is_same_v<ValueType, bool>) {
            return std::get<bool>(m_values.at(std::forward<decltype(key)>(key)));
        } else if constexpr (std::is_integral_v<ValueType> && std::is_signed_v<ValueType>) {
            return std::get<std::int64_t>(m_values.at(std::forward<decltype(key)>(key)));
        } else if constexpr (std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>) {
            return std::get<std::uint64_t>(m_values.at(std::forward<decltype(key)>(key)));
        } else if constexpr (std::is_floating_point_v<ValueType>) {
            return std::get<double>(m_values.at(std::forward<decltype(key)>(key)));
        } else if constexpr (std::is_same_v<ValueType, std::string>) {
            return std::get<std::string>(m_values.at(std::forward<decltype(key)>(key)));
        } else {
            statis_assert(details::InvalidVariantType<ValueType>, "Value type not supported.");
        }
    }

    [[nodiscard]] const auto& getValueArray(ProjectFieldKey auto&& key) const {
        return m_valuesArrays.at(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] const auto& getObject(ProjectFieldKey auto&& key) const {
        return m_objects.at(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] auto& getObject(ProjectFieldKey auto&& key) {
        return m_objects.at(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] const auto& getObjectArray(ProjectFieldKey auto&& key) const {
        return m_objectsArrays.at(std::forward<decltype(key)>(key));
    }

    [[nodiscard]] const auto& getAllObjectArrays() const noexcept {
        return m_objectsArrays;
    }

    [[nodiscard]] const auto& getValues() const noexcept {
        return m_values;
    }

    [[nodiscard]] const auto& getValuesArrays() const noexcept {
        return m_valuesArrays;
    }

    [[nodiscard]] const auto& getObjects() const noexcept {
        return m_objects;
    }

    void removeValue(ProjectFieldKey auto&& key) {
        m_values.erase(std::forward<decltype(key)>(key));
    }

protected:
    std::map<std::string, value_impl_type> m_values{};
    std::map<std::string, std::vector<value_impl_type>> m_valuesArrays{};
    std::map<std::string, ProjectNode> m_objects{};
    std::map<std::string, std::vector<ProjectNode>> m_objectsArrays{};
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

    explicit Project(time_point_type creationTime, project_title projectTitle,
                     project_version version) noexcept
        : m_creationTimePoint{creationTime},
          m_projectTitle{std::move(projectTitle)},
          m_projectVersion{version} {}

    [[nodiscard]] constexpr time_point_type getCreationTime() const noexcept {
        return m_creationTimePoint;
    }

    [[nodiscard]] project_title getTitle() const noexcept {
        return m_projectTitle;
    }

    [[nodiscard]] constexpr project_version getVersion() const noexcept {
        return m_projectVersion;
    }

private:
    time_point_type m_creationTimePoint;
    project_title m_projectTitle{};
    project_version m_projectVersion{};
};

//!!
//! \brief Compare two projects. This function compares the creation time, the title and the
//! version.
//!
//! \param lhs The first project to compare.
//! \param rhs The second project to compare.
//! \return True if the two projects are equal, false otherwise.
[[nodiscard]] inline bool SoftCompareProjects(const Project& lhs, const Project& rhs) noexcept {
    return lhs.getCreationTime() == rhs.getCreationTime() && lhs.getTitle() == rhs.getTitle() &&
           lhs.getVersion() == rhs.getVersion();
}

} // namespace gc::project_management
