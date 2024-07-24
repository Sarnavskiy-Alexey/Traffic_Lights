# Требуемая минимальная версия CMake
cmake_minimum_required(VERSION 3.30)

# Название и версия тестового проекта
project("traffic_lights_test" VERSION 1.0)

# Добавляем сторонние библиотеки
include(FetchContent)

# Отключение модульных тестов для библиотеки json
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Скачивание nlohmann json из GitHub репозитория
FetchContent_Declare(
    json
    URL
    https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

# Скачиваем gtest из GitHub репозитория (если еще не скачан)
FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# Добавляем директорию gtest
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Параметр управления многопоточностью
set(gtest_disable_pthreads on)

# Предотвращение переопределения настроек компилятора/компоновщика родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Открытие доступности скаченных из GitHub репозитория данных о json и google-test
FetchContent_MakeAvailable(json googletest)

enable_testing()

file(
    GLOB TARGET_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(
    traffic_lights_test
    ${TARGET_TEST}
)

target_link_libraries(traffic_lights_test PRIVATE
    gtest_main
    nlohmann_json::nlohmann_json
    MyTrafficLightsLib
)

include(googletest)