cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

aux_source_directory(./Src VOFA)

target_sources(
        ${CMAKE_PROJECT_NAME} PRIVATE ${VOFA}
)

target_include_directories(
        ${CMAKE_PROJECT_NAME} PRIVATE ./Inc
)