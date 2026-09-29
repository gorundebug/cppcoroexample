cmake_minimum_required(VERSION 3.24)

set(CPP_CORO_IO_BACKEND "epoll" CACHE STRING "Coro I/O backend: epoll or uring")
set_property(CACHE CPP_CORO_IO_BACKEND PROPERTY STRINGS epoll uring)

# This file is both a standalone service project and a workspace subdirectory.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(example_order_service LANGUAGES CXX)
  include(CTest)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  option(USE_CCACHE "Use ccache when it is available" ON)
  if(USE_CCACHE AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
      set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
      message(STATUS "C++ compiler cache: ${CCACHE_PROGRAM}")
    endif()
  endif()

  set(_servicelib_source_dir_default "")
  if(DEFINED ENV{SERVICELIB_SOURCE_DIR})
    set(_servicelib_source_dir_default "$ENV{SERVICELIB_SOURCE_DIR}")
  endif()
  set(SERVICELIB_SOURCE_DIR "${_servicelib_source_dir_default}" CACHE PATH
      "Path to a cppcoroservicelib source checkout")
  unset(_servicelib_source_dir_default)
  set(_servicegen_modules_root_default "${CMAKE_CURRENT_SOURCE_DIR}/modules")
  set(MODULES_ROOT "${_servicegen_modules_root_default}" CACHE PATH
      "Path containing local generated contract modules")
  option(FETCH_CPP_DEPENDENCIES
         "Fetch pinned cppcoroservicelib and missing contract modules" ON)
  set(CPPCOROSERVICELIB_REPOSITORY "https://github.com/gorundebug/cppcoroservicelib.git" CACHE STRING
      "Standalone cppcoroservicelib repository")
  set(CPPCOROSERVICELIB_REVISION "v0.2.149" CACHE STRING
      "Pinned cppcoroservicelib revision")
  set(CPPCOROSERVICELIB_ENABLE_GRPC ON
      CACHE BOOL "Build asio-grpc support" FORCE)
  set(CPPCOROSERVICELIB_ENABLE_KAFKA ON
      CACHE BOOL "Build librdkafka support" FORCE)
  set(CPPCOROSERVICELIB_ENABLE_OTEL ON
      CACHE BOOL "Build OpenTelemetry tracing and logging support" FORCE)
  set(CPPCOROSERVICELIB_BUILD_TESTS OFF CACHE BOOL
      "Do not register framework self-tests in a standalone consumer" FORCE)
  # The canonical standalone toolchain installs the exact dependency graph
  # through Conan whether the framework source is local or fetched. Keep this
  # as an overridable cache default so an explicit host build may still select
  # another provider without silently mixing Conan and system packages.
  set(CPPCOROSERVICELIB_DEPENDENCY_MODE "CONAN" CACHE STRING
      "Dependency provider for standalone cppcoroservicelib")

  if(SERVICELIB_SOURCE_DIR)
    add_subdirectory(
        "${SERVICELIB_SOURCE_DIR}"
        "${CMAKE_BINARY_DIR}/_deps/cppcoroservicelib-build"
        EXCLUDE_FROM_ALL)
  elseif(FETCH_CPP_DEPENDENCIES)
    include(FetchContent)
    if(DEFINED ENV{DEPENDENCY_GITHUB_RAW_URL} AND
       NOT "$ENV{DEPENDENCY_GITHUB_RAW_URL}" STREQUAL "" AND
       CPPCOROSERVICELIB_REPOSITORY MATCHES "^https://github.com/")
      string(REGEX REPLACE "^https://github.com/" ""
          _servicegen_framework_path "${CPPCOROSERVICELIB_REPOSITORY}")
      string(REGEX REPLACE "\\.git$" ""
          _servicegen_framework_path "${_servicegen_framework_path}")
      FetchContent_Declare(servicegen_cppcoroservicelib
          URL "$ENV{DEPENDENCY_GITHUB_RAW_URL}/${_servicegen_framework_path}/archive/${CPPCOROSERVICELIB_REVISION}.tar.gz"
          DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    else()
      FetchContent_Declare(servicegen_cppcoroservicelib
          GIT_REPOSITORY "${CPPCOROSERVICELIB_REPOSITORY}"
          GIT_TAG "${CPPCOROSERVICELIB_REVISION}"
          GIT_SHALLOW FALSE
          GIT_PROGRESS TRUE)
    endif()
    FetchContent_MakeAvailable(servicegen_cppcoroservicelib)
  else()
    find_package(cppcoroservicelib CONFIG REQUIRED)
  endif()

  if(NOT TARGET example_inventory_service_api)
    set(_servicegen_module_source "${MODULES_ROOT}/inventory_service_api/cppcoro")
    if(EXISTS "${_servicegen_module_source}/CMakeLists.txt")
      add_subdirectory("${_servicegen_module_source}"
          "${CMAKE_BINARY_DIR}/_modules/inventory_service_api")
    elseif(FETCH_CPP_DEPENDENCIES)
      include(FetchContent)
      FetchContent_Declare(servicegen_module_inventory_service_api
          GIT_REPOSITORY "https://github.com/gorundebug/cppcoroexample.git"
          GIT_TAG "v0.2.149"
          SOURCE_SUBDIR "inventory_service_api/cppcoro"
          GIT_SHALLOW FALSE
          GIT_PROGRESS TRUE)
      FetchContent_MakeAvailable(servicegen_module_inventory_service_api)
    else()
      message(FATAL_ERROR "Contract module inventory_service_api was not found")
    endif()
  endif()
  if(NOT TARGET example_model)
    set(_servicegen_module_source "${MODULES_ROOT}/model_cppcoro")
    if(EXISTS "${_servicegen_module_source}/CMakeLists.txt")
      add_subdirectory("${_servicegen_module_source}"
          "${CMAKE_BINARY_DIR}/_modules/model_cppcoro")
    elseif(FETCH_CPP_DEPENDENCIES)
      include(FetchContent)
      FetchContent_Declare(servicegen_module_model
          GIT_REPOSITORY "https://github.com/gorundebug/cppcoroexample.git"
          GIT_TAG "v0.2.149"
          SOURCE_SUBDIR "model_cppcoro/"
          GIT_SHALLOW FALSE
          GIT_PROGRESS TRUE)
      FetchContent_MakeAvailable(servicegen_module_model)
    else()
      message(FATAL_ERROR "Contract module model_cppcoro was not found")
    endif()
  endif()
  if(NOT TARGET example_order_service_api)
    set(_servicegen_module_source "${MODULES_ROOT}/order_service_api/cppcoro")
    if(EXISTS "${_servicegen_module_source}/CMakeLists.txt")
      add_subdirectory("${_servicegen_module_source}"
          "${CMAKE_BINARY_DIR}/_modules/order_service_api")
    elseif(FETCH_CPP_DEPENDENCIES)
      include(FetchContent)
      FetchContent_Declare(servicegen_module_order_service_api
          GIT_REPOSITORY "https://github.com/gorundebug/cppcoroexample.git"
          GIT_TAG "v0.2.149"
          SOURCE_SUBDIR "order_service_api/cppcoro"
          GIT_SHALLOW FALSE
          GIT_PROGRESS TRUE)
      FetchContent_MakeAvailable(servicegen_module_order_service_api)
    else()
      message(FATAL_ERROR "Contract module order_service_api was not found")
    endif()
  endif()
endif()

if(NOT TARGET servicelib::coro_event_engine)
  message(FATAL_ERROR "cppCoro requires cppcoroservicelib with the shared Callback API EventEngine")
endif()
if(NOT TARGET servicelib::coro_io)
  message(FATAL_ERROR "cppCoro requires a runtime with an explicit epoll/uring build backend")
endif()
get_target_property(_coro_runtime_backend servicelib::coro_io INTERFACE_SERVICELIB_CORO_IO_BACKEND)
if(NOT _coro_runtime_backend STREQUAL CPP_CORO_IO_BACKEND)
  message(FATAL_ERROR "Coro I/O backend mismatch: generated project=${CPP_CORO_IO_BACKEND}, runtime=${_coro_runtime_backend}")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(JEMALLOC REQUIRED IMPORTED_TARGET jemalloc)

set(_servicegen_include_root
    "${CMAKE_CURRENT_BINARY_DIR}/servicegen-include")
file(MAKE_DIRECTORY "${_servicegen_include_root}")
if(NOT EXISTS
    "${_servicegen_include_root}/orderservice")
  file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}"
      "${_servicegen_include_root}/orderservice"
      SYMBOLIC COPY_ON_ERROR)
