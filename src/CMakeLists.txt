target_include_directories(${PROJECT_NAME}.Core
    PUBLIC
        node
        util
)

add_subdirectory(node)
add_subdirectory(util)