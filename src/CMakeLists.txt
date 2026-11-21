target_include_directories(${PROJECT_NAME}.Core
    PUBLIC
        node
        util
        graph
        interface
)

add_subdirectory(node)
add_subdirectory(util)
add_subdirectory(graph)
add_subdirectory(interface)