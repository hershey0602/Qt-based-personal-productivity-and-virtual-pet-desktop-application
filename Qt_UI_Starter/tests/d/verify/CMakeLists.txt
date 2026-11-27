cmake_minimum_required(VERSION 3.21)
project(FocusGardenDServiceVerify LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Widgets Svg Multimedia Test)

set(FG_STARTER_DIR "${CMAKE_CURRENT_LIST_DIR}/../../..")
set(FG_REPO_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../..")

add_executable(d_service_verify
    verify_services.cpp
    "${FG_STARTER_DIR}/src/contracts/ServicePorts.h"
    "${FG_STARTER_DIR}/src/services/SettingsService.cpp"
    "${FG_STARTER_DIR}/src/services/MusicController.cpp"
    "${FG_STARTER_DIR}/src/services/CompletionSound.cpp"
    "${FG_REPO_DIR}/FocusGarden/resources.qrc"
)
target_include_directories(d_service_verify PRIVATE "${FG_STARTER_DIR}/src")
target_compile_definitions(d_service_verify PRIVATE
    FG_AUDIO_PATH="${FG_REPO_DIR}/FocusGarden/UI_ASSETS/audio/sfx_focus_complete.wav"
)
target_link_libraries(d_service_verify PRIVATE
    Qt6::Core Qt6::Widgets Qt6::Svg Qt6::Multimedia Qt6::Test
)

enable_testing()
add_test(NAME d_service_verify COMMAND d_service_verify)