endif()


add_executable(example_order_service
    cmd/service/main.cpp
    internal/app/service.generated.cpp

    internal/app/service.cpp
    internal/app/makers.generated.cpp
    internal/app/functions.generated.cpp
    internal/app/streams.generated.cpp
    internal/app/endpoints.generated.cpp
    internal/app/servers.generated.cpp
    internal/app/substreams.generated.cpp

    internal/functions/order/map_order_item_result_to_order_state.cpp
    internal/functions/order/map_to_order_processed.cpp
    internal/functions/order/map_to_order_state.cpp
    internal/functions/order/process_order_items.cpp
    internal/functions/order/soft_deadline.cpp
)
target_compile_features(example_order_service PRIVATE cxx_std_20)
target_include_directories(example_order_service PRIVATE "${_servicegen_include_root}")
target_link_libraries(example_order_service PRIVATE
    PkgConfig::JEMALLOC
    servicelib::servicelib servicelib::coro_event_engine servicelib::otel servicelib::grpc servicelib::kafka
    example_inventory_service_api
    example_model
    example_order_service_api
    example_inventory_service_api
    example_order_service_api
)

if(BUILD_TESTING)
add_executable(example_order_service_typed_config config/config.generated.cpp)
target_compile_features(example_order_service_typed_config PRIVATE cxx_std_20)
target_include_directories(example_order_service_typed_config PRIVATE
    "${_servicegen_include_root}")
