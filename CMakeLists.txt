cmake_minimum_required(VERSION 3.10)

# Force MSYS2 ucrt64 compiler before project definition.
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe" CACHE PATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe" CACHE PATH "C++ compiler" FORCE)

project(DataStructureVisualization)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SFML package path for MSYS2 ucrt64.
set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Collect project sources.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

if(SOURCES STREQUAL "")
	message(FATAL_ERROR "No source files found under src/")
endif()

add_executable(DataStructureVisualization ${SOURCES})

target_include_directories(DataStructureVisualization PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(DataStructureVisualization PRIVATE
	sfml-graphics
	sfml-window
	sfml-system
)

if(WIN32)
	target_link_libraries(DataStructureVisualization PRIVATE
		winmm
		gdi32
		ole32
		oleaut32
		uuid
		advapi32
	)
endif()

set_target_properties(DataStructureVisualization PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
