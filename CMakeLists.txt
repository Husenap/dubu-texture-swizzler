cmake_minimum_required(VERSION 3.16)

project("dubu_texture_swizzler" C CXX)

include("cmake/prevent_in_source_builds.cmake")
include("cmake/standard_project_setup.cmake")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")

set(${PROJECT_NAME}_FOLDER
    ${PROJECT_NAME}
    CACHE STRING "The folder that ${PROJECT_NAME} projects will be placed in.")

include("thirdparty/dubu_opengl_app.cmake")

add_subdirectory("dubu_texture_swizzler")