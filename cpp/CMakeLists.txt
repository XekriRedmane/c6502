cmake_minimum_required(VERSION 3.20)
# C for XZ Embedded's decoder in third_party/xz-embedded.
project(c6502 LANGUAGES C CXX)

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
  # the IR's integer (src/asdl/runtime.h), which is intended. /MP compiles
  # a target's files in parallel. MSBuild's /m (`cmake --build --parallel`)
  # only builds separate targets at once, and c6502_tests waits for the
  # c6502 library, so without /MP every file compiled one at a time.
  add_compile_options(/W4 /permissive- /utf-8 /EHsc /bigobj /Zc:__cplusplus
                      /wd4324 /MP)
else()
  # A designated initializer that leaves fields out, as in
  # `Function{.name = "f"}`, gives them their defaults, as the Python
  # dataclasses' keyword arguments do, so GCC's warning about it is off.
  add_compile_options(-Wall -Wextra -Wno-missing-field-initializers)
endif()

get_filename_component(C6502_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# asdlgen writes the IR classes in generated/ from the .asdl files at
# the root (tools/asdlgen.h). The build runs it when an .asdl file or
# the tool changes, and the files stay checked in, so that a change's
# generated diff is reviewed with it. The check_asdl test compares them.
add_library(asdlgen_lib STATIC "${CMAKE_CURRENT_SOURCE_DIR}/tools/asdlgen.cpp")
target_include_directories(asdlgen_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/tools")
add_executable(asdlgen "${CMAKE_CURRENT_SOURCE_DIR}/tools/asdlgen_main.cpp")
target_link_libraries(asdlgen PRIVATE asdlgen_lib)
set(C6502_ASDL_FILES "${C6502_ROOT}/c99.asdl" "${C6502_ROOT}/tac.asdl"
    "${C6502_ROOT}/asm.asdl" "${C6502_ROOT}/asm2.asdl")
set(C6502_ASDL_OUTPUTS)
foreach(stem c99 tac asm asm2)
  list(APPEND C6502_ASDL_OUTPUTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/${stem}_ast.h"
       "${CMAKE_CURRENT_SOURCE_DIR}/generated/${stem}_ast.cpp")
endforeach()
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/asdlgen.stamp"
  BYPRODUCTS ${C6502_ASDL_OUTPUTS}
  COMMAND asdlgen "${CMAKE_CURRENT_SOURCE_DIR}/generated" ${C6502_ASDL_FILES}
  COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/asdlgen.stamp"
  DEPENDS asdlgen ${C6502_ASDL_FILES}
  COMMENT "Writing the IR classes from the .asdl files")
add_custom_target(asdl_generated DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/asdlgen.stamp")

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
     "${CMAKE_CURRENT_SOURCE_DIR}/src/rtlib/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/sim/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.cpp")
list(REMOVE_ITEM C6502_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_library(c6502 STATIC ${C6502_SOURCES})
add_dependencies(c6502 asdl_generated)
# `c6502cpp compile` run by itself finds c6502's include/ in the source
# tree it was built from when the executable isn't inside a checkout.
set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/src/compile_command.cpp"
  PROPERTIES COMPILE_DEFINITIONS "C6502_SOURCE_ROOT=\"${CMAKE_CURRENT_SOURCE_DIR}/..\"")
target_include_directories(c6502 PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/generated")

# XZ Embedded (0BSD), the .xz decoder that reads the frozen captures
# (src/unxz.h), vendored as its README describes.
add_library(xz_embedded STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xz-embedded/xz_crc32.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xz-embedded/xz_dec_lzma2.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xz-embedded/xz_dec_stream.c")
target_include_directories(xz_embedded PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xz-embedded")
# Its warnings are its authors' to fix, so they are off.
if(MSVC)
  target_compile_options(xz_embedded PRIVATE /w)
else()
  target_compile_options(xz_embedded PRIVATE -w)
endif()
target_link_libraries(c6502 PRIVATE xz_embedded)

add_executable(c6502cpp src/main.cpp)
target_link_libraries(c6502cpp PRIVATE c6502)
if(WIN32)
  # CommandLineToArgvW, for the command line as UTF-8.
  target_link_libraries(c6502cpp PRIVATE shell32)
endif()

file(GLOB C6502_TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(c6502_tests ${C6502_TEST_SOURCES})
target_link_libraries(c6502_tests PRIVATE c6502 asdlgen_lib)
target_include_directories(c6502_tests PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/doctest")
# The checkout's root, where the tests read include/, docs/, golden/,
# the .asdl files, and the files under tests/ and cpp/tests/.
target_compile_definitions(c6502_tests PRIVATE C6502_SOURCE_ROOT="${C6502_ROOT}")

enable_testing()
add_test(NAME c6502_tests COMMAND c6502_tests)
# The checks after Python (README.md): the golden files, whose last line
# counts the jobs that differ, and the frozen captures.
add_test(NAME check_golden COMMAND c6502cpp check-golden --root "${C6502_ROOT}" -j 12
         WORKING_DIRECTORY "${C6502_ROOT}")
set_tests_properties(check_golden PROPERTIES FAIL_REGULAR_EXPRESSION " [1-9][0-9]* differ")
add_test(NAME check_frozen COMMAND c6502cpp check-frozen "${C6502_ROOT}/cpp/tests/frozen"
         WORKING_DIRECTORY "${C6502_ROOT}")
set_tests_properties(check_frozen PROPERTIES FAIL_REGULAR_EXPRESSION "MISMATCH|DIFFER")
add_test(NAME check_asdl
         COMMAND asdlgen --check "${CMAKE_CURRENT_SOURCE_DIR}/generated" ${C6502_ASDL_FILES})
