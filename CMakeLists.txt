cmake_minimum_required(VERSION 3.26)

project(CALUMI.Motion VERSION 0.2.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Informing Model where to look for syntax highlighting / autocomplete
list(APPEND QML_IMPORT_PATH
        "${QT_QML_OUTPUT_DIRECTORY}"
        "${CMAKE_BINARY_DIR}/.qt"
)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH "${QML_IMPORT_PATH}" CACHE STRING "QML Import Paths" FORCE)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Quick
        QuickControls2
        Test
)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_library(${PROJECT_NAME}.Core STATIC)

target_include_directories(${PROJECT_NAME}.Core
    PUBLIC
        src
)

target_link_libraries(${PROJECT_NAME}.Core
        PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
)

add_subdirectory(src)
add_subdirectory(ui)
qt_add_qml_module(${PROJECT_NAME}.Core
    URI "CALUMI.Motion.Core"
)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

if(WIN32 AND MSVC)
    target_sources(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CALUMI.Motion.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h
    )

#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            $<TARGET_FILE:CALUMI.Animation>
#            $<TARGET_FILE_DIR:CALUMI.Motion>
#            COMMENT "Getting Animation DLL..."
#    )
elseif(APPLE)

elseif(LINUX)
    
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_BUILD)
endif()

