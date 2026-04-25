# Yêu cầu CMake 3.14 trở lên để dùng FetchContent
cmake_minimum_required(VERSION 3.14)

project(DataStructureVisualization)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "========== CMAKE DEBUG INFO ==========")
message(STATUS "Compiler C++: ${CMAKE_CXX_COMPILER}")

include(FetchContent)
message(STATUS "Đang tải và cấu hình SFML tự động từ GitHub...")

# Ép build thư viện SFML ở dạng Static (Nhúng thẳng vào file .exe, không xài DLL)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static build for dependencies" FORCE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2   # Sử dụng phiên bản 3.0.2
)
FetchContent_MakeAvailable(SFML)

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
	"${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(DataStructureVisualization PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
	SFML::Main
	SFML::Audio
)

set_target_properties(DataStructureVisualization PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# ==============================================================================
# TỰ ĐỘNG COPY THƯ MỤC ASSETS SANG THƯ MỤC BUILD ĐỂ APP NHẬN DIỆN ĐƯỢC FONT/ẢNH
# ==============================================================================
add_custom_command(TARGET DataStructureVisualization POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
)

# ==============================================================================
# NHÚNG TĨNH MINGW (Giúp exe tự chạy không cần copy libstdc++-6.dll, libgcc...)
# ==============================================================================
if(MINGW)
    target_link_options(DataStructureVisualization PRIVATE 
        -static-libgcc -static-libstdc++ -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
    )
endif()
