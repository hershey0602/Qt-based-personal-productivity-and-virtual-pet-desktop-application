cmake_minimum_required(VERSION 3.21)
project(FocusGarden VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Widgets Svg Multimedia Sql)
include(CTest)
set(FG_SRC
 src/AppComposition.h src/AppComposition.cpp src/MainWindow.h src/MainWindow.cpp
 src/contracts/DomainTypes.h src/contracts/ServicePorts.h
 src/services/AppServices.h src/services/AppServices.cpp
 src/services/CompletionCoordinator.h src/services/CompletionCoordinator.cpp
 src/services/Repository.h src/services/Repository.cpp
 src/services/TaskManager.h src/services/TaskManager.cpp
 src/services/StatisticsService.h src/services/StatisticsService.cpp
 src/services/FocusEngine.h src/services/FocusEngine.cpp
 src/services/RewardService.h src/services/RewardService.cpp
 src/services/PetManager.h src/services/PetManager.cpp
 src/services/SettingsService.h src/services/SettingsService.cpp
 src/services/MusicController.h src/services/MusicController.cpp
 src/services/CompletionSound.h src/services/CompletionSound.cpp
 src/storage/Migrations.h src/storage/Migrations.cpp
 src/mocks/MockPorts.h src/mocks/MockPorts.cpp
 src/ui/Components.h src/ui/Components.cpp
 src/ui/ConfirmDialog.h src/ui/ConfirmDialog.cpp
 src/ui/ResultDialog.h src/ui/ResultDialog.cpp
 src/ui/TaskEditDialog.h src/ui/TaskEditDialog.cpp
 src/pages/DashboardPage.h src/pages/DashboardPage.cpp
 src/pages/TaskPage.h src/pages/TaskPage.cpp
 src/pages/FocusPage.h src/pages/FocusPage.cpp
 src/pages/PetPage.h src/pages/PetPage.cpp
 src/pages/StatisticsPage.h src/pages/StatisticsPage.cpp
 src/pages/MusicPage.h src/pages/MusicPage.cpp
 src/pages/SettingsPage.h src/pages/SettingsPage.cpp
)
set(FG_DESIGN_DIR "" CACHE PATH "Shared FocusGarden design directory")
if(NOT FG_DESIGN_DIR)
 foreach(candidate "${CMAKE_CURRENT_LIST_DIR}/../FocusGarden" "${CMAKE_CURRENT_LIST_DIR}/../../../FocusGarden")
  if(EXISTS "${candidate}/resources.qrc")
   get_filename_component(FG_DESIGN_DIR "${candidate}" ABSOLUTE)
   break()
  endif()
 endforeach()
endif()
if(NOT EXISTS "${FG_DESIGN_DIR}/resources.qrc")
 message(FATAL_ERROR "FocusGarden/resources.qrc not found; set FG_DESIGN_DIR to the shared FocusGarden directory")
endif()
set(FG_DESIGN "${FG_DESIGN_DIR}")
set(FG_LAYOUT_QRC "${CMAKE_CURRENT_BINARY_DIR}/layouts.generated.qrc")
file(WRITE "${FG_LAYOUT_QRC}" "<RCC><qresource prefix=\"/layouts\">")
foreach(page dashboard tasks focus pet statistics music settings)
 file(APPEND "${FG_LAYOUT_QRC}" "<file alias=\"${page}.json\">${FG_DESIGN}/Qt_UI_Implementation_Blueprint/layouts/${page}.json</file>")
endforeach()
file(APPEND "${FG_LAYOUT_QRC}" "</qresource></RCC>")
add_library(fg_ui SHARED ${FG_SRC} "${FG_DESIGN}/resources.qrc" "${FG_LAYOUT_QRC}")
set_target_properties(fg_ui PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_include_directories(fg_ui PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")
target_link_libraries(fg_ui PUBLIC Qt6::Core Qt6::Widgets Qt6::Svg Qt6::Multimedia Qt6::Sql)
add_executable(FocusGarden WIN32 src/main.cpp)
target_link_libraries(FocusGarden PRIVATE fg_ui)
if(BUILD_TESTING)
 find_package(Qt6 REQUIRED COMPONENTS Test)
 add_executable(fg_tests tests/test_shell.cpp)
 target_link_libraries(fg_tests PRIVATE fg_ui Qt6::Test)
 add_test(NAME shell COMMAND fg_tests -platform offscreen)
 foreach(role b c d integration)
  add_subdirectory(tests/${role})
 endforeach()
endif()
