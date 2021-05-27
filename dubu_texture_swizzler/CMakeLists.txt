set(target_name "dubu_texture_swizzler")

set(src_precompiled
    "src/precompiled.hpp")

set(src_dubu_texture_swizzler
    "src/dubu_texture_swizzler/stb_image.h"
    "src/dubu_texture_swizzler/DubuTextureSwizzler.hpp"
    "src/dubu_texture_swizzler/DubuTextureSwizzler.cpp"
    "src/main.cpp")

set(src_files
    ${src_precompiled}
    ${src_dubu_texture_swizzler})

# Project
add_executable(${target_name} ${src_files})

target_link_libraries(${target_name}
    compiler_features
    compiler_warnings
    dubu_opengl_app)

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(${target_name} PUBLIC ${src_precompiled})

source_group("src" FILES ${src_dubu_texture_swizzler})
source_group("src/precompiled" FILES ${src_precompiled})
source_group("src/precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
