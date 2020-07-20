cmake_minimum_required(VERSION 3.1)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/rv2d.h" rv2ds_version_defines
     REGEX "#define RV2D__VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${rv2ds_version_defines})
    if(ver MATCHES "#define RV2D__VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(RV2D__VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(VERSION ${RV2D__VERSION_MAJOR}.${RV2D__VERSION_MINOR}.${RV2D__VERSION_PATCH})
message(STATUS "rv2d version ${VERSION}")

project(rv2d VERSION "${VERSION}" LANGUAGES CXX)

enable_testing()

option(RV2D_BUILD_EXAMPLES "Set to ON to build examples" ON)
option(RV2D_BUILD_TESTS "Set to ON to build tests" ON)
option(RV2D_ENABLE_INSTALL "Generate the install target" ON)

# request c++11 without gnu extension for the whole project and enable more warnings
if (RV2D_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD ${RV2D_CXX_STANDARD})
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W2")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wshadow")
endif()

add_library(rv2d INTERFACE)
add_library(rv2d::rv2d ALIAS rv2d)

# optionally, enable unicode support using the ICU library
set(RV2D_USE_UNICODE_HELP FALSE CACHE BOOL "Use ICU Unicode library")
if(RV2D_USE_UNICODE_HELP)
    find_package(PkgConfig)
    pkg_check_modules(ICU REQUIRED icu-uc)

    target_link_libraries(rv2d INTERFACE ${ICU_LDFLAGS})
    target_compile_options(rv2d INTERFACE ${ICU_CFLAGS})
    target_compile_definitions(rv2d INTERFACE RV2D_USE_UNICODE)
endif()

target_include_directories(rv2d INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

if(RV2D_ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)
    set(RV2D_CMAKE_DIR "lib/cmake/rv2d" CACHE STRING
      "Installation directory for cmake files, relative to ${CMAKE_INSTALL_PREFIX}.")
    set(version_config "${PROJECT_BINARY_DIR}/rv2d-config-version.cmake")
    set(project_config "${PROJECT_BINARY_DIR}/rv2d-config.cmake")
    set(targets_export_name rv2d-targets)

    # Generate the version, config and target files into the build directory.
    write_basic_package_version_file(
        ${version_config}
        VERSION ${VERSION}
        COMPATIBILITY AnyNewerVersion)
    configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/rv2d-config.cmake.in
        ${project_config}
        INSTALL_DESTINATION ${RV2D_CMAKE_DIR})
    export(TARGETS rv2d NAMESPACE rv2d::
        FILE ${PROJECT_BINARY_DIR}/${targets_export_name}.cmake)

    # Install version, config and target files.
    install(
        FILES ${project_config} ${version_config}
        DESTINATION ${RV2D_CMAKE_DIR})
    install(EXPORT ${targets_export_name} DESTINATION ${RV2D_CMAKE_DIR}
        NAMESPACE rv2d::)

    # Install the header file and export the target
    install(TARGETS rv2d EXPORT ${targets_export_name} DESTINATION lib)
    install(FILES ${PROJECT_SOURCE_DIR}/include/rv2d.h DESTINATION include)
endif()

add_subdirectory(src)