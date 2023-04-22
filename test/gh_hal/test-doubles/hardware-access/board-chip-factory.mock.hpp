// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-chip.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>

// C++ STL
#include <memory>
#include <type_traits>

namespace gh_hal::hardware_access::mocks {

    class BoardChipFactoryMockImpl {
    public:
        using chip_pointer = std::unique_ptr<BoardChipFactory::board_chip_type>;

        virtual ~BoardChipFactoryMockImpl() noexcept = default;

        MOCK_METHOD(chip_pointer, openChipByPathMock, (std::filesystem::path), ());
    };

    template<typename MockImpl>
    concept BoardChipFactoryMockImplType = std::is_base_of_v<BoardChipFactoryMockImpl, MockImpl> || requires (MockImpl M, std::filesystem::path P) {
        typename MockImpl::chip_pointer;
        { M.openChipByPathMock(P) } -> std::same_as<BoardChipFactoryMockImpl::chip_pointer>;
    };

    static_assert(BoardChipFactoryMockImplType<BoardChipFactoryMockImpl>);

    template<BoardChipFactoryMockImplType MockImplType>
    struct BoardChipFactoryMock final {
        using chip_pointer = typename MockImplType::chip_pointer;
        static std::unique_ptr<MockImplType> MockImplPointer{};

        static chip_pointer openChipByPath(std::filesystem::path chipPath) {
            assert(BoardChipFactoryMock::BoardChipFactoryMock != nullptr);

            return BoardChipFactoryMock::BoardChipFactoryMock.openChipByPathMock(std::move(chipPath));
        }
    };

} // namespace gh_hal::hardware_access::mocks

#endif // USE_GMOCK
