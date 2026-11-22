cmake_minimum_required(VERSION 3.30.5)
project(QtEnvCheck VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_VERSION VERSION_EQUAL "3.30.5")
    message(FATAL_ERROR "QtEnvCheck requires CMake 3.30.5; actual: ${CMAKE_VERSION}")
endif()
if(NOT WIN32 OR NOT MINGW OR NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   OR NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "13.1.0"
   OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Requires Windows x64 and MinGW GCC 13.1.0 x64. Check the selected Kit.")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.11.2 EXACT REQUIRED COMPONENTS Widgets Svg Multimedia Sql Test)
foreach(module Core Gui Widgets Svg Multimedia Sql Test)
    if(NOT Qt6${module}_VERSION VERSION_EQUAL "6.11.2")
        message(FATAL_ERROR "Qt6${module} must be 6.11.2; actual: ${Qt6${module}_VERSION}")
    endif()
endforeach()

add_library(envchecks STATIC src/checks.cpp src/checks.h resources.qrc)
target_include_directories(envchecks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(envchecks PUBLIC Qt6::Widgets Qt6::Svg Qt6::Multimedia Qt6::Sql Qt6::Test)
target_compile_options(envchecks PUBLIC -Wall -Wextra -Wpedantic -finput-charset=UTF-8 -fexec-charset=UTF-8)
add_executable(QtEnvCheck src/main.cpp)
target_link_libraries(QtEnvCheck PRIVATE envchecks)

enable_testing()
add_executable(QtEnvCheckTests tests/tst_envcheck.cpp)
target_link_libraries(QtEnvCheckTests PRIVATE envchecks)
add_test(NAME QtEnvCheck.Unit COMMAND QtEnvCheckTests -o -,txt)
add_test(NAME QtEnvCheck.Window COMMAND QtEnvCheck --self-test)
set_tests_properties(QtEnvCheck.Unit QtEnvCheck.Window PROPERTIES TIMEOUT 60)
# Qt Creator injects the Kit PATH when running the app. CTest also needs Qt DLLs
# and the compiler runtime; derive both from the selected Kit, never a user path.
get_target_property(qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(qt_bin "${qmake_executable}" DIRECTORY)
get_filename_component(compiler_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
set_tests_properties(QtEnvCheck.Unit QtEnvCheck.Window PROPERTIES
    ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${qt_bin};PATH=path_list_prepend:${compiler_bin}")
message(STATUS "Environment locked: Qt ${Qt6_VERSION}, GCC ${CMAKE_CXX_COMPILER_VERSION}, CMake ${CMAKE_VERSION}, C++17, x64")
