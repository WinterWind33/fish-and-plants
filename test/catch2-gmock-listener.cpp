// Copyright (c) 2023 Andrea Ballestrazzi
#include <catch2-gmock-listener.hpp>

// C++ STL
#include <sstream>
#include <string>

#if defined(USE_CATCH2_AS_TESTING_FRAMEWORK) && defined(USE_GMOCK)

using C2AssertionHandler = Catch::AssertionHandler;
using C2SourceLineInfo = Catch::SourceLineInfo;
using EC2DispositionFlags = Catch::ResultDisposition::Flags;

void GMCatch2Listener::OnTestPartResult(const testing::TestPartResult& result) {
    static constexpr char C2_GMOCK_MACRO_NAME[] = {"GMOCK"};

    // In the following lines we are building the message
    // that will be displayed by Catch2 when a GMock
    // test fails.
    // The message should start like: "[GMOCK] => Failed ..."
    std::ostringstream messageStream{};
    messageStream << "[" << C2_GMOCK_MACRO_NAME << "] => ";
    messageStream << result.message();

    // Here we retrieve info about the failure, i.e. the file
    // name and the line-number.
    const char* const fileName{result.file_name()};
    std::string sourceFilename{};
    if (fileName != nullptr)
        sourceFilename = fileName;
    std::size_t lineNumber{};

    if (result.line_number() >= 0)
        lineNumber = static_cast<std::size_t>(result.line_number());

    // The newly retrieved info are going to be used by Catch2, so we
    // build the following object that will be used by Catch2 assertion
    // handler.
    const C2SourceLineInfo sourceLineInfo{sourceFilename.c_str(), lineNumber};
    EC2DispositionFlags resultDisposition{};

    // Based on what the GMock result is, we need different
    // Catch2 dispositions.
    if (result.fatally_failed())
        resultDisposition = EC2DispositionFlags::Normal;
    else if (result.nonfatally_failed())
        resultDisposition = EC2DispositionFlags::ContinueOnFailure;
    else
        return;

    // Now we can finally handle the newly built assertion.
    C2AssertionHandler assertionHandler{C2_GMOCK_MACRO_NAME, sourceLineInfo, "", resultDisposition};
    assertionHandler.handleMessage(Catch::ResultWas::ExplicitFailure, messageStream.str());
    assertionHandler.complete();
}

#endif  // USE_CATCH2_AS_TESTING_FRAMEWORK && USE_GMOCK
