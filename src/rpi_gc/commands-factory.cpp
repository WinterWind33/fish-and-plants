// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands-factory.hpp>

#include <application-configuration.hpp>

#include <gh_cmd/gh_cmd.hpp>
#include <project-loader.hpp>
#include <version/version-numbers.hpp>

// Third-party
#include <folder-provider/folder-provider.hpp>
#include <project-management/integrity-check/title-integrity-checker.hpp>
#include <project-management/integrity-check/version-integrity-checker.hpp>
#include <project-management/project-io/project-reader.hpp>
#include <project-management/project-io/project-writer.hpp>

// C++ STL
#include <chrono>
#include <fstream>
#include <stdexcept>

namespace rpi_gc::commands_factory {

namespace details {
constexpr std::string_view PROJECT_CMD_OPTION_PARSER_DESCRIPTION{"Project Command Options"};
constexpr std::string_view PROJECT_CMD_CREATE_OPTION_LONG_NAME{"create"};
} // namespace details

ProjectCommandFactory::ProjectCommandFactory(
    std::ostream& ost, std::istream& ist, gc_project::ProjectController& projectController) noexcept
    : m_inputStream{ist},
      m_outputStream{ost},
      m_projectController{projectController} {}

auto ProjectCommandFactory::create() -> std::unique_ptr<command_type> {
    return std::make_unique<command_type>(create_option_parser(), create_event_handler_map());
}

auto ProjectCommandFactory::create_option_parser() const -> command_type::option_parser_pointer {
    using parser_type = gh_cmd::DefaultOptionParser<char>;
    using parser_pointer = std::unique_ptr<gh_cmd::DefaultOptionParser<char>>;

    parser_pointer optionParser{
        std::make_unique<parser_type>(std::string{details::PROJECT_CMD_OPTION_PARSER_DESCRIPTION})};
    optionParser->addSwitch(
        std::make_shared<gh_cmd::Switch<char>>('h', "help", "Displays the help page"));

    optionParser->addOption(std::make_shared<gh_cmd::Value<char, std::string>>(
        'c',                                                       // short name
        std::string{details::PROJECT_CMD_CREATE_OPTION_LONG_NAME}, // long name
        "Creates a new project with the specified name.",          // Description
        "gc-project"                                               // Default value.
        ));

    optionParser->addSwitch(std::make_shared<gh_cmd::Switch<char>>(
        's', "save",
        "If a project is loaded it serializes it to file named <project-name>.json. If no project "
        "is loaded nothing happens."));

    optionParser->addOption(std::make_shared<gh_cmd::Value<char, std::string>>(
        'S', "save-to", "If a project is loaded it serialized it to the specified file."));

    optionParser->addOption(std::make_shared<gh_cmd::Value<char, std::string>>(
        'l', "load", "Loads the given project."));

    return optionParser;
}

auto ProjectCommandFactory::create_event_handler_map() -> command_type::event_handler_map {
    command_type::event_handler_map eventHandlerMap{};

    eventHandlerMap.emplace(
        std::string{details::PROJECT_CMD_CREATE_OPTION_LONG_NAME},
        [this](const command_type::option_parser::const_option_pointer& ptr) {
            const auto& valueOption{dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr)};

            // If there is an open project we need to save it before proceeding.
            if (m_projectController.get().hasProject())
                save_current_project("Saving old project before switching to new project.");

            m_projectController.get().setCurrentProject(gc::project_management::Project{
                std::chrono::system_clock::now(), valueOption.value(),
                version::getApplicationVersion()});

            m_userLogger->logInfo("Switched to new project " + valueOption.value());
            m_mainLogger->logInfo("Switched to new project " + valueOption.value());
        });

    eventHandlerMap.emplace(
        "save", [this](const command_type::option_parser::const_option_pointer& ptr) {
            // If there isn't any valid project loaded, there is no need
            // to save it to file.
            if (!m_projectController.get().hasProject()) {
                m_userLogger->logWarning("No project is loaded. Nothing will be saved.");
                return;
            }

            save_current_project();
        });

