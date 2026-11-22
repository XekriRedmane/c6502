cmake_minimum_required(VERSION 3.20)
project(c6502 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# The executables go in build/bin, whatever the generator, so the
# Python tools find them there (a Debug build of a multi-config
# generator goes in build/bin/debug).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin$<$<CONFIG:Debug>:/debug>")

if(MSVC)
  # Warnings on, not yet errors. /utf-8 reads the sources as UTF-8, and
  # /bigobj takes the generated readers' many functions. C4324 reports
  # each struct padded to the 16-byte alignment of `std::_Signed128`,
  # the IR's integer (src/asdl/runtime.h), which is intended.
  add_compile_options(/W4 /permissive- /utf-8 /EHsc /bigobj /Zc:__cplusplus
                      /wd4324)
else()
  # A designated initializer that leaves fields out, as in
  # `Function{.name = "f"}`, gives them their defaults, as the Python
  # dataclasses' keyword arguments do, so GCC's warning about it is off.
  add_compile_options(-Wall -Wextra -Wno-missing-field-initializers)
endif()

# The compiler's code: the IR runtime, the generated IR classes, and
# the ported passes. A new pass's .cpp under src/passes/ is picked up
# when CMake runs again.
file(GLOB C6502_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/asdl/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/binutils/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/passes/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/passes/optimization/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/passes/optimization_asm/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/passes/optimization_ast/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/sim/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.cpp")
list(REMOVE_ITEM C6502_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_library(c6502 STATIC ${C6502_SOURCES})
target_include_directories(c6502 PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/generated")

add_executable(c6502cpp src/main.cpp)
target_link_libraries(c6502cpp PRIVATE c6502)
if(WIN32)
  # CommandLineToArgvW, for the command line as UTF-8.
  target_link_libraries(c6502cpp PRIVATE shell32)
endif()

file(GLOB C6502_TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(c6502_tests ${C6502_TEST_SOURCES})
target_link_libraries(c6502_tests PRIVATE c6502)
target_include_directories(c6502_tests PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doctest")

enable_testing()
add_test(NAME c6502_tests COMMAND c6502_tests)
