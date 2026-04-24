cmake_minimum_required(VERSION 3.10)

project(DataStructureVisualization)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "========== CMAKE DEBUG INFO ==========")
message(STATUS "Compiler C++: ${CMAKE_CXX_COMPILER}")

set(SFML_DIR "D:/study/HK2/SFML-3.0.2-windows-gcc-14.2.0-mingw-64-bit/lib/cmake/SFML" CACHE PATH "Force SFML Dir" FORCE)
list(APPEND CMAKE_PREFIX_PATH "D:/study/HK2/SFML-3.0.2-windows-gcc-14.2.0-mingw-64-bit")
find_package(SFML 3.0 CONFIG COMPONENTS Graphics Window System Main REQUIRED)
message(STATUS "Found SFML Version: ${SFML_VERSION}")

# Collect project sources.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

if(SOURCES STREQUAL "")
	message(FATAL_ERROR "No source files found under src/")
else()
	list(LENGTH SOURCES SOURCE_COUNT)
	message(STATUS "Found ${SOURCE_COUNT} source files.")
endif()

add_executable(DataStructureVisualization WIN32 ${SOURCES})

target_include_directories(DataStructureVisualization PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(DataStructureVisualization PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
	SFML::Main
)

set_target_properties(DataStructureVisualization PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
