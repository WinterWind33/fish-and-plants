// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands-factory.hpp>

#include <gh_cmd/gh_cmd.hpp>
#include <version/version-numbers.hpp>

#include <project-management/project-io/project-writer.hpp>

// C++ STL
#include <string_view>
#include <chrono>

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

        return optionParser;
    }

    auto ProjectCommandFactory::create_event_handler_map() -> command_type::event_handler_map {
        command_type::event_handler_map eventHandlerMap{};

        eventHandlerMap.emplace(
            std::string{details::PROJECT_CMD_CREATE_OPTION_LONG_NAME},
            [this](const command_type::option_parser::const_option_pointer& ptr){
                const auto& valueOption{dynamic_cast<const gh_cmd::Value<char, std::string>&>(*ptr)};

                m_projectController.get().setCurrentProject(
                    gc::project_management::Project{
                        std::chrono::system_clock::now(),
                        valueOption.value(),
                        version::getApplicationVersion()
                    }
                );
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

                const auto& project{m_projectController.get().getCurrentProject()};
                const std::filesystem::path outputFilePath{project.getTitle() + ".json"};

                auto projectWriter{gc::project_management::project_io::createJsonProjectFileWriter(outputFilePath)};

                m_userLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
                m_mainLogger->logInfo("Saving project to " + outputFilePath.string() + ".");
                *projectWriter << project;
            }
        );

        return eventHandlerMap;
    }

} // namespace rpi_gc::commands_factory
