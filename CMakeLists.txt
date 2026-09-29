cmake_minimum_required(VERSION 3.24)

set(CPP_CORO_IO_BACKEND "epoll" CACHE STRING "Coro I/O backend: epoll or uring")
set_property(CACHE CPP_CORO_IO_BACKEND PROPERTY STRINGS epoll uring)

project(example LANGUAGES CXX)

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
option(FETCH_CPP_DEPENDENCIES
       "Fetch the pinned cppcoroservicelib source tree when absent" OFF)
set(CPPCOROSERVICELIB_REPOSITORY
    "https://github.com/gorundebug/cppcoroservicelib.git" CACHE STRING
    "Standalone cppcoroservicelib repository")
set(CPPCOROSERVICELIB_REVISION
    "v0.2.148" CACHE STRING
    "Pinned cppcoroservicelib revision")
set(CPPCOROSERVICELIB_DEPENDENCY_MODE "CONAN" CACHE STRING
    "Dependency provider used for a source-tree cppcoroservicelib build")
set(CPPCOROSERVICELIB_BUILD_TESTS OFF CACHE BOOL
    "Do not build framework self-tests in a generated consumer workspace" FORCE)

set(CPPCOROSERVICELIB_ENABLE_GRPC ON
    CACHE BOOL "Build asio-grpc support" FORCE)
set(CPPCOROSERVICELIB_ENABLE_KAFKA ON
    CACHE BOOL "Build librdkafka support" FORCE)
set(CPPCOROSERVICELIB_ENABLE_OTEL ON
    CACHE BOOL "Build OpenTelemetry tracing and logging support" FORCE)

if(SERVICELIB_SOURCE_DIR)
  add_subdirectory(
      "${SERVICELIB_SOURCE_DIR}"
      "${CMAKE_BINARY_DIR}/_deps/cppcoroservicelib-build"
      EXCLUDE_FROM_ALL)
elseif(FETCH_CPP_DEPENDENCIES)
  include(FetchContent)
  FetchContent_Declare(servicegen_cppcoroservicelib
      GIT_REPOSITORY "${CPPCOROSERVICELIB_REPOSITORY}"
      GIT_TAG "${CPPCOROSERVICELIB_REVISION}"
      GIT_SHALLOW FALSE
      GIT_PROGRESS TRUE)
  FetchContent_MakeAvailable(servicegen_cppcoroservicelib)
else()
  find_package(cppcoroservicelib CONFIG REQUIRED)
endif()

if(NOT TARGET servicelib::servicelib)
  message(FATAL_ERROR
      "cppcoroservicelib did not provide servicelib::servicelib")
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

if(NOT TARGET servicelib::otel)
  message(FATAL_ERROR
      "cppcoroservicelib was built without OpenTelemetry support")
endif()
if(NOT TARGET servicelib::grpc)
  message(FATAL_ERROR
      "cppcoroservicelib was built without asio-grpc support")
endif()

if(NOT TARGET servicelib::cron)
  message(FATAL_ERROR
      "cppcoroservicelib was built without libcron support")
endif()

if(NOT TARGET servicelib::kafka)
  message(FATAL_ERROR
      "cppcoroservicelib was built without librdkafka support")
endif()

add_subdirectory(inventory_service_api/cppcoro)
add_subdirectory(model_cppcoro)
add_subdirectory(order_service_api/cppcoro)
add_subdirectory(analyticsservice)
add_subdirectory(inventoryservice)
add_subdirectory(orderservice)