    eventHandlerMap.emplace(
        "save-to", [this](const command_type::option_parser::const_option_pointer& ptr) {
            // If there isn't any valid project loaded, there is no need
            // to save it to file.
            if (!m_projectController.get().hasProject()) {
                m_userLogger->logWarning("No project is loaded. Nothing will be saved.");
                return;
            }

            const auto& valueOption(dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr));
            const std::filesystem::path outputFilePath{valueOption.value()};

            m_projectController.get().setCurrentProjectFilePath(outputFilePath);
            save_current_project();
        });

    eventHandlerMap.emplace(
        "load", [this](const command_type::option_parser::const_option_pointer& ptr) {
            const auto& valueOption(dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr));

            // If there is an open project we need to save it before proceeding.
            if (m_projectController.get().hasProject()) {
                save_current_project("Saving old project before switching to new project.");
            }

            try {
                std::optional inputProjectOpt{
                    LoadProjectAndCheckIntegrity(valueOption.value(), *m_userLogger)};

                auto& inputProject = inputProjectOpt.value();

                // Now we can set the new project in the project controller.
                m_projectController.get().setCurrentProject(std::move(inputProject));
                m_projectController.get().setCurrentProjectFilePath(valueOption.value());

                // Now we make all components load the configuration from the project.
                m_projectController.get().loadProjectData();

                m_userLogger->logInfo("Switched to new project " + valueOption.value());
                m_mainLogger->logInfo("Switched to new project " + valueOption.value());
            } catch (const std::invalid_argument& exc) {
                const std::string errorString{
                    std::string{"Invalid argument. Cannot load the requested project. Message: "} +
                    exc.what()};

                m_userLogger->logError(errorString);
                m_mainLogger->logError(errorString);
                return;
            } catch (const std::exception& exc) {
                const std::string errorString{
                    std::string{"Generic error. Cannot load the requested project. Message: "} +
                    exc.what()};

                m_userLogger->logError(errorString);
                m_mainLogger->logError(errorString);
                return;
            } catch (...) {
                const std::string errorString{"Unknown error. Cannot load the requested project."};

                m_userLogger->logError(errorString);
                m_mainLogger->logError(errorString);
                return;
            }
        });

    return eventHandlerMap;
}

void ProjectCommandFactory::save_current_project(const std::string& customMessage) {
    utils::SaveProjectAndUpdateConfigFile(m_projectController.get(), m_userLogger, m_mainLogger,
                                          customMessage);
}

namespace utils {

void SaveProjectAndUpdateConfigFile(gc_project::ProjectController& projectController,
                                    const std::shared_ptr<gh_log::Logger>& userLogger,
                                    const std::shared_ptr<gh_log::Logger>& mainLogger,
                                    const std::string& customMessage) {
    if (!projectController.hasProject()) {
        userLogger->logWarning("No project is loaded. Nothing will be saved.");
        mainLogger->logWarning("No project is loaded. Nothing will be saved.");
        return;
    }

    const auto& project{projectController.getCurrentProject()};
    const std::filesystem::path outputFilePath{projectController.getCurrentProjectFilePath()};
    auto projectWriter{
        gc::project_management::project_io::createJsonProjectFileWriter(outputFilePath)};

    if (customMessage.empty()) {
        userLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
        mainLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
    } else {
        userLogger->logInfo(customMessage);
        mainLogger->logInfo(customMessage);
    }

    // Now we make all components save the configuration to the project.
    projectController.collectProjectData();

    *projectWriter << project;

    // Now we update the project path inside the application
    // configuration.
    const ApplicationConfiguration configData{
        CreateDefaultApplicationConfiguration(std::filesystem::absolute(outputFilePath))};

    auto folderProvider{gc::folder_provider::FolderProvider::create()};

    std::ofstream configFile{GetDefaultApplicationConfigFilePath(*folderProvider)};
    configFile << configData;
}

} // namespace utils

} // namespace rpi_gc::commands_factory
