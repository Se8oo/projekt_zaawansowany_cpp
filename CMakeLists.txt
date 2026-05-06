cmake_minimum_required(VERSION 3.15)
project(HomeLibrary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ostrzeżenia kompilatora
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(HomeLibrary
    src/main.cpp
    src/models/Author.cpp
    src/models/Category.cpp
    src/models/Item.cpp
    src/core/Library.cpp
    src/utils/FileStorage.cpp
)

target_include_directories(HomeLibrary PRIVATE src)

# Ustaw katalog roboczy na katalog binarny dla łatwiejszego dostępu do data/
set_target_properties(HomeLibrary PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
