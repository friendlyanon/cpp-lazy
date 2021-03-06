cmake_minimum_required(VERSION 3.14)

project(LazyTests LANGUAGES CXX)

# ---- Download Catch2 header ----
# TODO: actually use proper Catch2 integration
# See: https://github.com/catchorg/Catch2/blob/master/docs/cmake-integration.md

set(CPP-LAZY_CATCH_VERSION "2.12.2" CACHE STRING "Version of Catch2 to use for testing")

# Update only when the version downloaded does not match the cache variable
if(NOT CPP-LAZY_CATCH_VERSION_DOWNLOADED VERSION_EQUAL CPP-LAZY_CATCH_VERSION)
    file(DOWNLOAD
            "https://github.com/catchorg/Catch2/releases/download/v${CPP-LAZY_CATCH_VERSION}/catch.hpp"
            "${PROJECT_BINARY_DIR}/catch/catch.hpp")
    set(CPP-LAZY_CATCH_VERSION_DOWNLOADED "${CPP-LAZY_CATCH_VERSION}" CACHE INTERNAL "" FORCE)
endif()

# ---- Import root project ----

option(TEST_INSTALLED_VERSION "Import the library using find_package" OFF)
if(TEST_INSTALLED_VERSION)
    find_package(cpp-lazy REQUIRED CONFIG)
else()
    # Enable warnings from includes
    set(cpp-lazy_INCLUDE_WITHOUT_SYSTEM ON CACHE INTERNAL "")

    include(FetchContent)
    FetchContent_Declare(cpp-lazy SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
    FetchContent_MakeAvailable(cpp-lazy)
endif()

# ---- Tests ----

add_executable(LazyTests
        affirm-tests.cpp
        concatenate-tests.cpp
        enumerate-tests.cpp
        except-tests.cpp
        filter-tests.cpp
        function-tools-tests.cpp
        generate-tests.cpp
        join-tests.cpp
        map-tests.cpp
        random-tests.cpp
        range-tests.cpp
        repeat-tests.cpp
        string-splitter-tests.cpp
        take-every-tests.cpp
        take-tests.cpp
        test-main.cpp
        unique-tests.cpp
        zip-tests.cpp)

target_compile_features(LazyTests PRIVATE cxx_std_11)

target_compile_options(LazyTests
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wpedantic -Wextra -Wall -Wno-unused-function>)

target_include_directories(LazyTests
        SYSTEM # Prevent warnings from Catch2
        PRIVATE
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/catch>")

target_link_libraries(LazyTests PRIVATE cpp-lazy::cpp-lazy)

enable_testing()

add_test(NAME LazyTests COMMAND LazyTests)
