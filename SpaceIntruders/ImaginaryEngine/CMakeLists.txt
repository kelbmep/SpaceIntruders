# CMakeList.txt: проект CMake для SpaceIntruders; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_library(ImaginaryEngine STATIC src/engine.cpp src/engine.hpp src/window.hpp 
			src/eventManager.hpp src/eventManager.cpp src/render.hpp src/render.cpp 
			src/GL/GLRender.hpp src/GL/GLRender.cpp src/SDL/SDLWindow.hpp 
			src/SDL/SDLWindow.cpp src/SDL/SDLRender.hpp src/SDL/SDLRender.cpp 
			src/sprite.hpp src/sprite.cpp src/bitmap.hpp src/bitmap.cpp src/node.cpp 
			src/node.hpp src/shaderProgram.hpp src/meshData.hpp src/texture.cpp 
			src/texture.hpp src/vertexBuffer.cpp src/vertexBuffer.hpp src/GL/GLProgram.hpp  
			src/GL/GLProgram.cpp src/GL/GLSpriteProgram.hpp src/GL/GLSpriteProgram.cpp 
			src/GL/GLTexture.cpp src/GL/GLTexture.hpp src/GL/GLVertexBuffer.cpp 
			src/GL/GLVertexBuffer.hpp src/sound.hpp src/sound.cpp src/audioManager.cpp 
			src/audioManager.hpp src/button.hpp src/button.cpp src/UIManager.hpp 
			src/UIManager.cpp src/particleBuffer.cpp src/particleBuffer.hpp src/playlist.hpp 
			src/playlist.cpp src/GL/GLParticleBuffer.cpp src/GL/GLParticleProgram.hpp 
			src/GL/GLParticleProgram.cpp src/particleEmitter.cpp src/particleEmitter.hpp 
			src/utilities/keycodes.hpp src/vertexData.hpp src/scheduleManager.hpp 
			src/scheduleManager.cpp src/actionManager.hpp src/actionManager.cpp 
			src/physicalSprite.cpp src/physicalSprite.hpp src/vertexDataHelper.hpp  
			src/frameBuffer.hpp src/frameBuffer.cpp src/GL/GLFrameBuffer.hpp 
			src/GL/GLFrameBuffer.cpp src/GL/GLHeaders.hpp src/utilities/general_functions.hpp)

set_target_properties(ImaginaryEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(NOT IOS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
	set(SDL2MAIN_LIBRARY SDL2main)
	set(SDL2_LIBRARY SDL2-static)
endif()

if(ANDROID)
	set(OPENGL_LIBRARY GLESv2)
else()
	find_library(OPENGL_LIBRARY OpenGL)
endif()

if(NOT APPLE AND NOT ANDROID)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glew)
	set(GLEW_LIBRARY libglew_static)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
set(IMGUI_LIBRARY imgui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/box2d)
set(BOX2D_LIBRARY box2d)

target_link_libraries(ImaginaryEngine PUBLIC
		${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${IMGUI_LIBRARY} ${BOX2D_LIBRARY})

target_include_directories(ImaginaryEngine 
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/src 
			${CMAKE_CURRENT_SOURCE_DIR}/libs
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include
			${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include
			${CMAKE_CURRENT_SOURCE_DIR}/libs/box2d/include
			${CMAKE_CURRENT_SOURCE_DIR}/libs/stbi)

set(SYSTEM_LIBS -mwindows ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
