<<<<<<< HEAD
cmake_minimum_required(VERSION 3.21)
project(DataStructureVisualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0.2 COMPONENTS Graphics Window System REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/RedBlackTree
    ${PROJECT_SOURCE_DIR}/include/DoublyLinkedList
    ${PROJECT_SOURCE_DIR}/vendor
)

# Source files
set(SOURCES
    src/main.cpp
    src/WelcomeScreen.cpp
    src/AppLayout.cpp
    src/Menu.cpp
    vendor/tinyfiledialogs.c
    src/DoublyLinkedList/DoublyLinkedList.cpp
    src/DoublyLinkedList/DLLVisualizer.cpp
    src/RedBlackTree/RedBlackTree.cpp
    src/RedBlackTree/RedBlackNode.cpp
    src/RedBlackTree/RBTVisualizer.cpp
)

# Header files (optional, for IDE support)
set(HEADERS
    include/Common.h
    include/AppLayout.h
    include/GUI.h
    include/Menu.h
    include/WelcomeScreen.h
    vendor/tinyfiledialogs.h
    include/DoublyLinkedList/DoublyLinkedList.h
    include/DoublyLinkedList/DLLStepState.h
    include/DoublyLinkedList/DLLVisualizer.h
    include/RedBlackTree/RedBlackTree.h
    include/RedBlackTree/RedBlackNode.h
    include/RedBlackTree/RBTVisualizer.h
)

# Create executable
add_executable(DataStructureVisualization ${SOURCES} ${HEADERS})

# Link SFML
target_link_libraries(DataStructureVisualization PRIVATE SFML::Graphics SFML::Window SFML::System)

# Copy assets to build directory
add_custom_command(TARGET DataStructureVisualization POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:DataStructureVisualization>/assets"
    COMMENT "Copying assets to build directory"
)