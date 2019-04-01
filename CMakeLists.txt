cmake_minimum_required (VERSION 3.0)
project                (ParticleGL)

if (WIN32)
	set(CXX_FLAGS -std=c++14 -g -Wall)
elseif(UNIX)
	set(CXX_FLAGS `sdl2-config --cflags` -std=c++14 -g -Wall -fcolor-diagnostics)
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG " )

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# Modules
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Config
configure_file(config/root.h.conf config/root.h)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
if (WIN32)
	include_directories(${PROJECT_SOURCE_DIR}/include/sdl/windows)
elseif(APPLE)
	include_directories(${PROJECT_SOURCE_DIR}/include/sdl/unix)
endif()

# Code to be compiled
set(IMGUI src/imgui/imgui_impl_sdl.cpp src/imgui/imgui_impl_opengl3.cpp src/imgui/imgui.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_widgets.cpp src/imgui/imgui_internal.h src/imgui/imstb_rectpack.h src/imgui/imstb_textedit.h src/imgui/imstb_truetype.h src/imgui/imconfig.h)
set(SOURCES src/glad.c
	src/consoleMsg/consoleMsg.cpp
	src/controlGUI/controlGUI.cpp
	src/engine/camera/camera.cpp
	src/engine/emitter/emitter.cpp
	src/engine/eventManager/eventManager.cpp
	src/engine/geometries/geometry.cpp
	src/engine/geometries/sphere.cpp
	src/engine/geometries/square.cpp
	src/engine/geometries/triangle.cpp
	src/engine/renderer/renderer.cpp
	src/engine/resource/fileReader.cpp
	src/engine/resource/pathParser.cpp
	src/engine/resource/shaderParser.cpp
	src/engine/resource/stb_image.h
	src/engine/resource/texture.cpp
	src/engine/particleGL.cpp
	src/util/randGen.cpp
	src/main.cpp)

add_executable(${PROJECT_NAME} ${IMGUI} ${SOURCES})

if (WIN32)
	set(SDL_LIB ${PROJECT_SOURCE_DIR}/lib/sdl/windows/SDL2.lib ${PROJECT_SOURCE_DIR}/lib/sdl/windows/SDL2main.lib)
	target_link_libraries(${PROJECT_NAME} ${SDL_LIB})
elseif(APPLE)
	set(SDL2_LIBRARY ${PROJECT_SOURCE_DIR}/lib/sdl/macos/libSDL2.dylib)
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
elseif(UNIX)
	set(OpenGL_GL_PREFERENCE GLVND)
	find_package(SDL2 REQUIRED)
	# linking to libdl is required for GLAD on Linux
	target_link_libraries(${PROJECT_NAME} SDL2 dl)
endif(WIN32)

# find_package(assimp REQUIRED)
# include_directories(${ASSIMP_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARY})

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_custom_target(run
    COMMAND make
    COMMAND ./bin/${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/build
)