target_link_libraries(example_order_service_typed_config PRIVATE
    servicelib::servicelib)
add_test(NAME example_order_service_typed_config COMMAND example_order_service_typed_config)

if(NOT TARGET GTest::gtest_main)
  if(CPPCOROSERVICELIB_DEPENDENCY_MODE STREQUAL "CONAN")
    find_package(GTest CONFIG REQUIRED)
  else()
    include(FetchContent)
    FetchContent_Declare(servicegen_googletest
        URL "${CPPCOROSERVICELIB_GITHUB_ARCHIVE_BASE}/google/googletest/archive/refs/tags/${CPPCOROSERVICELIB_GOOGLETEST_VERSION}.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP FALSE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(servicegen_googletest)
  endif()
endif()
include(GoogleTest)

add_executable(example_order_service_unit_tests
    internal/functions/endpoint/order_processed_endpoint_sink_test.cpp
    internal/functions/endpoint/process_order_item_sink_test.cpp
    internal/functions/endpoint/process_order_source_test.cpp
    internal/functions/order/map_order_item_result_to_order_state_test.cpp
    internal/functions/order/map_to_order_processed_test.cpp
    internal/functions/order/map_to_order_state_test.cpp
    internal/functions/order/process_order_items_test.cpp
    internal/functions/order/soft_deadline_test.cpp
)
target_compile_features(example_order_service_unit_tests PRIVATE cxx_std_20)
target_include_directories(example_order_service_unit_tests PRIVATE
    "${_servicegen_include_root}")
target_link_libraries(example_order_service_unit_tests PRIVATE
    servicelib::servicelib GTest::gtest_main servicelib::grpc servicelib::kafka
    example_inventory_service_api
    example_model
    example_order_service_api
    example_inventory_service_api
    example_order_service_api
)
gtest_discover_tests(example_order_service_unit_tests)

endif()