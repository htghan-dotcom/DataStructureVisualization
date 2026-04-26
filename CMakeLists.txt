cmake_minimum_required(VERSION 3.16)
project(DataStructureVisualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────
#  SFML – try SFML 3.x first (MSYS2 ucrt64 ships 3.x)
# ─────────────────────────────────────────────
if(EXISTS "C:/SFML/SFML-3.0.2/lib/cmake/SFML")
    set(SFML_DIR "C:/SFML/SFML-3.0.2/lib/cmake/SFML" CACHE PATH "SFML cmake dir")
elseif(EXISTS "C:/msys64/ucrt64/lib/cmake/SFML")
    set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML" CACHE PATH "SFML cmake dir")
elseif(EXISTS "C:/SFML/lib/cmake/SFML")
    set(SFML_DIR "C:/SFML/lib/cmake/SFML" CACHE PATH "SFML cmake dir")
endif()

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)
# ─────────────────────────────────────────────
#  Source files
# ─────────────────────────────────────────────
file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(REMOVE_ITEM COMMON_SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

if(COMMON_SOURCES STREQUAL "")
    message(FATAL_ERROR "No source files found under src/ (excluding main files)")
endif()

# vendor (tinyfiledialogs is plain C)
set(VENDOR_SOURCES "${CMAKE_SOURCE_DIR}/vendor/tinyfiledialogs.c")

# ─────────────────────────────────────────────
#  Helper macro: create one executable
# ─────────────────────────────────────────────
macro(add_ds_executable TARGET_NAME MAIN_FILE)
    add_executable(${TARGET_NAME}
        ${COMMON_SOURCES}
        ${VENDOR_SOURCES}
        ${MAIN_FILE}
    )

    target_include_directories(${TARGET_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/vendor"
    )

    target_link_libraries(${TARGET_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
    )

    if(WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE
            winmm gdi32 ole32 oleaut32 uuid advapi32
        )
        # Hide console window in Release
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(${TARGET_NAME} PROPERTIES
                WIN32_EXECUTABLE TRUE
            )
        endif()
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

    # ── Copy assets next to the .exe after every build ──
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets"
        COMMENT "Copying assets to build dir for ${TARGET_NAME}"
    )

    # ── On Windows: copy SFML DLLs so the .exe can find them ──
    if(WIN32)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${TARGET_NAME}>
                $<TARGET_FILE_DIR:${TARGET_NAME}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying SFML DLLs for ${TARGET_NAME}"
        )
    endif()
endmacro()

# ─────────────────────────────────────────────
#  Executables
# ────────────────────────────cmake -S . -B build─────────────────
add_ds_executable(DataStructureVisualization "src/main.cpp")