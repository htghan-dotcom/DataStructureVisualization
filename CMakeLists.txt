cmake_minimum_required(VERSION 3.10)

project(DataStructureVisualization)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SFML Configuration - Multiple options for different installations
# Option 1: MSYS2 ucrt64 (default)
if(EXISTS "C:/msys64/ucrt64/lib/cmake/SFML")
    set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML" CACHE PATH "SFML directory")
# Option 2: Standard SFML installation
elseif(EXISTS "C:/SFML/lib/cmake/SFML")
    set(SFML_DIR "C:/SFML/lib/cmake/SFML" CACHE PATH "SFML directory")
# Option 3: Custom path (set by user)
else()
    # Uncomment and modify the line below if SFML is installed elsewhere
    # set(SFML_DIR "C:/path/to/your/sfml/lib/cmake/SFML" CACHE PATH "SFML directory")
endif()

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Collect all source files except main files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp" "${CMAKE_SOURCE_DIR}/src/main_hash.cpp")

if(SOURCES STREQUAL "")
	message(FATAL_ERROR "No source files found under src/ (excluding main files)")
endif()

# Add main executable (original visualizer)
add_executable(DataStructureVisualization ${SOURCES} src/main.cpp)

# Add hash visualizer executable
add_executable(HashVisualizer ${SOURCES} src/main_hash.cpp)

# Common target settings
foreach(TARGET DataStructureVisualization HashVisualizer)
	target_include_directories(${TARGET} PRIVATE
		"${CMAKE_SOURCE_DIR}/src"
		"${CMAKE_SOURCE_DIR}/include"
	)

	target_link_libraries(${TARGET} PRIVATE
		sfml-graphics
		sfml-window
		sfml-system
	)

	if(WIN32)
		target_link_libraries(${TARGET} PRIVATE
			winmm
			gdi32
			ole32
			oleaut32
			uuid
			advapi32
		)
	endif()

	set_target_properties(${TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endforeach()
