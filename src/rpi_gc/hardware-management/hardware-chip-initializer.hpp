// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Modules
#include <gh_hal/hardware-abstraction-layer.hpp>
#include <gh_hal/hardware-access/board-chip.hpp>
#include <gh_log/logger.hpp>

// C++ STL
#include <cassert>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

namespace rpi_gc::hardware_management {

template <gh_hal::hardware_access::BoardChipFactoryType ChipFactory>
class HardwareInitializer final {
public:
    using logger = gh_log::Logger;
    using logger_pointer = std::shared_ptr<logger>;
    using chip_factory = std::decay_t<ChipFactory>;

    explicit HardwareInitializer(logger_pointer logger) noexcept;

    std::unique_ptr<typename chip_factory::board_chip_type> initializeBoardChip(
        std::filesystem::path chipPath) const;

private:
    logger_pointer m_logger{};

    void _logHardwareBackendModuleInfo() const noexcept;
};

template <gh_hal::hardware_access::BoardChipFactoryType ChipFactory>
HardwareInitializer<ChipFactory>::HardwareInitializer(logger_pointer logger) noexcept
    : m_logger{std::move(logger)} {
    assert(static_cast<bool>(m_logger));

    _logHardwareBackendModuleInfo();
}

template <gh_hal::hardware_access::BoardChipFactoryType ChipFactory>
auto HardwareInitializer<ChipFactory>::initializeBoardChip(std::filesystem::path chipPath) const
    -> std::unique_ptr<typename chip_factory::board_chip_type> {
    using result_type = std::unique_ptr<typename chip_factory::board_chip_type>;

    assert(!chipPath.empty());

    m_logger->logInfo("Trying to open the chip identified by: " + chipPath.string());
    result_type result{};

    try {
        result = ChipFactory::openChipByPath(chipPath);
    } catch (const std::system_error& systemError) {
        std::ostringstream errorStream{};
        errorStream << "Failed to open the chip \'" << chipPath << "\' - Error code: ";
        errorStream << systemError.code() << ". Message: ";
        errorStream << systemError.what();

        m_logger->logCritical(errorStream.str());
        throw systemError;
    } catch (const std::exception& genericException) {
        m_logger->logCritical(genericException.what());

        throw(genericException);
    }

    return result;
}

template <gh_hal::hardware_access::BoardChipFactoryType ChipFactory>
void HardwareInitializer<ChipFactory>::_logHardwareBackendModuleInfo() const noexcept {
    std::ostringstream outputStream{};
    outputStream << "Hardware abstraction layer powered by: ";
    outputStream << gh_hal::BackendModule::GetBackendModuleName() << ' ';
    outputStream << gh_hal::BackendModule::GetBackendModuleVersion() << ' ';

    m_logger->logInfo(outputStream.str());
}

} // namespace rpi_gc::hardware_management
