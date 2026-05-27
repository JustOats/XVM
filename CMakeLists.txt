cmake_minimum_required(VERSION 3.16)

project(
    XVM
    VERSION 0.1.0
    DESCRIPTION "Anti-Tamper Software"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(XVM_ENABLE_WARNINGS "Enable compiler warnings" ON)

add_library(xvm_options INTERFACE)
target_compile_features(xvm_options INTERFACE c_std_17)

if(XVM_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(xvm_options INTERFACE /W4)
    else()
        target_compile_options(xvm_options INTERFACE -Wall -Wextra -Wpedantic)
    endif()
endif()

find_package(unofficial-sodium CONFIG QUIET)
if(TARGET unofficial-sodium::sodium)
    set(XVM_SODIUM_TARGET unofficial-sodium::sodium)
elseif(TARGET sodium)
    set(XVM_SODIUM_TARGET sodium)
else()
    find_path(SODIUM_INCLUDE_DIR NAMES sodium.h REQUIRED)
    find_library(SODIUM_LIBRARY NAMES sodium libsodium REQUIRED)

    add_library(xvm_sodium INTERFACE)
    target_include_directories(xvm_sodium INTERFACE "${SODIUM_INCLUDE_DIR}")
    target_link_libraries(xvm_sodium INTERFACE "${SODIUM_LIBRARY}")

    set(XVM_SODIUM_TARGET xvm_sodium)
endif()

find_package(zydis CONFIG QUIET)
if(NOT zydis_FOUND)
    find_package(Zydis CONFIG QUIET)
endif()

if(TARGET Zydis::Zydis)
    set(XVM_ZYDIS_TARGET Zydis::Zydis)
elseif(TARGET Zydis)
    set(XVM_ZYDIS_TARGET Zydis)
elseif(TARGET zydis)
    set(XVM_ZYDIS_TARGET zydis)
else()
    find_path(ZYDIS_INCLUDE_DIR NAMES Zydis/Zydis.h REQUIRED)
    find_library(ZYDIS_LIBRARY NAMES Zydis zydis REQUIRED)
    find_library(ZYCORE_LIBRARY NAMES Zycore zycore)

    add_library(xvm_zydis INTERFACE)
    target_include_directories(xvm_zydis INTERFACE "${ZYDIS_INCLUDE_DIR}")
    target_link_libraries(xvm_zydis INTERFACE "${ZYDIS_LIBRARY}")

    if(ZYCORE_LIBRARY)
        target_link_libraries(xvm_zydis INTERFACE "${ZYCORE_LIBRARY}")
    endif()

    set(XVM_ZYDIS_TARGET xvm_zydis)
endif()

find_package(xxHash CONFIG QUIET)
if(NOT xxHash_FOUND)
    find_package(xxhash CONFIG QUIET)
endif()

if(TARGET xxHash::xxhash)
    set(XVM_XXHASH_TARGET xxHash::xxhash)
elseif(TARGET xxhash::xxhash)
    set(XVM_XXHASH_TARGET xxhash::xxhash)
elseif(TARGET xxhash)
    set(XVM_XXHASH_TARGET xxhash)
elseif(TARGET xxHash::xxHash)
    set(XVM_XXHASH_TARGET xxHash::xxHash)
else()
    find_path(XXHASH_INCLUDE_DIR NAMES xxhash.h REQUIRED)
    find_library(XXHASH_LIBRARY NAMES xxhash libxxhash REQUIRED)

    add_library(xvm_xxhash INTERFACE)
    target_include_directories(xvm_xxhash INTERFACE "${XXHASH_INCLUDE_DIR}")
    target_link_libraries(xvm_xxhash INTERFACE "${XXHASH_LIBRARY}")

    set(XVM_XXHASH_TARGET xvm_xxhash)
endif()

find_package(unofficial-libyara CONFIG QUIET)
if(TARGET unofficial::libyara::libyara)
    set(XVM_YARA_TARGET unofficial::libyara::libyara)
elseif(TARGET libyara)
    set(XVM_YARA_TARGET libyara)
else()
    find_path(YARA_INCLUDE_DIR NAMES yara.h REQUIRED)
    find_library(YARA_LIBRARY NAMES yara libyara REQUIRED)

    add_library(xvm_yara_lib INTERFACE)
    target_include_directories(xvm_yara_lib INTERFACE "${YARA_INCLUDE_DIR}")
    target_link_libraries(xvm_yara_lib INTERFACE "${YARA_LIBRARY}")

    set(XVM_YARA_TARGET xvm_yara_lib)
endif()

add_library(xvm_core
    src/xvm_crypto.c
    src/xvm_disasm.c
    src/xvm_fast_hash.c
    src/xvm_yara.c
    src/xvm.c
)

target_include_directories(xvm_core
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(xvm_core
    PUBLIC
        xvm_options
    PRIVATE
        "${XVM_SODIUM_TARGET}"
        "${XVM_ZYDIS_TARGET}"
        "${XVM_XXHASH_TARGET}"
        "${XVM_YARA_TARGET}"
)

add_executable(xvm
    src/main.c
)

target_link_libraries(xvm
    PRIVATE
        xvm_core
)
