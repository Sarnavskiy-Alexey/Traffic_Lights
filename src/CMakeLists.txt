# Требуемая минимальная версия CMake
cmake_minimum_required(VERSION 3.30)

# Название и версия основного проекта
project("traffic_lights" VERSION 1.0)

# Создаем переменную, хранящую пути к файлам
file(
    GLOB TARGET_SRC
    main.cpp
)

# Создаем переменную, хранящую пути к заголовочным файлам библиотеки
file(
    GLOB HEADER_LIB
    ../inc/*.hpp
)

# Создаем переменную, хранящую пути к файлам реализации библиотеки
file(
    GLOB SOURCE_LIB
    ../inc/*((?!main))*.cpp
)

# Создаем библиотеку для последующего использования (например, в тестах)
add_library(MyTrafficLightsLib ${HEADER_LIB} ${SOURCE_LIB})

# Добавляем в проект задачу собрать исполняемый файл
add_executable(
    traffic_lights
    ${TARGET_SRC}
)

# Добавляем библиотеку MyTrafficLightsLib в проект
target_link_libraries(
    traffic_lights
    MyTrafficLightsLib
)
