// Copyright (c) 2023 Andrea Ballestrazzi
#include <initial-project-loader.hpp>

#include <project-loader.hpp>

#include <nlohmann/json.hpp>
#include <project-management/project-io/project-reader.hpp>
#include <version/version-numbers.hpp>

// C++ STL
#include <filesystem>
#include <fstream>

namespace rpi_gc {

namespace details {

// Logs the error if the given error code is not valid.
[[nodiscard]] bool logErrorIfNotValid(std::error_code& errorCode, gh_log::Logger& logger) noexcept {
    // The following cast is true when the error code represents a failed error code.
    const bool bRes{static_cast<bool>(errorCode)};
    if (bRes) {
        logger.logError("Error while trying to load project config. Error code: " +
                        std::to_string(errorCode.value()));
        logger.logError("Error message: " + errorCode.message());
    }

    return !bRes;
}

// Creates the standard config file
void createStandardConfigFile(const std::filesystem::path& configFilePath,
                              gh_log::Logger& logger) noexcept {
    std::ofstream configFile{configFilePath};
    if (!configFile.is_open()) {
        logger.logError("Error while trying to create the config file");
        return;
    }

    // Now we create the JSON to write to the config file.
    // We serialize the application version.
    nlohmann::json configJson{};
    configJson["version"] = rpi_gc::version::getApplicationVersion().to_string();

    // Write out the JSON to the config file.
    configFile << configJson.dump(4);
}

} // namespace details

InitialProjectLoader::InitialProjectLoader(
    gh_log::Logger& logger, const gc::folder_provider::FolderProvider& folder_provider) noexcept
    : m_logger{logger},
      m_folderProvider{folder_provider} {}

std::optional<std::pair<gc::project_management::Project, std::filesystem::path>>
InitialProjectLoader::tryLoadCachedProject() noexcept {
    using namespace gc::project_management;
    m_logger.get().logInfo("Initiating cached project loading flow...");

    const auto rpigcFolder = m_folderProvider.get().getAppDataFolder() /
                             std::filesystem::path{"FishAndPlants"} /
                             std::filesystem::path{"rpi_gc"};

    auto configFilePath = rpigcFolder / std::filesystem::path{"rpi_gc_config.json"};

    std::error_code errorCode{};
    if (!std::filesystem::exists(rpigcFolder, errorCode)) {
        m_logger.get().logWarning("Application data folder was not found. Creating it...");

        // If the error code is not zero then there was a system error.
        if (!details::logErrorIfNotValid(errorCode, m_logger.get())) {
            return std::nullopt;
        }

        // We create the folder and the config file.
        std::filesystem::create_directories(rpigcFolder, errorCode);
        if (!details::logErrorIfNotValid(errorCode, m_logger.get())) {
            return std::nullopt;
        }

        details::createStandardConfigFile(configFilePath, m_logger.get());
        return std::nullopt;
    }

    // If we are here this means that the application data folder exists. We
    // must check if the configuration file exists as well.
    if (!std::filesystem::exists(configFilePath, errorCode)) {
        m_logger.get().logWarning("Application config file was not found. Creating it...");

        // If the error code is not zero then there was a system error.
        if (!details::logErrorIfNotValid(errorCode, m_logger.get())) {
            return std::nullopt;
        }

        details::createStandardConfigFile(configFilePath, m_logger.get());
        return std::nullopt;
    }

    // If we are here this means that the application data folder exists and
    // the application config file as well. We can load the information about
    // the last opened project location.
    std::ifstream configFile{configFilePath};
    if (!configFile.is_open()) {
        m_logger.get().logError("Error while trying to open the config file");
        return std::nullopt;
    }

    // Now we parse the JSON.
    nlohmann::json configJson{};
    configFile >> configJson;

    // We check if the version is the same as the current one.
    const auto version = configJson["version"].get<std::string>();

    // If the version is an old one we update it to the last one.
    if (version != rpi_gc::version::getApplicationVersion().to_string()) {
        m_logger.get().logWarning(
            "The application version is different from the one in the config file. Updating it...");
        configJson["version"] = rpi_gc::version::getApplicationVersion().to_string();
    }

    // We check if the project location is present.
    if (!configJson.contains("lastProjectLocation")) {
        m_logger.get().logWarning("Project location not found in the config file");
        return std::nullopt;
    }

    // We check if the project location is valid.
    const auto projectLocation =
        std::filesystem::path{configJson["lastProjectLocation"].get<std::string>()};
    if (!std::filesystem::exists(projectLocation, errorCode)) {
        m_logger.get().logWarning("Project location not found");
        return std::nullopt;
    }

    // If we are here this means that the project location is valid. We can
    // load the project and send it to the standard integrity checks.
    auto projectOpt{LoadProjectAndCheckIntegrity(projectLocation, m_logger.get())};
    if (!projectOpt.has_value()) {
        m_logger.get().logError("Project loading with integrity checks failed.");
        return std::nullopt;
    }

    m_logger.get().logInfo("Project loaded successfully");
    return std::make_pair(projectOpt.value(), projectLocation);
}

} // namespace rpi_gc
