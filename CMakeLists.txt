cmake_minimum_required(VERSION 3.0)
project(avakar_variant)

include(deps.cmake)

include(CheckIncludeFile)
check_include_file("variant" HAVE_VARIANT)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(avakar_variant INTERFACE)

target_include_directories(
    avakar_variant
    INTERFACE include
    )

if(NOT HAVE_VARIANT)

target_include_directories(
    avakar_variant SYSTEM
    INTERFACE include_std
    )

endif()

add_executable(avakar_variant_test
    include_std/variant
    include/avakar/monostate.hpp
    include/avakar/variant.hpp
    src/variant_alternative.hpp
    src/variant_fwd.hpp
    src/variant_helpers.hpp
    src/variant_impl.hpp
    src/variant_size.hpp
    src/variant_visit.hpp
    test/variant.cpp
    )
target_link_libraries(avakar_variant_test avakar_variant mutest)

source_group(include REGULAR_EXPRESSION include/.*|include_std/.*)
source_group(src REGULAR_EXPRESSION src/.*)
source_group(test REGULAR_EXPRESSION test/.*)
