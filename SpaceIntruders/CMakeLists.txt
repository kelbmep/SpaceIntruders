# CMakeList.txt: проект CMake для SpaceIntruders; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

# Добавьте источник в исполняемый файл этого проекта.
set(GAME_SRC main.cpp Game/tank.cpp Game/tank.hpp Game/bullet.cpp Game/bullet.hpp)

if(ANDROID)
    add_library(SpaceIntruders SHARED ${GAME_SRC})
else()
    add_executable(SpaceIntruders ${GAME_SRC})
endif()

add_subdirectory("ImaginaryEngine")

target_link_libraries(SpaceIntruders ImaginaryEngine)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
