cmake_minimum_required(VERSION 3.10)
project(as_bundler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

# AngelScript paths
set(ANGELSCRIPT_DIR "${CMAKE_SOURCE_DIR}/include/angelscript")
set(ADDONS_DIR "${CMAKE_SOURCE_DIR}/include/add_on")

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${ANGELSCRIPT_DIR}/include
    ${ADDONS_DIR}
)

# Collect all AngelScript source files
file(GLOB ANGELSCRIPT_SOURCES
    ${ANGELSCRIPT_DIR}/source/as_atomic.cpp
    ${ANGELSCRIPT_DIR}/source/as_builder.cpp
    ${ANGELSCRIPT_DIR}/source/as_bytecode.cpp
    ${ANGELSCRIPT_DIR}/source/as_compiler.cpp
    ${ANGELSCRIPT_DIR}/source/as_configgroup.cpp
    ${ANGELSCRIPT_DIR}/source/as_context.cpp
    ${ANGELSCRIPT_DIR}/source/as_datatype.cpp
    ${ANGELSCRIPT_DIR}/source/as_gc.cpp
    ${ANGELSCRIPT_DIR}/source/as_generic.cpp
    ${ANGELSCRIPT_DIR}/source/as_globalproperty.cpp
    ${ANGELSCRIPT_DIR}/source/as_memory.cpp
    ${ANGELSCRIPT_DIR}/source/as_module.cpp
    ${ANGELSCRIPT_DIR}/source/as_objecttype.cpp
    ${ANGELSCRIPT_DIR}/source/as_outputbuffer.cpp
    ${ANGELSCRIPT_DIR}/source/as_parser.cpp
    ${ANGELSCRIPT_DIR}/source/as_restore.cpp
    ${ANGELSCRIPT_DIR}/source/as_scriptcode.cpp
    ${ANGELSCRIPT_DIR}/source/as_scriptengine.cpp
    ${ANGELSCRIPT_DIR}/source/as_scriptfunction.cpp
    ${ANGELSCRIPT_DIR}/source/as_scriptnode.cpp
    ${ANGELSCRIPT_DIR}/source/as_scriptobject.cpp
    ${ANGELSCRIPT_DIR}/source/as_string.cpp
    ${ANGELSCRIPT_DIR}/source/as_string_util.cpp
    ${ANGELSCRIPT_DIR}/source/as_thread.cpp
    ${ANGELSCRIPT_DIR}/source/as_tokenizer.cpp
    ${ANGELSCRIPT_DIR}/source/as_typeinfo.cpp
    ${ANGELSCRIPT_DIR}/source/as_variablescope.cpp
)

# Add calling convention files - as_callfunc.cpp is always needed for common functions
list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc.cpp)

# Add platform-specific calling convention files
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    if(MSVC)
        list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_x64_msvc.cpp)
        enable_language(ASM_MASM)
        list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_x64_msvc_asm.asm)
    elseif(MINGW)
        list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_x64_mingw.cpp)
    else()
        list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_x64_gcc.cpp)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686|i386|x86")
    list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_x86.cpp)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_arm64.cpp)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    list(APPEND ANGELSCRIPT_SOURCES ${ANGELSCRIPT_DIR}/source/as_callfunc_arm.cpp)
endif()

# AngelScript library
add_library(angelscript STATIC ${ANGELSCRIPT_SOURCES})

# Add-ons
add_library(angelscript_addons STATIC
    ${ADDONS_DIR}/scriptstdstring/scriptstdstring.cpp
    ${ADDONS_DIR}/scriptstdstring/scriptstdstring_utils.cpp
    ${ADDONS_DIR}/scriptarray/scriptarray.cpp
    ${ADDONS_DIR}/scriptdictionary/scriptdictionary.cpp
    ${ADDONS_DIR}/scriptmath/scriptmath.cpp
    ${ADDONS_DIR}/scriptmath/scriptmathcomplex.cpp
    ${ADDONS_DIR}/scriptany/scriptany.cpp
    ${ADDONS_DIR}/scriptgrid/scriptgrid.cpp
    ${ADDONS_DIR}/scripthelper/scripthelper.cpp
)

# C++ wrapper and API registration
add_library(as_wrapper
    src/as_wrapper.cpp
    src/api_registration.cpp
)

target_link_libraries(as_wrapper angelscript angelscript_addons)

# Main bundler executable
add_executable(as_bundler
    src/main.c
    src/common/common.c
    src/fs/fs.c
    src/processing/processing.c
    src/preprocessor/preprocessor.c
    src/obfuscator/obfuscator.c
    src/errors/errors.c
)
target_link_libraries(as_bundler as_wrapper)

# Cross-compile for Windows
if(MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()
