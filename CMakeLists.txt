cmake_minimum_required(VERSION 3.16)
project(lab_interdisciplinar C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lab_interdisciplinar
        main.c
        mathlib.c
        mathlib.h
        projetos.h
        cinetica.c
        decaimento.c
        demo.c)

# No Linux/macOS a biblioteca matemática (libm) precisa ser ligada
if(NOT MSVC)
    target_link_libraries(lab_interdisciplinar m)
    target_compile_options(lab_interdisciplinar PRIVATE -Wall -Wextra)
endif()
