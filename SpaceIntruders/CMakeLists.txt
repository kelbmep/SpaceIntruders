# CMakeList.txt: проект CMake для SpaceIntruders; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (SpaceIntruders "main.cpp")

set_target_properties(SpaceIntruders PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory("ImaginaryEngine")

target_link_libraries(SpaceIntruders ImaginaryEngine)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
