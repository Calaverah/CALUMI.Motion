cmake_minimum_required(VERSION 3.26)

project(CALUMI.Motion VERSION 0.2.0.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(qlementine-icons GIT_REPOSITORY "https://github.com/oclero/qlementine-icons.git")
FetchContent_MakeAvailable(qlementine-icons)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        Gui
        LinguistTools
        Svg
)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

set(PROJECT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMI.Motion.ico
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMI.Motion.rc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMIMotion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMIMotion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMIMotion.qrc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMIMotion.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/CALUMI.Animation)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/3rdPartyDirect)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/AgxNodes)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Application)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Interfaces)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Localization)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Models)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Painter)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Widgets)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        qlementine-icons
        CALUMI.Animation
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(WIN32 AND MSVC)
    target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMI.Motion.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:CALUMI.Animation>
            $<TARGET_FILE_DIR:CALUMI.Motion>
            COMMENT "Getting Animation DLL..."
    )
elseif(APPLE)

elseif(LINUX)
    
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_BUILD)

    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            MACOSX_BUNDLE TRUE
            WIN32_EXECUTABLE TRUE
    )
endif()

