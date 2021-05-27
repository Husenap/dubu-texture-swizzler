message("-- External Project: dubu_opengl_app")
include(FetchContent)

FetchContent_Declare(
    dubu_opengl_app
    GIT_REPOSITORY  https://github.com/Husenap/dubu-opengl-app.git
    GIT_TAG         v1.2
)

set(dubu_opengl_app_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(dubu_opengl_app_FOLDER "thirdparty/dubu_opengl_app" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(dubu_opengl_app)