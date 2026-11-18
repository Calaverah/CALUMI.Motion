target_sources(${PROJECT_NAME}.Core
    PUBLIC
        EntryTypes.h
        TagDictionary.h
        DropdownCollection.h
    PRIVATE
        EntryTypes.cpp
        TagDictionary.cpp
        DropdownCollection.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(TAG_JSON_SRC
    "${PROJECT_SOURCE_DIR}/lists/tags/general.json"
    "${PROJECT_SOURCE_DIR}/lists/tags/sf1.json"
    "${PROJECT_SOURCE_DIR}/lists/tags/sen.json"
)

set(DROP_JSON_SRC
    "${PROJECT_SOURCE_DIR}/lists/unique_dropdowns/sf1.json"
    "${PROJECT_SOURCE_DIR}/lists/unique_dropdowns/general.json"
)

set(GEN_TAGS_HDR_REL "generated/g_tags.h")
set(GEN_DROP_HDR_REL "generated/g_dropdown.h")
set(GEN_TAGS_SRC_REL "generated/g_tags.cpp")
set(GEN_DROP_SRC_REL "generated/g_dropdown.inc")
set(GEN_TAGS_HEADER "${PROJECT_SOURCE_DIR}/src/util/${GEN_TAGS_HDR_REL}")
set(GEN_DROP_HEADER "${PROJECT_SOURCE_DIR}/src/util/${GEN_DROP_HDR_REL}")
set(GEN_TAGS_SOURCE "${PROJECT_SOURCE_DIR}/src/util/${GEN_TAGS_SRC_REL}")
set(GEN_DROP_SOURCE "${PROJECT_SOURCE_DIR}/src/util/${GEN_DROP_SRC_REL}")

execute_process(
        COMMAND ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/tools/tags_gen/generate_tags.py" "${GEN_TAGS_HEADER}" "${GEN_TAGS_SOURCE}" ${TAG_JSON_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE PYTHON_RESULT
        OUTPUT_VARIABLE PYTHON_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT PYTHON_RESULT EQUAL 0)
    message(FATAL_ERROR "Python script failed with code: ${PYTHON_OUTPUT}")
endif()

message(STATUS "Python output: ${PYTHON_OUTPUT}")

execute_process(
        COMMAND ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/tools/dropdown_gen/generate_uniques.py" "${GEN_DROP_HEADER}" "${GEN_DROP_SOURCE}" ${DROP_JSON_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE PYTHON_RESULT
        OUTPUT_VARIABLE PYTHON_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT PYTHON_RESULT EQUAL 0)
    message(FATAL_ERROR "Python script failed with code: ${PYTHON_OUTPUT}")
endif()

message(STATUS "Python output: ${PYTHON_OUTPUT}")

if(EXISTS "${GEN_TAGS_HEADER}")
    target_sources(${PROJECT_NAME}.Core PRIVATE "${GEN_TAGS_HDR_REL}")
endif()

if(EXISTS "${GEN_TAGS_SOURCE}")
    target_sources(${PROJECT_NAME}.Core PRIVATE "${GEN_TAGS_SRC_REL}")
endif()

if(EXISTS "${GEN_DROP_HEADER}")
    target_sources(${PROJECT_NAME}.Core PRIVATE "${GEN_DROP_HDR_REL}")
endif()
