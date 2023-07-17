// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands-factory.hpp>

#include <gh_cmd/gh_cmd.hpp>
#include <version/version-numbers.hpp>

#include <project-management/project-io/project-writer.hpp>
#include <project-management/project-io/project-reader.hpp>

// C++ STL
#include <string_view>
#include <chrono>
#include <stdexcept>

namespace rpi_gc::commands_factory {

    namespace details {
        constexpr std::string_view PROJECT_CMD_OPTION_PARSER_DESCRIPTION{"Project Command Options"};
        constexpr std::string_view PROJECT_CMD_CREATE_OPTION_LONG_NAME{"create"};
    } // namesapce details

    ProjectCommandFactory::ProjectCommandFactory(std::ostream& ost, std::istream& ist, gc_project::ProjectController& projectController) noexcept :
        m_inputStream{ist},
        m_outputStream{ost},
        m_projectController{projectController} {}

    auto ProjectCommandFactory::create() -> std::unique_ptr<command_type> {
        return std::make_unique<command_type>(create_option_parser(), create_event_handler_map());
    }

    auto ProjectCommandFactory::create_option_parser() const -> command_type::option_parser_pointer {
        using parser_type = gh_cmd::DefaultOptionParser<char>;
        using parser_pointer = std::unique_ptr<gh_cmd::DefaultOptionParser<char>>;

        parser_pointer optionParser{std::make_unique<parser_type>(std::string{details::PROJECT_CMD_OPTION_PARSER_DESCRIPTION})};
        optionParser->addSwitch(
            std::make_shared<gh_cmd::Switch<char>>(
                'h',
                "help",
                "Displays the help page"
            )
        );

        optionParser->addOption(
            std::make_shared<gh_cmd::Value<char, std::string>>(
                'c',  // short name
                std::string{details::PROJECT_CMD_CREATE_OPTION_LONG_NAME}, // long name
                "Creates a new project with the specified name.", // Description
                "gc-project" // Default value.
        ));

        optionParser->addSwitch(
            std::make_shared<gh_cmd::Switch<char>>(
                's',
                "save",
                "If a project is loaded it serializes it to file named <project-name>.json. If no project is loaded nothing happens."
        ));

        optionParser->addOption(
            std::make_shared<gh_cmd::Value<char, std::string>>(
                'S',
                "save-to",
                "If a project is loaded it serialized it to the specified file."
        ));

        optionParser->addOption(
            std::make_shared<gh_cmd::Value<char, std::string>>(
                'l',
                "load",
                "Loads the given project."
        ));

        return optionParser;
    }

    auto ProjectCommandFactory::create_event_handler_map() -> command_type::event_handler_map {
        command_type::event_handler_map eventHandlerMap{};

        eventHandlerMap.emplace(
            std::string{details::PROJECT_CMD_CREATE_OPTION_LONG_NAME},
            [this](const command_type::option_parser::const_option_pointer& ptr){
                const auto& valueOption{dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr)};

                // If there is an open project we need to save it before proceeding.
                if(m_projectController.get().hasProject())
                    save_current_project("Saving old project before switching to new project.");

                m_projectController.get().setCurrentProject(
                    gc::project_management::Project{
                        std::chrono::system_clock::now(),
                        valueOption.value(),
                        version::getApplicationVersion()
                    }
                );

                m_userLogger->logInfo("Switched to new project " + valueOption.value());
                m_mainLogger->logInfo("Switched to new project " + valueOption.value());
            }
        );

        eventHandlerMap.emplace(
            "save",
            [this](const command_type::option_parser::const_option_pointer& ptr) {
                // If there isn't any valid project loaded, there is no need
                // to save it to file.
                if(!m_projectController.get().hasProject()) {
                    m_userLogger->logWarning("No project is loaded. Nothing will be saved.");
                    return;
                }

                save_current_project();
            }
        );

        eventHandlerMap.emplace(
            "save-to",
            [this](const command_type::option_parser::const_option_pointer& ptr) {
                // If there isn't any valid project loaded, there is no need
                // to save it to file.
                if(!m_projectController.get().hasProject()) {
                    m_userLogger->logWarning("No project is loaded. Nothing will be saved.");
                    return;
                }

                const auto& valueOption(dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr));
                const std::filesystem::path outputFilePath{valueOption.value()};

                m_projectController.get().setCurrentProjectFilePath(outputFilePath);
                save_current_project();
            }
        );

        eventHandlerMap.emplace(
            "load",
            [this](const command_type::option_parser::const_option_pointer& ptr) {
                const auto& valueOption(dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr));

                // If there is an open project we need to save it before proceeding.
                if(m_projectController.get().hasProject()) {
                    save_current_project("Saving old project before switching to new project.");
                }

                try{
                    auto inputJsonReader{gc::project_management::project_io::CreateJsonProjectFileReader(valueOption.value())};

                    gc::project_management::Project inputProject{};
                    *inputJsonReader >> inputProject;

                    if(m_projectController.get().hasProject() && m_projectController.get().getCurrentProject() == inputProject) {
                        m_userLogger->logWarning("The loaded project is the same as the current one. Skipping this action.");
                        return;
                    }

                    // Now we can set the new project in the project controller.
                    m_projectController.get().setCurrentProject(inputProject);
                    m_projectController.get().setCurrentProjectFilePath(valueOption.value());

                    m_userLogger->logInfo("Switched to new project " + valueOption.value());
                    m_mainLogger->logInfo("Switched to new project " + valueOption.value());
                } catch(const std::invalid_argument& exc) {
                    const std::string errorString{std::string{"Cannot load the requested project. Message: "} + exc.what()};

                    m_userLogger->logError(errorString);
                    m_mainLogger->logError(errorString);
                    return;
                }
            }
        );

        return eventHandlerMap;
    }

    void ProjectCommandFactory::save_current_project(const std::string& customMessage) {
        const auto& project{m_projectController.get().getCurrentProject()};
        const std::filesystem::path outputFilePath{m_projectController.get().getCurrentProjectFilePath()};
        auto projectWriter{gc::project_management::project_io::createJsonProjectFileWriter(outputFilePath)};

        if(customMessage.empty()) {
            m_userLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
            m_mainLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
        } else {
            m_userLogger->logInfo(customMessage);
            m_mainLogger->logInfo(customMessage);
        }

        *projectWriter << project;
    }

} // namespace rpi_gc::commands_factory
