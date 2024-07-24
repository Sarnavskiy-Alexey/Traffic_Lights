# Требуемая минимальная версия CMake
cmake_minimum_required(VERSION 3.30)

# Применяемый стандарт C++
set(CMAKE_CXX_STANDARD 20)

# Название корневого проекта (этот проект не собирается)
project(traffic_lights_pr)

# Добавляем директории для всех сборок
include_directories(
    inc
    src
)

# добавляем поддиректорию движка
add_subdirectory(src)

# добавляем поддиректорию тестов
add_subdirectory(tests)
