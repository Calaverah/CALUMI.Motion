target_sources(${PROJECT_NAME}.Core
    PUBLIC
        EntryTypes.h
        TagDictionary.h
    PRIVATE
        EntryTypes.cpp
        TagDictionary.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(TAG_JSON_SRC
    "${PROJECT_SOURCE_DIR}/tags/general.json"
    "${PROJECT_SOURCE_DIR}/tags/sf1.json"
    "${PROJECT_SOURCE_DIR}/tags/sen.json"
)

set(GEN_HEADER_REL "generated/g_tags.h")
set(GEN_SOURCE_REL "generated/g_tags.cpp")
set(GEN_HEADER "${PROJECT_SOURCE_DIR}/src/util/${GEN_HEADER_REL}")
set(GEN_SOURCE "${PROJECT_SOURCE_DIR}/src/util/${GEN_SOURCE_REL}")

execute_process(
        COMMAND ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/tools/tags_gen/generate_tags.py" "${GEN_HEADER}" "${GEN_SOURCE}" ${TAG_JSON_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE PYTHON_RESULT
        OUTPUT_VARIABLE PYTHON_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT PYTHON_RESULT EQUAL 0)
    message(FATAL_ERROR "Python script failed with code: ${PYTHON_OUTPUT}")
endif()

message(STATUS "Python output: ${PYTHON_OUTPUT}")

if(EXISTS "${GEN_HEADER}")
    target_sources(${PROJECT_NAME}.Core PRIVATE "${GEN_HEADER_REL}")
endif()

if(EXISTS "${GEN_SOURCE}")
    target_sources(${PROJECT_NAME}.Core PRIVATE "${GEN_SOURCE_REL}")
endif()
