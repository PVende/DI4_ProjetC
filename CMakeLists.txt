cmake_minimum_required(VERSION 3.8)
project(DI4_ProjetC)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        .gitignore
        Batch.c
        Batch.h
        Batches.c
        Batches.h
        Config.c
        Config.h
        helpers.c
        helpers.h
        Instance.c
        Instance.h
        main.c
        README.md
        Sequence.c
        Sequence.h
        tabu_python.py)

add_executable(DI4_ProjetC ${SOURCE_FILES})