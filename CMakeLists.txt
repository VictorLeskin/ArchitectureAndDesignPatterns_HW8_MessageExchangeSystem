cmake_minimum_required(VERSION 3.14)
project(ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  MessageExchangeSystem

  source/ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp 
  source/ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.cpp 
  source/aMessageBroker.hpp
  source/aMessageBroker.cpp
  source/cMessage.hpp
  source/cMessage.cpp
  source/cEndPoint.hpp
  source/cEndPoint.cpp
  source/cIoC.hpp
  source/cIoC.cpp
#add source file here
  source/tests/test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.cpp
  source/tests/test_aMessageBroker.cpp
  source/tests/test_cMessage.cpp
  source/tests/test_cEndPoint.cpp
  source/tests/test_cIoC.cpp
#add test file here
)

target_include_directories(
   MessageExchangeSystem PRIVATE 
   source
)

target_link_libraries(
  MessageExchangeSystem
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest GoogleMock)
gtest_discover_tests(MessageExchangeSystem)